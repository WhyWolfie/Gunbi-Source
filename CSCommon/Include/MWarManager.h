#pragma once

struct MWarNPC {
	string strModel;
	int nHP;
	int nAP;
	float fPosX;
	float fPosY;
	float fPosZ;
	float fDirX;
	float fDirY;
	float fDirZ;
};

struct MWarEvent {
	string strMap;
	int nInterval;
	int nTime;
	int nPrepare;
};

struct MWarReward {
	int nType;
	int nXp;
	int nBp;
	int nCash;
};

class MWarManager
{
private:
	MWarEvent		pEvent;
	vector<MWarNPC> pNpc;
	vector<MWarReward> pReward;
	unsigned long	ulLastWar;
	MUID			m_UID;


public:
	MWarManager();

	void Announce(const char* szMsg, ZAdminAnnounceType nMode = ZAAT_CHAT, MUID uidChar = MUID(0, 0));
	bool ReadXml(const char* szFileName);
	bool Start();

	MUID GetStageUID() { return m_UID; }
	void SetStageUID(MUID m) { m_UID = m; }

	vector<MWarNPC> GetNpc() { return pNpc; }
	vector<MWarReward> GetReward() { return pReward; }


	static MWarManager* GetInstance();
};

inline MWarManager* MGetWarMgr()
{
	return MWarManager::GetInstance();
}
