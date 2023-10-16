#include "stdafx.h"
#include "MWarManager.h"

MWarManager::MWarManager() {
	ulLastWar = 0;
	m_UID = MUID(0, 0);
}

MWarManager* MWarManager::GetInstance()
{
	static MWarManager pInstance;
	return &pInstance;
}

bool MWarManager::ReadXml(const char* szFileName) {

	MXmlDocument xmlDoc;
	xmlDoc.Create();

	if (xmlDoc.LoadFromFile(szFileName) == false) {
		xmlDoc.Destroy();
		return false;
	}

	char szTagName[256];

	MXmlElement rootElement = xmlDoc.GetDocumentElement();
	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++) {

		MXmlElement chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);

		if (szTagName[0] == '#') continue;

		if (!stricmp(szTagName, "EVENT")) {
			char szAttrValue[256];
			char szAttrName[64];
			char szTagName[128];

			int nAttrCount = chrElement.GetAttributeCount();
			for (int i = 0; i < nAttrCount; i++)
			{
				chrElement.GetAttribute(i, szAttrName, szAttrValue);
				if (!_stricmp(szAttrName, "INTERVAL")) {
					pEvent.nInterval = (atoi(szAttrValue) * 1000);
				}

				else if (!_stricmp(szAttrName, "TIME")) {
					pEvent.nTime = atoi(szAttrValue);
				}

				else if (!_stricmp(szAttrName, "PREPARE")) {
					pEvent.nPrepare = (atoi(szAttrValue) * 1000);
				}

				else if (!_stricmp(szAttrName, "MAP")) {
					pEvent.strMap = szAttrValue;
				}
			}

			int iChildCount = chrElement.GetChildNodeCount();
			for (int i = 0; i < iChildCount; i++)
			{
				MXmlElement element = chrElement.GetChildNode(i);
				element.GetTagName(szTagName);
				if (szTagName[0] == '#') {
					continue;
				}

				if (!_stricmp(szTagName, "NPC"))
				{
					MWarNPC pTemp;
					int nAttrCount = element.GetAttributeCount();
					for (int i = 0; i < nAttrCount; i++)
					{
						element.GetAttribute(i, szAttrName, szAttrValue);
						if (!_stricmp(szAttrName, "MODEL")) {
							pTemp.strModel = szAttrValue;
						}

						else if (!_stricmp(szAttrName, "HP")) {
							pTemp.nHP = atoi(szAttrValue);
						}

						else if (!_stricmp(szAttrName, "AP")) {
							pTemp.nAP = atoi(szAttrValue);
						}

						else if (!_stricmp(szAttrName, "POS")) {
							int nCnt = sscanf(szAttrValue, "%f %f %f", &pTemp.fPosX, &pTemp.fPosY, &pTemp.fPosZ);
							if (nCnt != 3) {
								return false;
							}
						}

						else if (!_stricmp(szAttrName, "DIR")) {
							int nCnt = sscanf(szAttrValue, "%f %f %f", &pTemp.fDirX, &pTemp.fDirY, &pTemp.fDirZ);
							if (nCnt != 3) {
								return false;
							}
						}
					}
					pNpc.push_back(pTemp);
				}
				
				if (!_stricmp(szTagName, "REWARD")) 
				{
					MWarReward pRewardTemp;
					int nAttrCount = element.GetAttributeCount();
					for (int i = 0; i < nAttrCount; i++)
					{
						element.GetAttribute(i, szAttrName, szAttrValue);
						if (!_stricmp(szAttrName, "TYPE")) {
							pRewardTemp.nType = atoi(szAttrValue);
						}

						else if (!_stricmp(szAttrName, "XP")) {
							pRewardTemp.nXp = atoi(szAttrValue);
						}

						else if (!_stricmp(szAttrName, "BP")) {
							pRewardTemp.nBp = atoi(szAttrValue);
						}

						else if (!_stricmp(szAttrName, "CASH")) {
							pRewardTemp.nCash = atoi(szAttrValue);
						}
					}
					pReward.push_back(pRewardTemp);
				}
			}
		}
	}
	xmlDoc.Destroy();

	return true;
}

void MWarManager::Announce(const char* szMsg, ZAdminAnnounceType nMode, MUID uidChar) {
	MCommand* pCmd = MMatchServer::GetInstance()->CreateCommand(MC_ADMIN_ANNOUNCE, MUID(0, 0));
	if (pCmd) {
		pCmd->AddParameter(new MCmdParamUID(MUID(0, 0)));
		pCmd->AddParameter(new MCmdParamStr(szMsg));
		pCmd->AddParameter(new MCmdParamUInt(nMode));

		if (uidChar.IsInvalid()) {
			MMatchServer::GetInstance()->RouteToAllClient(pCmd);
		}
		else {
			MObject* pObj = MMatchServer::GetInstance()->GetObjectA(uidChar);
			if (pObj) {
				MMatchServer::GetInstance()->RouteToListener(pObj, pCmd);
			}
		}
	}
}

bool MWarManager::Start() {

	if (ulLastWar == 0 || (timeGetTime() - ulLastWar) > pEvent.nInterval) {

		ulLastWar = timeGetTime();

		MMatchServer* pServer = MMatchServer::GetInstance();
		if (!pServer) {
			return false;
		}
	
		bool bRet = pServer->LaunchWarMatch(pEvent);
		if (bRet) {
			Announce("Enter the war battle now!");
		}
		return bRet;
	} 
	return false;
}