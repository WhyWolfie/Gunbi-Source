#include "stdafx.h"
#include "ZDummyNPC.h"
#include "ZRuleWarMode.h"

ZRuleWarMode::ZRuleWarMode(ZMatch* pMatch) : ZRuleTeamDeathMatch2(pMatch)
{

}

ZRuleWarMode::~ZRuleWarMode()
{

}

bool ZRuleWarMode::OnCommand(MCommand* pCommand)
{
	if (!ZGetGame()) {
		return false;
	}

	switch (pCommand->GetID())
	{
		case MC_MATCH_GAME_DEAD:
			{
				MUID uidAttacker, uidVictim;

				pCommand->GetParameter(&uidAttacker, 0, MPT_UID);
				pCommand->GetParameter(&uidVictim, 2, MPT_UID);

				ZCharacter* pAttacker = (ZCharacter*)ZGetCharacterManager()->Find(uidAttacker);
				ZCharacter* pVictim = (ZCharacter*)ZGetCharacterManager()->Find(uidVictim);

				if (pAttacker == nullptr || pVictim == nullptr) {
					break;
				}
			}
			break;

		case MC_WAR_DUMMY_BASICINFO:
			{
				MCommandParameter* pParam = pCommand->GetParameter(0);
				if (pParam->GetType() != MPT_BLOB) {
					break;
				}

				ZDUMMY_BASICINFO* pbi = (ZDUMMY_BASICINFO*)pParam->GetPointer(); 
					if (pbi == nullptr) {
						break;
					}

				ZDummyNPC* pDummy = (ZDummyNPC*)ZGetObjectManager()->GetNPCObject(pbi->uidNPC);
				if (pDummy) {
					pDummy->OnBasicInfo(pbi);
				}
			}
			break;

		case MC_WAR_SPAWN_TOWER:
			{
				MUID uidOwner;
				MUID uidActor;
				char szModelName[128];
				unsigned char nTeamID;
				unsigned short nHP;
				unsigned short nAP;
				rvector nPos;
				rvector nDir;

				pCommand->GetParameter(&uidOwner, 0, MPT_UID);
				pCommand->GetParameter(szModelName, 1, MPT_STR, sizeof(szModelName));
				pCommand->GetParameter(&uidActor, 2, MPT_UID);
				pCommand->GetParameter(&nTeamID, 3, MPT_UCHAR);
				pCommand->GetParameter(&nHP, 4, MPT_USHORT);
				pCommand->GetParameter(&nAP, 5, MPT_USHORT);
				pCommand->GetParameter(&nPos, 6, MPT_POS);
				pCommand->GetParameter(&nDir, 7, MPT_DIR);

				if (uidOwner.IsInvalid()) {
					break;
				}

				RMesh* pNPCMesh = ZGetNpcMeshMgr()->Get(szModelName);
				if (pNPCMesh)
				{
					if (!pNPCMesh->m_isMeshLoaded)
					{
						ZGetNpcMeshMgr()->Load(szModelName);
						ZGetNpcMeshMgr()->ReloadAllAnimation();
					}

					ZDummyNPC* pObject = new ZDummyNPC(ZGetGame());
					if (pObject) {

						ZDUMMY_INFO* p = new ZDUMMY_INFO();
						p->fHP = nHP;
						p->fAP = nAP;
						p->fCollHeight = 400;
						p->fCollRadius = 120;
						p->fScale = 0.7f;

						p->posx = nPos.x;
						p->posy = nPos.y;
						p->posz = nPos.z;

						p->dirx = nDir.x;
						p->diry = nDir.y;
						p->dirz = nDir.z;

						pObject->Init(szModelName, p);
						pObject->SetUID(uidActor);
						pObject->SetTeamID((MMatchTeam)nTeamID);
						pObject->SetMyControl(uidOwner == ZGetMyUID());						
					}

					m_vTowers.insert(pair<MMatchTeam, MUID>((MMatchTeam)nTeamID, uidActor));

					ZGetObjectManager()->Add(pObject);
				}
			}
			break;
	}

	return false;
}

ZDummyNPC* ZRuleWarMode::GetTower(MMatchTeam nTeamId) {

	auto itor = m_vTowers.find(nTeamId);
	if (itor != m_vTowers.end()) {

		ZActorBase* pObj = ZGetObjectManager()->GetNPCObject((*itor).second);
		if (pObj) {
			return MDynamicCast(ZDummyNPC, pObj);
		}
	}
	return nullptr;
}