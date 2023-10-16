#include "stdafx.h"
#include "MMatchBlitzKrieg.h"

MMatchBlitzKrieg::MMatchBlitzKrieg()
{

}

MMatchBlitzKrieg::~MMatchBlitzKrieg()
{

}

bool MMatchBlitzKrieg::ReadXml(const char* szFileName, MZFileSystem* pFileSystem)
{
	MXmlDocument xmlIniData;
	xmlIniData.Create();

	char *buffer;
	MZFile mzf;

	if (pFileSystem)
	{
		if (!mzf.Open(szFileName, pFileSystem))
		{
			if (!mzf.Open(szFileName))
			{
				xmlIniData.Destroy();
				return false;
			}
		}
	}
	else
	{
		if (!mzf.Open(szFileName))
		{
			xmlIniData.Destroy();
			return false;
		}
	}

	buffer = new char[mzf.GetLength() + 1];
	buffer[mzf.GetLength()] = 0;
	mzf.Read(buffer, mzf.GetLength());

	if (!xmlIniData.LoadFromMemory(buffer))
	{
		delete[] buffer;
		xmlIniData.Destroy();
		return false;
	}

	delete[] buffer;
	mzf.Close();

	MXmlElement rootElement, chrElement, attrElement;
	char szTagName[256];
	char szAttrName[64];
	char szAttrValue[64];

	rootElement = xmlIniData.GetDocumentElement();

	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++)
	{
		chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		if (!stricmp(szTagName, "EVENT"))
		{
			LoadBlitzEvent(chrElement);
		}
		else if (!stricmp(szTagName, "REWARD"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			RewardStruct* pRewards = new RewardStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "baseBounty"))
				{
					pRewards->nbaseBounty = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "baseExp"))
				{
					pRewards->nbaseExp = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minTime"))
				{
					pRewards->nminTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minHonor"))
				{
					pRewards->nminHonor = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "winnerMedal"))
				{
					pRewards->nwinnerMedal = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "loserMedal"))
				{
					pRewards->nloserMedal = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minuteBonusMedal"))
				{
					pRewards->nminuteBonusMedal = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minuteBonusMedalMax"))
				{
					pRewards->nminuteBonusMedalMax = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minPlayCount"))
				{
					pRewards->nminPlayCount = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "minPlayCountBonusMedal"))
				{
					pRewards->nminPlayCountBonusMedal = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WaitingTime"))
				{
					pRewards->nWaitingTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WaitingRewardMedal"))
				{
					pRewards->nWaitingRewardMedal = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WaitingRewardMax"))
				{
					pRewards->nWaitingRewardMax = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WinXPBonusMVP"))
				{
					pRewards->fWinXPBonusMVP = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WinBPBonusMVP"))
				{
					pRewards->fWinBPBonusMVP = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "WinMedalBonusMVP"))
				{
					pRewards->fWinMedalBonusMVP = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "LoseXPBonusMVP"))
				{
					pRewards->fLoseXPBonusMVP = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "LoseBPBonusMVP"))
				{
					pRewards->fLoseBPBonusMVP = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "LoseMedalBonusMVP"))
				{
					pRewards->fLoseMedalBonusMVP = atof(szAttrValue);
				}
			}
			m_pRewards.push_back(move(pRewards));
		}
		else if (!stricmp(szTagName, "PENALTY"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			PenaltyStruct* pPenalty = new PenaltyStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "first"))
				{
					pPenalty->nfirst = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "second"))
				{
					pPenalty->nsecond = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "afterThird"))
				{
					pPenalty->nafterThird = atoi(szAttrValue);
				}
			}
			m_pPenalty.push_back(move(pPenalty));
		}
		else if (!stricmp(szTagName, "CLASS_TABLE"))
		{
			LoadBlitzClassTable(chrElement);
		}
		else if (!stricmp(szTagName, "CLASS_BOOK"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			ClassBookStruct* pClassBook = new ClassBookStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "gladiator"))
				{
					pClassBook->nGladiatorID = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "duelist"))
				{
					pClassBook->nDuelistID = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "incinerator"))
				{
					pClassBook->nIncineratorID = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "combatofficer"))
				{
					pClassBook->nCombatOfficerID = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "assassin"))
				{
					pClassBook->nAssassinID = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "terrorist"))
				{
					pClassBook->nTerroristID = atoi(szAttrValue);
				}
			}
			m_pClassBook.insert(std::map<int, ClassBookStruct*>::value_type(pClassBook->nGladiatorID, pClassBook));
		}
		else if (!stricmp(szTagName, "UPGRADE"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			UpgradeStruct* pUpgrade = new UpgradeStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "requireHonor"))
				{
					sscanf(szAttrValue, "%d:%d:%d:%d", &pUpgrade->nrequireHonor[0], &pUpgrade->nrequireHonor[1],
						&pUpgrade->nrequireHonor[2], &pUpgrade->nrequireHonor[3]);
				}
				else if (!stricmp(szAttrName, "dps"))
				{
					sscanf(szAttrValue, "%d:%d:%d:%d", &pUpgrade->ndps[0], &pUpgrade->ndps[1], &pUpgrade->ndps[2],
						&pUpgrade->ndps[3]);
				}
				else if (!stricmp(szAttrName, "reduceShotDelayRatio"))
				{
					sscanf(szAttrValue, "%f:%f:%f:%f", &pUpgrade->freduceShotDelayRatio[0], &pUpgrade->freduceShotDelayRatio[1],
						&pUpgrade->freduceShotDelayRatio[2], &pUpgrade->freduceShotDelayRatio[3]);
				}
				else if (!stricmp(szAttrName, "hpAp"))
				{
					sscanf(szAttrValue, "%d:%d:%d:%d", &pUpgrade->nhpAp[0], &pUpgrade->nhpAp[1], &pUpgrade->nhpAp[2],
						&pUpgrade->nhpAp[3]);
				}
				else if (!stricmp(szAttrName, "enchantFire"))
				{
					sscanf(szAttrValue, "%d:%d:%d:%d", &pUpgrade->nenchantFire[0], &pUpgrade->nenchantFire[1],
						&pUpgrade->nenchantFire[2], &pUpgrade->nenchantFire[3]);
				}
				else if (!stricmp(szAttrName, "fireDamageDuration"))
				{
					pUpgrade->ffireDamageDuration = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "magazineRatio"))
				{
					sscanf(szAttrValue, "%f:%f:%f:%f", &pUpgrade->fmagazineRatio[0], &pUpgrade->fmagazineRatio[1],
						&pUpgrade->fmagazineRatio[2], &pUpgrade->fmagazineRatio[3]);
				}
				else if (!stricmp(szAttrName, "reviveRatio"))
				{
					sscanf(szAttrValue, "%f:%f:%f:%f", &pUpgrade->freviveRatio[0], &pUpgrade->freviveRatio[1],
						&pUpgrade->freviveRatio[2], &pUpgrade->freviveRatio[3]);
				}
			}
			m_pUpgradeStruct.push_back(move(pUpgrade));
		}
		else if (!stricmp(szTagName, "WEAPON"))
		{
			LoadWeaponInfo(chrElement);
		}
		else if (!stricmp(szTagName, "SPAWN_LIST"))
		{
			LoadSpawnList(chrElement);
		}
		else if (!stricmp(szTagName, "HONOR_LIST"))
		{
			LoadHonorList(chrElement);
		}
		else if (!stricmp(szTagName, "ROUTE_LIST"))
		{
			LoadRouteList(chrElement);
		}
		else if (!stricmp(szTagName, "EVENT_MESSAGE"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			EventMsgStruct* pEventMsg = new EventMsgStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "viewTime"))
				{
					pEventMsg->nviewTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "delayTime"))
				{
					pEventMsg->fdelayTime = atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "damagedRadarCoolDown"))
				{
					pEventMsg->ndamagedRadarCoolDown = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "sound_Benefit"))
				{
					pEventMsg->strsound_Benefit = szAttrValue;
				}
				else if (!stricmp(szAttrName, "sound_Loss"))
				{
					pEventMsg->strsound_Loss = szAttrValue;
				}
			}
			m_pEventMsg.push_back(move(pEventMsg));
		}
		else if (!stricmp(szTagName, "HELP_MESSAGE"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			HelpMsgStruct* pEventMsg = new HelpMsgStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "viewTime"))
				{
					pEventMsg->nviewTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "dist"))
				{
					pEventMsg->ndist = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "honor"))
				{
					pEventMsg->nhonor = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "sound"))
				{
					pEventMsg->strsound = szAttrValue;
				}
			}
			m_pHelpMsg.push_back(move(pEventMsg));
		}
	}

	xmlIniData.Destroy();
	return true;
}

MMatchBlitzKrieg* MMatchBlitzKrieg::GetInstance()
{
	static MMatchBlitzKrieg pBlitz;
	return &pBlitz;
}

void MMatchBlitzKrieg::LoadBlitzEvent(MXmlElement& Element)
{

	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement, childElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		if (!stricmp(szTagName, "HONOR"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			HonorStruct* pHonor = new HonorStruct();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "startHonor"))
				{
					pHonor->nStartHonor = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "autoIncHonorSec"))
				{
					pHonor->nautoIncHonorSec = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "autoIncHonor"))
				{
					pHonor->nautoIncHonor = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "firstKillHonor"))
				{
					pHonor->nfirstKillHonor = atoi(szAttrValue);
				}
			}
			m_pHonorStruct.push_back(move(pHonor));
			LoadBlitzHonorSubTag(chrElement);
		}
		else if (!stricmp(szTagName, "RESPAWN"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			RespawnStruct* pRespawn = new RespawnStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "baseTime"))
				{
					pRespawn->nbaseTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "invincibleTime"))
				{
					pRespawn->ninvincibleTime = atoi(szAttrValue);
				}
			}
			m_pRespawnStruct.push_back(move(pRespawn));
		}
		else if (!stricmp(szTagName, "CLASS_SELECT_TIME"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			ClassSelectStruct* pClassSelect = new ClassSelectStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "ClassSelectTime"))
				{
					pClassSelect->nClassSelectTime = atoi(szAttrValue);
				}
			}
			m_pClass = pClassSelect;
		}
		else if (!stricmp(szTagName, "FINISH_DELAY_TIME"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			FinishDelayTimeStruct* pTimeDelay = new FinishDelayTimeStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "FinishDelayTime"))
				{
					pTimeDelay->nFinishDelayTime = atoi(szAttrValue);
				}
			}
			m_pFinish = pTimeDelay;
		}
		else if (!stricmp(szTagName, "ENHANCE_PLAYER"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			EnhancePlayerStruct* pEnhancePlayer = new EnhancePlayerStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "apHp"))
				{
					pEnhancePlayer->napHp = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "dps"))
				{
					pEnhancePlayer->ndps = atoi(szAttrValue);
				}
			}
			m_pEnhancePlayer.push_back(move(pEnhancePlayer));
		}
		else if (!stricmp(szTagName, "ENHANCE_NPC"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			EnhanceNPCStruct* pEnhanceNPC = new EnhanceNPCStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "nextTime"))
				{
					pEnhanceNPC->nNextTime = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "maxCount"))
				{
					pEnhanceNPC->nMaxCount = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "enhancedRatio"))
				{
					pEnhanceNPC->fEnhancedRatio = atof(szAttrValue);
				}
			}
			m_pEnhanceNPC.push_back(move(pEnhanceNPC));
		}
		else if (!stricmp(szTagName, "BUILDING"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			BuildingStruct* pBuilding = new BuildingStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "reduceDamageRatioFromPlayer"))
				{
					pBuilding->freduceDamageRatioFromPlayer = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "checkDelay"))
				{
					pBuilding->fcheckDelay = (float)atof(szAttrValue);
				}
			}
			m_pBuilding.push_back(move(pBuilding));
			LoadBuildingInfo(chrElement);
		}
		else if (!stricmp(szTagName, "HONOR_ITEM_LIST"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			HonorItemListStruct* pHonorItemList = new HonorItemListStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "respawnTime"))
				{
					pHonorItemList->nrespawnTime = atoi(szAttrValue);
				}

			}
			m_pItemList.insert(std::map<string, HonorItemListStruct*>::value_type(pHonorItemList->strHonorItemActor, pHonorItemList));
			LoadHonorItemSubTag(chrElement);
		}
		else if (!stricmp(szTagName, "REINFORCE_LIST"))
		{
			LoadReinforceList(chrElement);
		}
	}
}

void MMatchBlitzKrieg::LoadBlitzHonorSubTag(MXmlElement& Element)
{

	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		HonorStruct* pLeaveHonorStruct = new HonorStruct();

		if (!stricmp(szTagName, "LEAVE_AUTO_INC_HONOR"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "autoIncHonor"))
				{
					pLeaveHonorStruct->nautoIncHonorSub = atoi(szAttrValue);
				}
			}
		}
		m_pHonorStruct.push_back(move(pLeaveHonorStruct));
	}
}

void MMatchBlitzKrieg::LoadBlitzClassTable(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		ClassTableStruct* pClassTable = new ClassTableStruct();

		if (!stricmp(szTagName, "HUNTER"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "aquirHonorRatio"))
				{
					pClassTable->faquirHonorRatio = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "SLAUGHTER"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int k = 0; k < nAttrCount; ++k)
			{
				chrElement.GetAttribute(k, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "enchantFireDamage"))
				{
					pClassTable->nenchantFireDamage = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "fireDamageDuration"))
				{
					pClassTable->ffireDamageDuration = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "addMagazineRatio"))
				{
					pClassTable->faddMagazineRatio = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "TRICKSTER"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int l = 0; l < nAttrCount; ++l)
			{
				chrElement.GetAttribute(l, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "recoveryApHpRatio"))
				{
					pClassTable->frecoveryApHpRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "recoveryMagazineRatio"))
				{
					pClassTable->frecoveryMagazineRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "reduceDamageRatio"))
				{
					pClassTable->freduceDamageRatio = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "GLADIATOR"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int m = 0; m < nAttrCount; ++m)
			{
				chrElement.GetAttribute(m, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "enhanceMeleeDPS"))
				{
					pClassTable->nenhanceMeleeDPS = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "addMaxApHp"))
				{
					pClassTable->naddMaxApHp = atoi(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "DUELIST"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int n = 0; n < nAttrCount; ++n)
			{
				chrElement.GetAttribute(n, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "addShotgunMagazine"))
				{
					pClassTable->naddShotgunMagazine = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "enhanceShotgunDamage"))
				{
					pClassTable->nenhanceShotgunDamage = atoi(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "INCINERATOR"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int o = 0; o < nAttrCount; ++o)
			{
				chrElement.GetAttribute(o, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "enchantFireDamage"))
				{
					pClassTable->nenchantFireDamageIncinerator = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "fireDamageDuration"))
				{
					pClassTable->ffireDamageDurationIncinerator = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "reduceDPS"))
				{
					pClassTable->nreduceDPS = atoi(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "COMBATOFFICER"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int p = 0; p < nAttrCount; ++p)
			{
				chrElement.GetAttribute(p, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "reduceDamageRatioForMyTeam"))
				{
					pClassTable->freduceDamageRatioForMyTeam = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "distance"))
				{
					pClassTable->ndistance = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "checkDelay"))
				{
					pClassTable->fcheckDelay = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "ASSASSIN"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int q = 0; q < nAttrCount; ++q)
			{
				chrElement.GetAttribute(q, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "enhanceDamageRatio"))
				{
					pClassTable->fenhanceDamageRatio = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "TERRORIST"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int r = 0; r < nAttrCount; ++r)
			{
				chrElement.GetAttribute(r, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "enhanceDamageRatioAtBuilding"))
				{
					pClassTable->fenhanceDamageRatioAtBuilding = (float)atof(szAttrValue);
				}
			}
		}
		m_pClassTable.push_back(move(pClassTable));
	}
}

void MMatchBlitzKrieg::LoadSpawnList(MXmlElement& Element)
{

	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!stricmp(szTagName, "SPAWN"))
		{
			SpawnListStruct* pSpawnList = new SpawnListStruct();
			int nAttrCount = chrElement.GetAttributeCount();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "actor"))
				{
					pSpawnList->strActor = szAttrValue;
				}
				else if (!stricmp(szAttrName, "num"))
				{
					pSpawnList->nNumActor = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "team"))
				{
					pSpawnList->nTeamID = atoi(szAttrValue);
				}
			}
			m_pSpawnList.insert(map<string, SpawnListStruct*>::value_type(pSpawnList->strActor, pSpawnList));
		}
	}
}

void MMatchBlitzKrieg::LoadReinforceList(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		ReinforceListStruct* pReinforceList = new ReinforceListStruct();
		int nAttrCount = chrElement.GetAttributeCount();

		for (int j = 0; j < nAttrCount; ++j)
		{
			chrElement.GetAttribute(j, szAttrName, szAttrValue);

			if (!stricmp(szAttrName, "barricadeCount"))
			{
				pReinforceList->nbarricadeCount = atoi(szAttrValue);
			}
			else if (!stricmp(szAttrName, "state"))
			{
				pReinforceList->strReinforceState = szAttrValue;
			}
			else if (!stricmp(szAttrName, "alliance"))
			{
				if (!stricmp(szAttrValue, "true")) pReinforceList->balliance = true;
				else pReinforceList->balliance = false;
			}
		}
		m_pReinforceList.push_back(move(pReinforceList));
	}
}

void MMatchBlitzKrieg::LoadWeaponInfo(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		WeaponStruct* pWeaponStruct = new WeaponStruct();
		int nAttrCount = chrElement.GetAttributeCount();
		if (!stricmp(szTagName, "DPS"))
		{
			for (int j = 0; j < nAttrCount; ++j)
			{

				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "smg"))
				{
					pWeaponStruct->fsmg = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "rifle"))
				{
					pWeaponStruct->fRifle = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "machineGun"))
				{
					pWeaponStruct->fMachineGun = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "pistol"))
				{
					pWeaponStruct->fpistol = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "revolver"))
				{
					pWeaponStruct->frevolver = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "shotGun"))
				{
					pWeaponStruct->fshotGun = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "roket"))
				{
					pWeaponStruct->froket = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "dagger"))
				{
					pWeaponStruct->fdagger = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "katana"))
				{
					pWeaponStruct->fkatana = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "doubleKatana"))
				{
					pWeaponStruct->fdoubleKatana = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "DELAY"))
		{
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "smg"))
				{
					pWeaponStruct->nsmg = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "rifle"))
				{
					pWeaponStruct->nRifle = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "machineGun"))
				{
					pWeaponStruct->nMachineGun = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "pistol"))
				{
					pWeaponStruct->npistol = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "revolver"))
				{
					pWeaponStruct->nrevolver = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "shotGun"))
				{
					pWeaponStruct->nshotGun = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "roket"))
				{
					pWeaponStruct->nroket = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "dagger"))
				{
					pWeaponStruct->ndagger = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "katana"))
				{
					pWeaponStruct->nkatana = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "doubleKatana"))
				{
					pWeaponStruct->ndoubleKatana = atoi(szAttrValue);
				}
			}
		}
		m_pWeaponStruct.push_back(move(pWeaponStruct));
	}
}

void MMatchBlitzKrieg::LoadHonorList(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		HonorListStruct* pHonorStruct = new HonorListStruct();
		int nAttrCount = chrElement.GetAttributeCount();
		if (!stricmp(szTagName, "PLAYER_HONOR"))
		{
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "single"))
				{
					pHonorStruct->nplayerhonorsingle = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "assist"))
				{
					pHonorStruct->nassist = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "singleTotalDiv"))
				{
					pHonorStruct->fsingleTotalDiv = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "assistTotalDiv"))
				{
					pHonorStruct->fassistTotalDiv = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "killDelay"))
				{
					pHonorStruct->fkillDelay = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "assistDelay"))
				{
					pHonorStruct->fassistDelay = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "HONOR"))
		{
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "actorType"))
				{
					pHonorStruct->strActorType = szAttrValue;
				}
				else if (!stricmp(szAttrName, "single"))
				{
					pHonorStruct->nHonorActorsingle = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "all"))
				{
					pHonorStruct->nall = atoi(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "HONOR_SOUND"))
		{
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "lessGainHonor"))
				{
					pHonorStruct->strLessHonor = szAttrValue;
				}
				else if (!stricmp(szAttrName, "regularGainHonor"))
				{
					pHonorStruct->strRegularGainHonor = szAttrValue;
				}
				else if (!stricmp(szAttrName, "moreGainHonor"))
				{
					pHonorStruct->strMoreGainHonor = szAttrValue;
				}
			}
		}
		m_pHonorList.push_back(move(pHonorStruct));
	}
}

void MMatchBlitzKrieg::LoadRouteList(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!stricmp(szTagName, "ROUTE"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			RouteListStruct* pRouteList = new RouteListStruct();

			for (int j = 0; j < nAttrCount; ++j)
			{

				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "id"))
				{
					pRouteList->nid = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "route1"))
				{
					pRouteList->strroute1 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route2"))
				{
					pRouteList->strroute2 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route3"))
				{
					pRouteList->strroute3 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route4"))
				{
					pRouteList->strroute4 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route5"))
				{
					pRouteList->strroute5 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route6"))
				{
					pRouteList->strroute6 = szAttrValue;
				}
				else if (!stricmp(szAttrName, "route7"))
				{
					pRouteList->strroute7 = szAttrValue;
				}
			}
			m_pRouteList.insert(std::map<int, RouteListStruct*>::value_type(pRouteList->nid, pRouteList));
		}
	}
}

void MMatchBlitzKrieg::LoadBuildingInfo(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];


	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		BuildingStruct* pBuilding = new BuildingStruct();
		if (!stricmp(szTagName, "BARRICADE"))
		{
			int nAttrCount = chrElement.GetAttributeCount();
			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "dist"))
				{
					pBuilding->ndist = atoi(szAttrValue);
				}
				else if (!stricmp(szAttrName, "reduceDamageRatio"))
				{
					pBuilding->freduceDamageRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "recoveryMagazineRatio"))
				{
					pBuilding->frecoveryMagazineRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "recoveryDelay"))
				{
					pBuilding->frecoveryDelay = (float)atof(szAttrValue);
				}
			}
		}
		else if (!stricmp(szTagName, "RADAR"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);

				if (!stricmp(szAttrName, "dist"))
				{
					pBuilding->ndist = atoi(szAttrValue);
				}
				if (!stricmp(szAttrName, "recoveryApHpRatio"))
				{
					pBuilding->frecoveryApHpRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "recoveyrMagazineRatio"))
				{
					pBuilding->frecoveryMagazineRatio = (float)atof(szAttrValue);
				}
				else if (!stricmp(szAttrName, "recoveryDelay"))
				{
					pBuilding->frecoveryDelay = (float)atof(szAttrValue);
				}
			}
		}
		m_pBuilding.push_back(move(pBuilding));
	}
}

void MMatchBlitzKrieg::LoadHonorItemSubTag(MXmlElement& Element)
{
	char szAttrValue[256];
	char szAttrName[64];
	char szTagName[128];

	int nChildCount = Element.GetChildNodeCount();
	MXmlElement chrElement;
	for (int i = 0; i < nChildCount; i++)
	{
		chrElement = Element.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;
		HonorItemListStruct* pHonorActor = new HonorItemListStruct();

		if (!stricmp(szTagName, "HONOR_ITEM"))
		{
			int nAttrCount = chrElement.GetAttributeCount();

			for (int j = 0; j < nAttrCount; ++j)
			{
				chrElement.GetAttribute(j, szAttrName, szAttrValue);
				if (!stricmp(szAttrName, "actor"))
				{
					pHonorActor->strHonorItemActor = szAttrValue;
				}
				m_pItemList.insert(std::map<string, HonorItemListStruct*>::value_type(pHonorActor->strHonorItemActor, pHonorActor));
			}
		}
	}
}