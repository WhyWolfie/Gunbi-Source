#include "stdafx.h"
#include "MBlobArray.h"
#include "MMatchConfig.h"
#include "MMatchServer.h"
#include "MMatchRuleWar.h"

bool MMatchServer::LaunchWarMatch(MWarEvent pEvent) {

	MUID uidStage = MUID(0, 0);
	if (!StageAdd(NULL, "WarMode_Stage", true, "", &uidStage, true)) {
		LOG(LOG_PROG, "Can't Add Stage - StageUID(%d%d)", uidStage.High, uidStage.Low);
		return false;
	}

	MMatchStage* pStage = FindStage(uidStage);
	if (pStage == nullptr) {
		return false;
	}

	if (!pStage->IsValidMap(pEvent.strMap.c_str())) {
		return false;
	}

	pStage->ChangeRule(MMATCH_GAMETYPE_WAR);
	pStage->SetStageType(MST_NORMAL);

	MMatchStageSetting* pSetting = pStage->GetStageSetting();
	if (pSetting == nullptr) {
		return false;
	}

	pSetting->SetMasterUID(MUID(0, 0));
	pSetting->SetMapName((char*)pEvent.strMap.c_str());
	pSetting->SetGameType(MMATCH_GAMETYPE_WAR);
	pSetting->SetLimitTime(pEvent.nTime);
	pSetting->SetRoundMax(1);

	MCommand* pCmd = CreateCmdResponseStageSetting(uidStage);
	if (pCmd) {
		RouteToStage(uidStage, pCmd);
	}

	if (pStage->StartGame(MGetServerConfig()->IsUseResourceCRC32CacheCheck()) == true) {
		ReserveAgent(pStage);
	
		MGetWarMgr()->SetStageUID(uidStage);

		return true;
	}
	
	return false;
}

void MMatchServer::ResponseWarJoinGame(MUID uid) {
	MMatchObject* pObj = GetObject(uid);
	if (pObj == NULL) {
		return;
	}

	if (pObj->GetStageUID() != MUID(0, 0)) {
		StageLeave(pObj->GetUID());
	}

	MUID uidStage = MGetWarMgr()->GetStageUID();
	if (uidStage == MUID(0, 0)) {
		return;
	}

	StageJoin(uid, uidStage);
}

void MMatchServer::RequestWarNPCDead(MUID uidAttacker, MUID uidNPC) {
	MMatchObject* pObj = GetObject(uidAttacker);
	if (pObj == nullptr) {
		return;
	}
	
	MMatchStage* pStage = FindStage(pObj->GetStageUID());
	if (pStage == nullptr) {
		return;
	}

	MMatchRuleWar* pRule = (MMatchRuleWar*)pStage->GetRule();
	if (pRule == nullptr) {
		return;
	}

	pRule->RequestNPCDead(uidAttacker, uidNPC);
}

void MMatchServer::UpdateWarNPCHpAp(MUID uidOwner, MUID uidNpc, int nHP, int nAP) {

	MMatchObject* pObj = GetObject(uidOwner);
	if (pObj == nullptr) {
		return;
	}

	MMatchStage* pStage = FindStage(pObj->GetStageUID());
	if (pStage == nullptr) {
		return;
	}

	MMatchRuleWar* pRule = (MMatchRuleWar*)pStage->GetRule();
	if (pRule == nullptr) {
		return;
	}

	pRule->UpdateNPCHpAp(uidOwner, uidNpc, nHP, nAP);
}