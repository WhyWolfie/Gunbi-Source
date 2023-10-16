#include "stdafx.h"
#include "ZRuleQuestChallenge.h"
#include "ZMatch.h"
#include "MActorDef.h"
#include "MNewQuestScenario.h"
#include "ZFSMManager.h"
#include "ZActorActionManager.h"
#include "ZActorAction.h"
#include "ZActorWithFSM.h"
#include "ZApplication.h"
#include "ZFSMManager.h"
#include "ZFSMParser.h"
#include "ZFSM.h"

ZRuleQuestChallenge::ZRuleQuestChallenge(ZMatch* pMatch) 
 : ZRule(pMatch)
 , m_pScenario(NULL)
 , m_nCurrSector(0)
{
}

ZRuleQuestChallenge::~ZRuleQuestChallenge()
{

}

bool ZRuleQuestChallenge::LoadScenarioMap(const char* szScenarioName)
{
	MNewQuestScenarioManager* pScenarioMgr = ZApplication::GetStageInterface()->GetChallengeQuestScenario();
	if (!pScenarioMgr) { _ASSERT(0); return false; }
	MNewQuestScenario* pScenario = pScenarioMgr->GetScenario(szScenarioName);
	if (!pScenario) { _ASSERT(0); return false; }

	int numSector = pScenario->GetNumSector();
	for (int i=0; i<numSector; ++i)
	{
		MNewQuestSector* pSector = pScenario->GetSector(i);
		if (!pSector)  { _ASSERT(0); return false; }

		const char* szMapName = pSector->GetMapName();
		_ASSERT(0 != strlen(szMapName));
		ZGetWorldManager()->AddWorld(szMapName);
	}

	return true;
}

int ZRuleQuestChallenge::GetRoundMax()
{
	if (!m_pScenario) {
		_ASSERT(0);
		return 1;
	}

	return m_pScenario->GetNumSector();
}

int ZRuleQuestChallenge::GetCurrRound()
{
	if (!m_pScenario)
	{
		return 1;
	}

	return m_nCurrSector;
}

bool ZRuleQuestChallenge::Init()
{
	const char* szScenarioName = ZGetGameClient()->GetMatchStageSetting()->GetMapName();
	
	MNewQuestScenarioManager* pScenarioMgr = ZApplication::GetStageInterface()->GetChallengeQuestScenario();
	if (!pScenarioMgr) {
		return false; 
	}

	MNewQuestScenario* pScenario = pScenarioMgr->GetScenario(szScenarioName);
	if (!pScenario) { 
		return false; 
	}

	m_pScenario = pScenario;
	m_nCurrSector = 0;

	return true;
}

void ZRuleQuestChallenge::OnUpdate( float fDelta )
{
	if (ZGetGame() == NULL) return;

	//MUID uidChar(ZGetMyUID());
	//static int tempUid = 111;
	//MUID uidNPC(1,tempUid);
	////unsigned char nNPCType, nPositionIndex;

	////pCommand->GetParameter(&uidChar,			0, MPT_UID);
	////pCommand->GetParameter(&uidNPC,				1, MPT_UID);
	////pCommand->GetParameter(&nNPCType,			2, MPT_UCHAR);
	////pCommand->GetParameter(&nPositionIndex,		3, MPT_UCHAR);

	//// �ϴ� ���� ���� �����Ѵ�
	//ZNPCObjectMap* pNpcMap = ZGetObjectManager()->GetNPCObjectMap();
	//for (ZNPCObjectMap::iterator it=pNpcMap->begin(); it!=pNpcMap->end(); )
	//{
	//	if (it->second->IsDie())
	//	{
	//		delete it->second;
	//		it = pNpcMap->erase(it);
	//	}
	//	else
	//		++it;
	//}

	//// ���� ���� ���� �����Ѵ�
	//if (5 <= pNpcMap->size())
	//	return;

	//tempUid++;
	//uidNPC = MUID(1, tempUid);

	///*
	//MQUEST_NPC NPCType = NPC_GOBLIN_GUNNER;

	////ZMapSpawnType nSpawnType = ZMST_NPC_MELEE;

	//ZMapSpawnManager* pMSM = ZGetGame()->GetMapDesc()->GetSpawnManager();

	//MQuestNPCInfo* pNPCInfo = ZGetQuest()->GetNPCCatalogue()->GetInfo(NPCType);
	////MQuestNPCInfo* pNPCInfo = GetNPCInfo(NPCType);
	//if (pNPCInfo == NULL) return;

	//ZMapSpawnData* pSpawnData = pMSM->GetSpawnData(ZMST_NPC_MELEE, 0);

	//rvector NPCPos = rvector(0,0,0);
	//rvector NPCDir = rvector(1,0,0);

	//if (pSpawnData)
	//{
	//	NPCPos = pSpawnData->m_Pos;
	//	NPCDir = pSpawnData->m_Dir;
	//}*/

	//bool bMyControl = (uidChar == ZGetMyUID());
	//SpawnActor("knifeman", uidNPC, bMyControl);


	//todokkkkk ���� ���Ǿ� ������ �˸�
}

void ZRuleQuestChallenge::OnSetRoundState( MMATCH_ROUNDSTATE roundState )
{
	switch(roundState) 
	{
	case MMATCH_ROUNDSTATE_PREPARE: 
		{
		}
		break;
	case MMATCH_ROUNDSTATE_PRE_COUNTDOWN:
		{
		}
		break;
	case MMATCH_ROUNDSTATE_COUNTDOWN : 
		{
		}
		break;
	case MMATCH_ROUNDSTATE_PLAY:
		{
		}
		break;
	case MMATCH_ROUNDSTATE_FINISH:
		{
		}
		break;
	case MMATCH_ROUNDSTATE_EXIT:
		{
		}
		break;
	case MMATCH_ROUNDSTATE_FREE:
		{
		}
		break;
	};
}

bool ZRuleQuestChallenge::OnCommand( MCommand* pCommand )
{
	switch (pCommand->GetID())
	{
	case MC_NEWQUEST_NPC_SPAWN:
		{
			MUID uidController;
			MUID uidNpc;
			char szActorDefName[128];
			unsigned char nCustomSpawnTypeIndex, nSpawnIndex;
			pCommand->GetParameter(&uidController,	0, MPT_UID);
			pCommand->GetParameter(&uidNpc,			1, MPT_UID);
			pCommand->GetParameter(szActorDefName,	2, MPT_STR, sizeof(szActorDefName));
			pCommand->GetParameter(&nCustomSpawnTypeIndex,	3, MPT_UCHAR);
			pCommand->GetParameter(&nSpawnIndex,			4, MPT_UCHAR);

			SpawnActor(szActorDefName, uidNpc, nCustomSpawnTypeIndex, nSpawnIndex, ZGetMyUID()==uidController);
			return true;
		}

	case MC_NEWQUEST_NPC_DEAD:
		{
			MUID uidKiller, uidNPC;

			pCommand->GetParameter(&uidKiller,	0, MPT_UID);
			pCommand->GetParameter(&uidNPC,		1, MPT_UID);

			ZActorWithFSM* pActor = (ZActorWithFSM*)ZGetObjectManager()->GetNPCObject(uidNPC);
			if (pActor)
			{
				ZGetObjectManager()->Delete(pActor);

				//todok �Ʒ� �ּ� ��������� ��
				/*m_GameInfo.IncreaseNPCKilled();

				ZCharacter* pCharacter = (ZCharacter*) ZGetCharacterManager()->Find(uidKiller);
				if (pCharacter)
				{
					ZModule_QuestStatus* pMod = (ZModule_QuestStatus*)pCharacter->GetModule(ZMID_QUESTSTATUS);
					if (pMod)
					{
						pMod->AddKills();
					}
				}*/
			}
			return true;
		}

	case MC_NEWQUEST_MOVE_TO_NEXT_SECTOR:
		{
			MoveToNextSector();
			return true;
		}

	case MC_NEWQUEST_PEER_NPC_BASICINFO:
		{
			MCommandParameter* pParam = pCommand->GetParameter(0);
			if(pParam->GetType()!=MPT_BLOB)
			{
				_ASSERT(0); break;
			}

			ZACTOR_WITHFSM_BASICINFO* pbi = (ZACTOR_WITHFSM_BASICINFO*)pParam->GetPointer();
			ZActorWithFSM* pActor = (ZActorWithFSM*)ZGetObjectManager()->GetNPCObject(pbi->uidNPC);
			if (pActor)
			{
				pActor->OnBasicInfo(pbi);
			}

			return true;
		}

	case MC_NEWQUEST_PEER_NPC_ACTION_EXECUTE:
		{
			MUID uidNpc;
			int nActionIndex;
			pCommand->GetParameter(&uidNpc, 0, MPT_UID);
			pCommand->GetParameter(&nActionIndex, 1, MPT_INT, sizeof(nActionIndex));

			ZActorBase* pActor = ZGetObjectManager()->GetNPCObject(uidNpc);
			ZActorWithFSM* pActorWithFSM = MDynamicCast(ZActorWithFSM, pActor);
			if (pActorWithFSM)
			{
				pActorWithFSM->OnPeerActionExecute(nActionIndex);
			}

			return true;
		}
	}

	return false;
}

void ZRuleQuestChallenge::SpawnActor(const char* szActorDefName, MUID& uid, int nCustomSpawnTypeIndex, int nSpawnIndex, bool bMyControl)
{
	rvector pos(0,0,0);
	rvector dir(1,0,0);

	ZMapSpawnManager* pMSM = ZGetGame()->GetMapDesc()->GetSpawnManager();
	if (pMSM)
	{
		ZMapSpawnData* pSpawnData = pMSM->GetCustomSpawnData(nCustomSpawnTypeIndex, nSpawnIndex);
		if (pSpawnData)
		{
			pos = pSpawnData->m_Pos;
			dir = pSpawnData->m_Dir;
		}
	}

	MActorDef* pActorDef = MGetActorDefMgr()->GetDef(szActorDefName);
	if (!pActorDef) 
	{
		mlog("ERROR : cannot found actordef:\'%s\'\n", szActorDefName);
		_ASSERT(0); 
		return; 
	}

	// ���� ���ҽ� �ε��� �������� �ε� - �̷����� �׽�Ʈ���� ������Ѵ�.
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
	pNewActor->SetUID(uid);
	pNewActor->SetPosition(pos);
	pNewActor->SetDirection(dir);
	pNewActor->SetMyControl(bMyControl);
	pNewActor->SetTeamID(MMT_BLUE);

	ZGetObjectManager()->Add(pNewActor);
	ZGetEffectManager()->AddReBirthEffect(pNewActor->GetPosition());
}

void ZRuleQuestChallenge::MoveToNextSector()
{
	//FADE OUT HERE

	ZCharacter *pMyChar = ZGetGame()->m_pMyCharacter;
	pMyChar->InitStatus();

	// �� ȭ�鿡�� �������� �� �ִ� ź�� ����Ʈ�� ����
	ZGetEffectManager()->Clear();
	ZGetGame()->m_WeaponManager.Clear();

	// ���ο� ����� �̵�!!
	++m_nCurrSector;	//todok ���� ���Ͱ� ������ ���Ͷ��? �������ǰ�..
	ZGetWorldManager()->SetCurrent(m_nCurrSector);
	//todok ���� ���·� ���� �����߱� ������ �� �ڵ�� ���� �ʿ� ���µ�
	//// �̹��� �̵��� ĳ������ ��ġ
	//int nPosIndex = ZGetCharacterManager()->GetCharacterIndex(pMyChar->GetUID(), false);
	//if (nPosIndex < 0) nPosIndex=0;
	//ZMapSpawnData* pSpawnData = ZGetWorld()->GetDesc()->GetSpawnManager()->GetSoloData(nPosIndex);
	//// �� ��ǥ�� �̵�
	//if (pSpawnData!=NULL && pMyChar!=NULL)
	//{
	//	pMyChar->SetPosition(pSpawnData->m_Pos);
	//	pMyChar->SetDirection(pSpawnData->m_Dir);
	//	ZGetEffectManager()->AddReBirthEffect(pSpawnData->m_Pos);
	//}

	//todok quest �ӽ÷� �ּ��� �����ؾ� �Ѵ�
	//// �ƹ��� �������� �ʴ´�.
	//for(ZCharacterManager::iterator i = ZGetCharacterManager()->begin();i!=ZGetCharacterManager()->end();i++)
	//{
	//	i->second->SetVisible(false);
	//}

	//// ko�� ����ȭ
	//ZModule_QuestStatus* pMod = (ZModule_QuestStatus*)pMyChar->GetModule(ZMID_QUESTSTATUS);
	//if (pMod)
	//{
	//	int nKills = pMod->GetKills();
	//	ZGetScreenEffectManager()->SetKO(nKills);
	//}
}