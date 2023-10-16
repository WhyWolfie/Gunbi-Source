#include "stdafx.h"
#include "ZRuleBlitzKrieg.h"
#include "ZMatch.h"
#include "MActorDef.h"
#include "MNewQuestScenario.h"
#include "MBlitzKriegDef.h"
#include "MMatchBlitzKrieg.h"
#include "ZFSMManager.h"
#include "ZActorActionManager.h"
#include "ZActorAction.h"
#include "ZActorWithFSM.h"
#include "ZApplication.h"
#include <vector>

vector<string> explode(const string str, size_t nLen)
{
	string tmp;
	vector<string> res;

	for (auto it = str.begin() + nLen + 1; it != str.end(); it++)
	{
		if (*it == '_')
		{
			if (tmp.empty() == false) {
				res.push_back(tmp);
				tmp.clear();
			}
		}
		else {
			tmp += *it;
		}
	}

	if (tmp.empty() == false) {
		res.push_back(tmp);
	}

	return res;
}

ZMapSpawnData* ZRuleBlitzKrieg::DataRadar(MMatchTeam nTeamId)
{
	ZMapSpawnManager* pMap = ZGetGame()->GetWorld()->GetDesc()->GetSpawnManager();
	if (pMap)
	{
		for (int i = 0; i != pMap->GetSpawnCount(ZMST_NPC_BLITZ_RADAR); i++)
		{
			ZMapSpawnData* pData = pMap->GetSpawnData(ZMST_NPC_BLITZ_RADAR, i);
			if (pData) {
				vector<string> data = explode(pData->m_szSpawnName, strlen(MBLITZ_SPAWN_RADAR));

				if (!stricmp(data[0].c_str(), "BLUE")) {
					if (nTeamId != MMT_BLUE) {
						continue;
					}
				}
				else {
					if (nTeamId != MMT_RED) {
						continue;
					}
				}

				return pData;
			}
		}
	}
	return nullptr;
}

ZMapSpawnData* ZRuleBlitzKrieg::DataBarricade(MMatchTeam nTeamId, int nPosIndex)
{
	ZMapSpawnManager* pMap = ZGetGame()->GetWorld()->GetDesc()->GetSpawnManager();
	if (pMap)
	{
		for (int i = 0; i != pMap->GetSpawnCount(ZMST_NPC_BLITZ_BARRICADE); i++)
		{
			ZMapSpawnData* pData = pMap->GetSpawnData(ZMST_NPC_BLITZ_BARRICADE, i);
			if (pData)
			{
				vector<string> data = explode(pData->m_szSpawnName, strlen(MBLITZ_SPAWN_BARRICADE));

				if (!stricmp(data[0].c_str(), "BLUE")) {
					if (nTeamId != MMT_BLUE) {
						continue;
					}
				}
				else {
					if (nTeamId != MMT_RED) {
						continue;
					}
				}

				if (stoi(data[1]) != nPosIndex) {
					continue;
				}

				return pData;
			}
		}
	}
	return nullptr;
}

ZMapSpawnData* ZRuleBlitzKrieg::DataGuardian(MMatchTeam nTeamId)
{
	ZMapSpawnManager* pMap = ZGetGame()->GetWorld()->GetDesc()->GetSpawnManager();
	if (pMap)
	{
		for (int i = 0; i != pMap->GetSpawnCount(ZMST_NPC_BLITZ_GUARDIAN); i++)
		{
			ZMapSpawnData* pData = pMap->GetSpawnData(ZMST_NPC_BLITZ_GUARDIAN, i);
			if (pData) {
				vector<string> data = explode(pData->m_szSpawnName, strlen(MBLITZ_SPAWN_GUARDIAN));

				if (!stricmp(data[0].c_str(), "BLUE")) {
					if (nTeamId != MMT_BLUE) {
						continue;
					}
				}
				else {
					if (nTeamId != MMT_RED) {
						continue;
					}
				}

				return pData;
			}
		}
	}
	return nullptr;
}

ZRuleBlitzKrieg::ZRuleBlitzKrieg(ZMatch* pMatch) : ZRule(pMatch)
{

}

ZRuleBlitzKrieg::~ZRuleBlitzKrieg()
{

}


bool ZRuleBlitzKrieg::OnCommand(MCommand* pCommand)
{
	if (!ZGetGame()) {
		return false;
	}

	if (!pCommand) {
		return false;
	}

	int nId = pCommand->GetID();
	//if (nId >= MC_BLITZ_REQUEST_MATCH_COLLABORATOR_LIST && nId <= MC_BLITZ_REINFORCE_STATE) {	
	switch (nId)
	{
	case MC_BLITZ_NPC_SPAWN: {
		SpawnActor(pCommand);

		return true;
	}

	case MC_NEWQUEST_PEER_NPC_ACTION_EXECUTE: {
		MUID uidNpc;
		int nActionIndex;
		pCommand->GetParameter(&uidNpc, 0, MPT_UID);
		pCommand->GetParameter(&nActionIndex, 1, MPT_INT, sizeof(nActionIndex));

		if (uidNpc.IsInvalid()) {
			break;
		}

		ZActorBase* pActor = ZGetObjectManager()->GetNPCObject(uidNpc);
		if (!pActor) {
			break;
		}

		ZActorWithFSM* pActorWithFSM = MDynamicCast(ZActorWithFSM, pActor);
		if (!pActorWithFSM) {
			break;
		}

		pActorWithFSM->OnPeerActionExecute(nActionIndex);

		return true;
	}
	}
	//}
	return false;
}

void ZRuleBlitzKrieg::SpawnActor(MCommand* pCommand)
{
	MUID uidOwner;
	MUID uidActor;
	char szActorName[128];
	unsigned char nTeamID;
	unsigned char nPosIndex;
	unsigned short nHP;
	unsigned short nAP;
	float fUnknown;

	pCommand->GetParameter(&uidOwner, 0, MPT_UID);
	pCommand->GetParameter(&uidActor, 1, MPT_UID);
	pCommand->GetParameter(szActorName, 2, MPT_STR, sizeof(szActorName));
	pCommand->GetParameter(&nTeamID, 3, MPT_UCHAR);
	pCommand->GetParameter(&nPosIndex, 4, MPT_UCHAR);
	pCommand->GetParameter(&nHP, 5, MPT_USHORT);
	pCommand->GetParameter(&nAP, 6, MPT_USHORT);
	pCommand->GetParameter(&fUnknown, 7, MPT_FLOAT);

	if (uidOwner.IsInvalid()) {
		return;
	}

	rvector pos(0, 0, 0);
	rvector dir(1, 0, 0);

	MActorDef* pActorDef = MGetActorDefMgr()->GetDef(szActorName);
	if (!pActorDef)
	{
		mlog("ERROR : cannot found actordef:'%s'\n", szActorName);
		return;
	}

	ZMapSpawnData* pSpawnData = nullptr;
	switch (pActorDef->GetType()) {
	case 1:
		pSpawnData = DataRadar((MMatchTeam)nTeamID);
		break;
	case 2:
		pSpawnData = DataBarricade((MMatchTeam)nTeamID, nPosIndex);
		break;
		//case 3:
		//	pSpawnData = DataHonorItem(nPosIndex); 
		//	break;
	case 4:
		pSpawnData = DataGuardian((MMatchTeam)nTeamID);
		break;
	}

	if (pSpawnData) {
		dir = pSpawnData->m_Dir;
		pos = pSpawnData->m_Pos;
	}

	RMesh* pNPCMesh = ZGetNpcMeshMgr()->Get(pActorDef->GetModelName());
	if (pNPCMesh)
	{
		if (!pNPCMesh->m_isMeshLoaded)
		{
			ZGetNpcMeshMgr()->Load(pActorDef->GetModelName());
			ZGetNpcMeshMgr()->ReloadAllAnimation();
		}
	}

	ZActorWithFSM* pNewActor = new ZActorWithFSM(ZGetGame(), ZGetActorActionMgr());
	pNewActor->InitWithActorDef(pActorDef, ZGetFSMMgr());
	pNewActor->SetUID(uidActor);
	pNewActor->SetPosition(pos);
	pNewActor->SetDirection(dir);
	pNewActor->SetMyControl(uidOwner == ZGetMyUID());
	pNewActor->SetTeamID((MMatchTeam)nTeamID);


	ZGetObjectManager()->Add(pNewActor);
	ZGetEffectManager()->AddReBirthEffect(pNewActor->GetPosition());
}
