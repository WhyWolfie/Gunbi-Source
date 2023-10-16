#include "stdafx.h"
#include "MMatchRuleWar.h"

#include "MNewQuestNpcManager.h"
#include "MNewQuestPlayerManager.h"

MMatchRuleWar::MMatchRuleWar(MMatchStage* pStage) : MMatchRuleTeamDeath2(pStage)
, m_pPlayerMgr(new MNewQuestPlayerManager)
, m_pNpcMgr(new MNewQuestNpcManager)
{
	m_nLoseTeam = MMT_ALL;
}

void MMatchRuleWar::OnRoundBegin()
{
	MMatchRule::OnRoundBegin();
}

void MMatchRuleWar::OnRoundEnd()
{
	MWarManager * pMgr = MGetWarMgr();
	if (!pMgr) {
		return;
	}

	if (m_pStage->GetUID() == pMgr->GetStageUID()) {

		for (auto itor = m_pStage->GetObjBegin(); itor != m_pStage->GetObjEnd(); itor++)
		{
			MUID uid = (*itor).first;
			if (uid.IsInvalid()) {
				continue;
			}

			MMatchObject* pObj = MMatchServer::GetInstance()->GetObjectA(uid);
			if (pObj) {
				MWarReward pReward;
				if (m_nLoseTeam == MMT_ALL) {
					pReward = pMgr->GetReward()[2];
				}
				else if (pObj->GetTeam() == m_nLoseTeam) {
					pReward = pMgr->GetReward()[0];
				}
				else {
					pReward = pMgr->GetReward()[1];
				}

				pObj->GetCharInfo()->IncBP(pReward.nBp);
				pObj->GetCharInfo()->IncXP(pReward.nXp);

				if (pReward.nCash != 0) {
					pObj->GetAccountInfo()->m_nCashCoins += pReward.nCash;
					MMatchServer::GetInstance()->GetDBMgr()->UpdateAccCash(pObj->GetAccountInfo()->m_nAID, pReward.nCash);
				}

				pMgr->Announce("You received your reward!", ZAAT_MSGBOX, pObj->GetUID());
			}
		}
		m_pStage->ChangeState(STAGE_STATE_CLOSE);
	}

	m_nLoseTeam = MMT_ALL;
}

void MMatchRuleWar::SpawnTower(MUID& uidChar, MMatchTeam nTeam)
{
	MWarNPC pNpc = MGetWarMgr()->GetNpc()[nTeam-2];

	auto itor = m_vTowers.find(nTeam);
	if (itor != m_vTowers.end()) {
		MUID uid = (*itor).second;
		if (uid.IsValid()) {
			MNewQuestNpcObject* pNpcObj = m_pNpcMgr->GetNpc(uid);
			if (pNpcObj) {
				MObject* pObj = MMatchServer::GetInstance()->GetObjectA(uidChar);
				if (pObj) {
					MCommand* pCmd = MGetMatchServer()->CreateCommand(MC_WAR_SPAWN_TOWER, MUID(0, 0));
					if (pCmd) {
						pCmd->AddParameter(new MCmdParamUID(pNpcObj->GetController()));
						pCmd->AddParameter(new MCmdParamStr(pNpc.strModel.c_str()));
						pCmd->AddParameter(new MCmdParamUID(uid));
						pCmd->AddParameter(new MCmdParamUChar(nTeam));
						pCmd->AddParameter(new MCmdParamUShort(pNpcObj->GetHp()));
						pCmd->AddParameter(new MCmdParamUShort(pNpcObj->GetAp()));
						pCmd->AddParameter(new MCmdParamPos(pNpc.fPosX, pNpc.fPosY, pNpc.fPosZ));
						pCmd->AddParameter(new MCmdParamDir(pNpc.fDirX, pNpc.fDirY, pNpc.fDirZ));
						MGetMatchServer()->RouteToListener(pObj, pCmd);
					}
					return;
				}
			}
		}
	}

	MUID uidController = m_pPlayerMgr->FindSuitableController();
	if (uidController.IsInvalid()) {
		return;
	}

	MUID uidNPC = MGetMatchServer()->UseUID();
	if (uidNPC.IsInvalid()) {
		return;
	}

	MObject* pObj = MMatchServer::GetInstance()->GetObjectA(uidController);
	if (!pObj) {
		return;
	}

	m_pNpcMgr->AddNpcObject(uidNPC, uidController, 0);
	m_pPlayerMgr->IncreaseNpcControl(uidController, uidNPC);

	MNewQuestNpcObject* pNpcObj = m_pNpcMgr->GetNpc(uidNPC);
	if (!pNpcObj) {
		return;
	}

	pNpcObj->SetAp(pNpc.nAP);
	pNpcObj->SetHp(pNpc.nHP);

	MCommand* pCmd = MGetMatchServer()->CreateCommand(MC_WAR_SPAWN_TOWER, MUID(0, 0));
	if (pCmd) {
		pCmd->AddParameter(new MCmdParamUID(uidController));
		pCmd->AddParameter(new MCmdParamStr(pNpc.strModel.c_str()));
		pCmd->AddParameter(new MCmdParamUID(uidNPC));
		pCmd->AddParameter(new MCmdParamUChar(nTeam));
		pCmd->AddParameter(new MCmdParamUShort(pNpcObj->GetHp()));
		pCmd->AddParameter(new MCmdParamUShort(pNpcObj->GetAp()));
		pCmd->AddParameter(new MCmdParamPos(pNpc.fPosX, pNpc.fPosY, pNpc.fPosZ));
		pCmd->AddParameter(new MCmdParamDir(pNpc.fDirX, pNpc.fDirY, pNpc.fDirZ));
		MGetMatchServer()->RouteToListener(pObj, pCmd);
	}

	m_vTowers.insert(pair<MMatchTeam, MUID>(nTeam, uidNPC));
}

void MMatchRuleWar::OnEnterBattle(MUID& uidChar) {

	if (uidChar.IsInvalid()) {
		return;
	}

	MMatchObject* pObj = MMatchServer::GetInstance()->GetObject(uidChar);
	if (pObj) {
		if (!pObj->CheckPlayerFlags(MTD_PlayerFlags_AdminHide)) {
			m_pPlayerMgr->AddPlayer(pObj);
		}
	}

	SpawnTower(uidChar, MMT_RED);
	SpawnTower(uidChar, MMT_BLUE);
}

void MMatchRuleWar::OnLeaveBattle(MUID& uidChar) {
	m_pPlayerMgr->RemovePlayer(uidChar);
}

void MMatchRuleWar::RequestNPCDead(MUID uidAttacker, MUID uidNPC) {

	if (uidAttacker.IsInvalid()) {
		return;
	}

	for (auto itor = m_vTowers.begin(); itor != m_vTowers.end(); itor++) {
		MUID uidTower = (*itor).second;
		if (uidTower == uidNPC) {
			MMatchTeam nTeamId = (*itor).first;

			m_pStage->AddTeamKills(nTeamId == MMT_BLUE ? MMT_RED : MMT_BLUE);
			m_nLoseTeam = nTeamId;
			return;
		}
	}
}

void MMatchRuleWar::UpdateNPCHpAp(MUID uidOwner, MUID uidNpc, int nHP, int nAP) {
	if (uidOwner.IsInvalid()) {
		return;
	}

	MNewQuestNpcObject* pNpc = m_pNpcMgr->GetNpc(uidNpc);
	if (!pNpc) {
		return;
	}

	if (pNpc->GetController() != uidOwner) {
		return;
	}

	pNpc->SetHp(nHP);
	pNpc->SetAp(nAP);
}

bool MMatchRuleWar::OnCheckRoundFinish()
{
	if (!m_pStage) {
		return false;
	}

	if (m_nLoseTeam != MMT_ALL) {

		SetRoundArg(m_nLoseTeam == MMT_BLUE ? MMATCH_ROUNDRESULT_REDWON : MMATCH_ROUNDRESULT_BLUEWON);
		return true;
	}
	return false;
}