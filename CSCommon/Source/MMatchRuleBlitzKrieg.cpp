
#include "stdafx.h"
#include "MNewQuestNpcManager.h"
#include "MNewQuestPlayerManager.h"
#include "MActorDef.h"
#include "MMatchRuleBlitzKrieg.h"


MMatchRuleBlitzKrieg::MMatchRuleBlitzKrieg(MMatchStage* pStage) : MMatchRule(pStage)
, m_pPlayerMgr(new MNewQuestPlayerManager)
, m_pNpcMgr(new MNewQuestNpcManager)
{

}

void MMatchRuleBlitzKrieg::OnBegin()
{
}

void MMatchRuleBlitzKrieg::OnEnd()
{
}

bool MMatchRuleBlitzKrieg::OnRun()
{
	bool ret = MMatchRule::OnRun();

	return ret;
}

void MMatchRuleBlitzKrieg::OnRoundBegin()
{
	for (MUIDRefCache::iterator it = m_pStage->GetObjBegin(); it != m_pStage->GetObjEnd(); ++it)
	{
		MUID uidChar = it->first;
		if (uidChar.IsValid()) {
			MMatchObject* pObj = MMatchServer::GetInstance()->GetObject(uidChar);
			if (IsAdminGrade(pObj) && pObj->CheckPlayerFlags(MTD_PlayerFlags_AdminHide)) {
				continue;
			}

			m_pPlayerMgr->AddPlayer(pObj);
		}
	}

	NPCSpawn();

	MMatchRule::OnRoundBegin();
}

void MMatchRuleBlitzKrieg::OnRoundEnd()
{
	if (m_pStage != NULL)
	{
		switch (m_nRoundArg)
		{
		case MMATCH_ROUNDRESULT_BLUE_ALL_OUT: m_pStage->OnRoundEnd_FromTeamGame(MMT_RED); break;
		case MMATCH_ROUNDRESULT_RED_ALL_OUT: m_pStage->OnRoundEnd_FromTeamGame(MMT_BLUE); break;
		case MMATCH_ROUNDRESULT_REDWON: m_pStage->OnRoundEnd_FromTeamGame(MMT_RED); break;
		case MMATCH_ROUNDRESULT_BLUEWON: m_pStage->OnRoundEnd_FromTeamGame(MMT_BLUE); break;
		case MMATCH_ROUNDRESULT_DRAW: break;
		}
	}

	MMatchRule::OnRoundEnd();
}

bool MMatchRuleBlitzKrieg::OnCheckEnableBattleCondition()
{
	//// 선승제일 경우는 Free상태가 안된다.
	//if (m_pStage->GetStageSetting()->IsTeamWinThePoint() == true)
	//{
	//	return true;
	//}

	//int nRedTeam = 0, nBlueTeam = 0;
	//int nPreRedTeam = 0, nPreBlueTeam = 0;
	//int nStageObjects = 0;		// 게임안에 없고 스테이지에 있는 사람

	//MMatchStage* pStage = GetStage();
	//if (pStage == NULL) return false;

	//for (MUIDRefCache::iterator i = pStage->GetObjBegin(); i != pStage->GetObjEnd(); i++)
	//{
	//	MMatchObject* pObj = (MMatchObject*)(*i).second;
	//	if ((pObj->GetEnterBattle() == false) && (!pObj->IsLaunchedGame()))
	//	{
	//		nStageObjects++;
	//		continue;
	//	}

	//	if (pObj->GetTeam() == MMT_RED)
	//	{
	//		nRedTeam++;
	//	}
	//	else if (pObj->GetTeam() == MMT_BLUE)
	//	{
	//		nBlueTeam++;
	//	}
	//}

	//if (nRedTeam == 0 || nBlueTeam == 0)
	//{
	//	return false;
	//}

	return true;
}

// 만약 레드팀이나 블루팀에서 팀원이 0명일 경우는 false 반환 , true,false 모두 AliveCount 반환
bool MMatchRuleBlitzKrieg::GetAliveCount(int* pRedAliveCount, int* pBlueAliveCount)
{
	int nRedCount = 0, nBlueCount = 0;
	int nRedAliveCount = 0, nBlueAliveCount = 0;
	(*pRedAliveCount) = 0;
	(*pBlueAliveCount) = 0;

	MMatchStage* pStage = GetStage();
	if (pStage == NULL) return false;

	for (MUIDRefCache::iterator i = pStage->GetObjBegin(); i != pStage->GetObjEnd(); i++)
	{
		MMatchObject* pObj = (MMatchObject*)(*i).second;
		if (pObj->GetEnterBattle() == false) continue;	// 배틀참가하고 있는 플레이어만 체크

		if (pObj->GetTeam() == MMT_RED)
		{
			nRedCount++;
			if (pObj->CheckAlive() == true)
			{
				nRedAliveCount++;
			}
		}
		else if (pObj->GetTeam() == MMT_BLUE)
		{
			nBlueCount++;
			if (pObj->CheckAlive() == true)
			{
				nBlueAliveCount++;
			}
		}
	}

	(*pRedAliveCount) = nRedAliveCount;
	(*pBlueAliveCount) = nBlueAliveCount;

	if ((nRedAliveCount == 0) || (nBlueAliveCount == 0))
	{
		return false;
	}
	return true;
}

bool MMatchRuleBlitzKrieg::OnCheckRoundFinish()
{
	//int nRedAliveCount = 0;
	//int nBlueAliveCount = 0;

	//// 팀원이 0명인 팀이 있으면 false반환
	//if (GetAliveCount(&nRedAliveCount, &nBlueAliveCount) == false)
	//{
	//	int nRedTeam = 0, nBlueTeam = 0;
	//	int nStageObjects = 0;		// 게임안에 없고 스테이지에 있는 사람

	//	MMatchStage* pStage = GetStage();

	//	for (MUIDRefCache::iterator i = pStage->GetObjBegin(); i != pStage->GetObjEnd(); i++)
	//	{
	//		MMatchObject* pObj = (MMatchObject*)(*i).second;
	//		if ((pObj->GetEnterBattle() == false) && (!pObj->IsLaunchedGame()))
	//		{
	//			nStageObjects++;
	//			continue;
	//		}

	//		if (pObj->GetTeam() == MMT_RED)		nRedTeam++;
	//		else if (pObj->GetTeam() == MMT_BLUE)	nBlueTeam++;
	//	}

	//	if (nBlueTeam == 0 && (pStage->GetTeamScore(MMT_BLUE) > pStage->GetTeamScore(MMT_RED)))
	//		SetRoundArg(MMATCH_ROUNDRESULT_BLUE_ALL_OUT);
	//	else if (nRedTeam == 0 && (pStage->GetTeamScore(MMT_RED) > pStage->GetTeamScore(MMT_BLUE)))
	//		SetRoundArg(MMATCH_ROUNDRESULT_RED_ALL_OUT);
	//	else if ((nRedAliveCount == 0) && (nBlueAliveCount == 0))
	//		SetRoundArg(MMATCH_ROUNDRESULT_DRAW);
	//	else if (nRedAliveCount == 0)
	//		SetRoundArg(MMATCH_ROUNDRESULT_BLUEWON);
	//	else if (nBlueAliveCount == 0)
	//		SetRoundArg(MMATCH_ROUNDRESULT_REDWON);
	//}

	//if (nRedAliveCount == 0 || nBlueAliveCount == 0) return true;
	//else return false;

	return false;
}

void MMatchRuleBlitzKrieg::OnRoundTimeOut()
{
	int nRedAliveCount = 0;
	int nBlueAliveCount = 0;
	GetAliveCount(&nRedAliveCount, &nBlueAliveCount);

	if (nRedAliveCount > nBlueAliveCount) {
		SetRoundArg(MMATCH_ROUNDRESULT_REDWON);
	}
	else if (nBlueAliveCount > nRedAliveCount) {
		SetRoundArg(MMATCH_ROUNDRESULT_BLUEWON);
	}
	else {
		SetRoundArg(MMATCH_ROUNDRESULT_DRAW);
	}
}

// 반환값이 false이면 게임이 끝난다.
bool MMatchRuleBlitzKrieg::RoundCount()
{
	if (m_pStage == NULL) return false;

	int nTotalRound = m_pStage->GetStageSetting()->GetRoundMax();
	m_nRoundCount++;

	if (m_pStage->GetStageSetting()->IsTeamWinThePoint() == false)
	{
		// 선승제가 아닐 경우
		if (m_nRoundCount < nTotalRound) return true;

	}
	else
	{
		// 선승제일 경우 

		// 팀원이 0명인 팀이 있어도 게임이 끝난다.
		int nRedTeamCount = 0, nBlueTeamCount = 0;
		m_pStage->GetTeamMemberCount(&nRedTeamCount, &nBlueTeamCount, NULL, true);

		if ((nRedTeamCount == 0) || (nBlueTeamCount == 0))
		{
			return false;
		}

		int nRedScore = m_pStage->GetTeamScore(MMT_RED);
		int nBlueScore = m_pStage->GetTeamScore(MMT_BLUE);

		// 래더게임에서 먼저 4승인 팀이 승리
		const int LADDER_WINNING_ROUNT_COUNT = 4;


		// 두팀이 모두 4승이 아니면 true반환
		if ((nRedScore < LADDER_WINNING_ROUNT_COUNT) && (nBlueScore < LADDER_WINNING_ROUNT_COUNT))
		{
			return true;
		}
	}

	return false;
}

void MMatchRuleBlitzKrieg::CalcTeamBonus(MMatchObject* pAttacker, MMatchObject* pVictim,
	int nSrcExp, int* poutAttackerExp, int* poutTeamExp)
{
	if (m_pStage == NULL)
	{
		*poutAttackerExp = nSrcExp;
		*poutTeamExp = 0;
		return;
	}

	*poutTeamExp = (int)(nSrcExp * m_pStage->GetStageSetting()->GetCurrGameTypeInfo()->fTeamBonusExpRatio);
	*poutAttackerExp = (int)(nSrcExp * m_pStage->GetStageSetting()->GetCurrGameTypeInfo()->fTeamMyExpRatio);
}

void MMatchRuleBlitzKrieg::NPCSpawn() 
{
	SpawnActor("radar_red", MMT_RED, 0);
	SpawnActor("radar_blue", MMT_BLUE, 0);

	for (int i = 0; i != 12; i++) {
		SpawnActor("barricade_red", MMT_RED, i);
		SpawnActor("barricade_blue", MMT_BLUE, i);
	}

	SpawnActor("guardian", MMT_RED, 0);
	SpawnActor("guardian", MMT_BLUE, 0);
}

void MMatchRuleBlitzKrieg::SpawnActor(const char* szActorName, MMatchTeam nTeam, int nPosIndex)
{
	MUID uidController = m_pPlayerMgr->FindSuitableController();
	if (uidController.IsInvalid()) {
		mlog("Failed to find Suitable Controller\n");
		return;
	}

	MActorDef* pActor = MGetActorDefMgr()->GetDef(szActorName);
	if (!pActor) {
		mlog("Failed to find actor %s\n", szActorName);
		return;
	}

	MUID uidNPC = MGetMatchServer()->UseUID();

	m_pNpcMgr->AddNpcObject(uidNPC, uidController, 0);
	m_pPlayerMgr->IncreaseNpcControl(uidController, uidNPC);

	MCommand* pCmd = MGetMatchServer()->CreateCommand(MC_BLITZ_NPC_SPAWN, MUID(0, 0));
	pCmd->AddParameter(new MCmdParamUID(uidController));
	pCmd->AddParameter(new MCmdParamUID(uidNPC));
	pCmd->AddParameter(new MCmdParamStr(szActorName));
	pCmd->AddParameter(new MCmdParamUChar(nTeam));
	pCmd->AddParameter(new MCmdParamUChar(nPosIndex));
	pCmd->AddParameter(new MCmdParamUShort(pActor->GetMaxHp()));
	pCmd->AddParameter(new MCmdParamUShort(pActor->GetMaxAp()));
	pCmd->AddParameter(new MCmdParamFloat(1.0f));
	MGetMatchServer()->RouteToBattle(m_pStage->GetUID(), pCmd);	
}
