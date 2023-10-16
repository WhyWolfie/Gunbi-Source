#pragma once

class ZFSM;

class ZFSMManager
{
	typedef map<string, ZFSM*>		MapFsm;
	typedef MapFsm::iterator		ItorFsm;
	
	MapFsm m_mapFSM;

public:
	ZFSMManager() {}
	~ZFSMManager();

	int GetFsmCount() { return (int)m_mapFSM.size(); }

	void AddFsm(ZFSM** arrayFsm, unsigned int count);
	ZFSM* GetFsm(const char* szName);

	static ZFSMManager* GetInstance();
};

inline ZFSMManager* ZGetFSMMgr() { return ZFSMManager::GetInstance(); }