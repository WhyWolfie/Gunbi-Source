#pragma once

struct HonorStruct
{
	int nStartHonor;
	int nautoIncHonorSec;
	int nautoIncHonor;
	int nfirstKillHonor;
	int nautoIncHonorSub;
};

struct RespawnStruct
{
	int nbaseTime;
	int ninvincibleTime;
};

struct ClassSelectStruct
{
	int nClassSelectTime;
};

struct FinishDelayTimeStruct
{
	int nFinishDelayTime;
};

struct EnhancePlayerStruct
{
	int napHp;
	int ndps;
};

struct EnhanceNPCStruct
{
	int nNextTime;
	int nMaxCount;
	float fEnhancedRatio;
};

struct BuildingStruct
{
	float freduceDamageRatioFromPlayer;
	float fcheckDelay;
	//Barricade Info & Radar Info
	int	  ndist;
	float frecoveryMagazineRatio;
	float frecoveryDelay;
	//Barricade Only
	float freduceDamageRatio;
	//Radar Info
	float frecoveryApHpRatio;
};

struct HonorItemListStruct
{
	int nrespawnTime;
	string strHonorItemActor;
};

struct ReinforceListStruct
{
	int nbarricadeCount;
	string strReinforceState;
	bool balliance;
};

struct RewardStruct
{
	int nbaseBounty;
	int nbaseExp;
	int nminTime;
	int nminHonor;
	int nwinnerMedal;
	int nloserMedal;
	int nminuteBonusMedal;
	int nminuteBonusMedalMax;
	int nminPlayCount;
	int nminPlayCountBonusMedal;
	int nWaitingTime;
	int nWaitingRewardMedal;
	int nWaitingRewardMax;
	float fWinXPBonusMVP;
	float fWinBPBonusMVP;
	float fWinMedalBonusMVP;
	float fLoseXPBonusMVP;
	float fLoseBPBonusMVP;
	float fLoseMedalBonusMVP;
};

struct PenaltyStruct
{
	int nfirst;
	int nsecond;
	int nafterThird;
};

struct ClassTableStruct
{
	//Hunter
	float faquirHonorRatio;
	//Slaughter
	int nenchantFireDamage;
	float ffireDamageDuration;
	float faddMagazineRatio;
	//Trickster
	float frecoveryApHpRatio;
	float frecoveryMagazineRatio;
	float freduceDamageRatio;
	//Gladiator
	int nenhanceMeleeDPS;
	int naddMaxApHp;
	//Duelist
	int naddShotgunMagazine;
	int nenhanceShotgunDamage;
	//Incinerator
	int nenchantFireDamageIncinerator;
	float ffireDamageDurationIncinerator;
	int nreduceDPS;
	//CombatOfficer
	float freduceDamageRatioForMyTeam;
	int ndistance;
	float fcheckDelay;
	//Assassin
	float fenhanceDamageRatio;
	//Terrorist
	float fenhanceDamageRatioAtBuilding;
};

struct ClassBookStruct
{
	int nGladiatorID;
	int nDuelistID;
	int nIncineratorID;
	int nCombatOfficerID;
	int nAssassinID;
	int nTerroristID;
};

struct UpgradeStruct
{
	int nrequireHonor[4];
	int ndps[4];
	float freduceShotDelayRatio[4];
	int nhpAp[4];
	int nenchantFire[4];
	float ffireDamageDuration;
	float fmagazineRatio[4];
	float freviveRatio[4];
};

struct WeaponStruct
{
	float fsmg;
	float fRifle;
	float fMachineGun;
	float fpistol;
	float frevolver;
	float fshotGun;
	float froket;
	float fdagger;
	float fkatana;
	float fdoubleKatana;
	int nsmg;
	int nRifle;
	int nMachineGun;
	int npistol;
	int nrevolver;
	int nshotGun;
	int nroket;
	int ndagger;
	int nkatana;
	int ndoubleKatana;
};

struct SpawnListStruct
{
	string strActor;
	int nNumActor;
	int nTeamID;
};

struct HonorListStruct
{
	int nplayerhonorsingle;
	int nassist;
	float fsingleTotalDiv;
	float fassistTotalDiv;
	float fkillDelay;
	float fassistDelay;
	string strActorType;
	int nHonorActorsingle;
	int nall;
	string strLessHonor;
	string strRegularGainHonor;
	string strMoreGainHonor;
};

struct RouteListStruct
{
	int nid;
	string strroute1;
	string strroute2;
	string strroute3;
	string strroute4;
	string strroute5;
	string strroute6;
	string strroute7;
};

struct EventMsgStruct
{
	int nviewTime;
	float fdelayTime;
	int ndamagedRadarCoolDown;
	string strsound_Benefit;
	string strsound_Loss;
};

struct HelpMsgStruct
{
	int nviewTime;
	int ndist;
	int nhonor;
	string strsound;
};

class MMatchBlitzKrieg
{
private:
	vector<HonorStruct*> m_pHonorStruct;
	vector<RespawnStruct*> m_pRespawnStruct;
	ClassSelectStruct* m_pClass;
	FinishDelayTimeStruct* m_pFinish;
	vector<EnhancePlayerStruct*> m_pEnhancePlayer;
	vector<EnhanceNPCStruct*> m_pEnhanceNPC;
	vector<BuildingStruct*> m_pBuilding;
	map<string, HonorItemListStruct*> m_pItemList;
	vector<ReinforceListStruct*> m_pReinforceList;
	vector<RewardStruct*> m_pRewards;
	vector<PenaltyStruct*> m_pPenalty;
	vector<ClassTableStruct*> m_pClassTable;
	std::map<int, ClassBookStruct*> m_pClassBook;
	vector<UpgradeStruct*> m_pUpgradeStruct;
	vector<WeaponStruct*> m_pWeaponStruct;
	map<string, SpawnListStruct*> m_pSpawnList;
	vector<HonorListStruct*> m_pHonorList;
	map<int, RouteListStruct*> m_pRouteList;
	vector<EventMsgStruct*> m_pEventMsg;
	vector<HelpMsgStruct*> m_pHelpMsg;

public:
	MMatchBlitzKrieg();
	virtual ~MMatchBlitzKrieg();
	bool ReadXml(const char* szFileName, MZFileSystem* pFileSystem = 0);

private:
	void LoadBlitzEvent(MXmlElement& Element);
	void LoadBlitzHonorSubTag(MXmlElement& Element);
	void LoadBlitzClassTable(MXmlElement& Element);
	void LoadSpawnList(MXmlElement& Element);
	void LoadReinforceList(MXmlElement& Element);
	void LoadWeaponInfo(MXmlElement& Element);
	void LoadHonorList(MXmlElement& Element);
	void LoadRouteList(MXmlElement& Element);
	void LoadBuildingInfo(MXmlElement& Element);
	void LoadHonorItemSubTag(MXmlElement& Element);

public:
	static MMatchBlitzKrieg* GetInstance();

	SpawnListStruct* GetSpawnInfo(string strInfo)
	{
		std::map<string, SpawnListStruct*>::iterator itor = m_pSpawnList.find(strInfo);
		if (itor != m_pSpawnList.end())
		{
			return (*itor).second;
		}
		return nullptr;
	}

	RouteListStruct* GetRouteList(int nID)
	{
		std::map<int, RouteListStruct*>::iterator itor = m_pRouteList.find(nID);
		if (itor != m_pRouteList.end())
		{
			return (*itor).second;
		}
		return nullptr;
	}

	ClassBookStruct* GetPlayerClass(int nClassID)
	{
		std::map<int, ClassBookStruct*>::iterator itor = m_pClassBook.find(nClassID);
		if (itor != m_pClassBook.end())
		{
			return (*itor).second;
		}
		return nullptr;
	}

	HonorItemListStruct* GetHonorItemActor(string strActor)
	{
		std::map<string, HonorItemListStruct*>::iterator itor = m_pItemList.find(strActor);
		if (itor != m_pItemList.end())
		{
			return (*itor).second;
		}
		return nullptr;
	}

	int GetSelectTime()
	{
		return m_pClass->nClassSelectTime;
	}

}; 
inline MMatchBlitzKrieg* MGetMatchBlitzKrieg() { return MMatchBlitzKrieg::GetInstance(); }