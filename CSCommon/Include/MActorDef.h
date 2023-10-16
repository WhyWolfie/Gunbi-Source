#pragma once

#include "d3dx9math.h"

// 신규 퀘스트의 FSM을 사용한 npc의 정의 포맷
class MActorDef
{
	string m_strName;
	string m_strFsm;
	string m_strModel;
	string m_strDieSound;
	string m_strNeverBlastedSound;

	int m_nMaxHp;
	int m_nMaxAp;
	int m_nType;

	float m_fCollRadius;
	float m_fCollHeight;

	float m_fSpeed;		// 초당 이동거리
	float m_fRotSpeed;	// 초당 회전속도(radian)
	float m_fGroggyRecoverySpeed;	// 초당 그로기 회복량
	float m_fScale;

	bool m_bNeverBlasted;		// 띄우기 면역
	bool m_bMeshPicking;
	bool m_bBoss;
	bool m_bGrenadeCollision;
	bool m_bForcedCollup120;
	bool m_bNoShadow;

public:
	void SetName(const char* sz) { m_strName = sz; }
	void SetFsmName(const char* sz) { m_strFsm = sz; }
	void SetModelName(const char* sz) { m_strModel = sz; }
	const char* GetName() { return m_strName.c_str(); }
	const char* GetFsmName() { return m_strFsm.c_str(); }
	const char* GetModelName() { return m_strModel.c_str(); }

	void SetMaxHp(int n) { m_nMaxHp = n; }
	void SetMaxAp(int n) { m_nMaxAp = n; }
	int GetMaxHp() { return m_nMaxHp; }
	int GetMaxAp() { return m_nMaxAp; }

	void SetCollisionRadius(float f) { m_fCollRadius = f; }
	void SetCollisionHeight(float f) { m_fCollHeight = f; }
	float GetCollisionRadius() { return m_fCollRadius; }
	float GetCollisionHeight() { return m_fCollHeight; }

	void SetSpeed(float f) { m_fSpeed = f; }
	void SetRotSpeed(float f) { m_fRotSpeed = f; }
	void SetScale(float f) { m_fScale = f; }
	float GetSpeed() { return m_fSpeed; }
	float GetRotSpeed() { return m_fRotSpeed; }
	float GetScale() { return m_fScale; }

	void SetGroggyRecoverySpeed(float f) { m_fGroggyRecoverySpeed = f; }
	float GetGroggyRecoverySpeed() { return m_fGroggyRecoverySpeed; }

	void SetNeverBlasted(bool b) { m_bNeverBlasted=b; }
	bool IsNeverBlasted() { return m_bNeverBlasted; }

	void SetMeshPicking(bool b) { m_bMeshPicking = b; }
	bool IsMeshPicking() { return m_bMeshPicking; }

	void SetBoss(bool b) { m_bBoss = b; }
	bool IsBoss() { return m_bBoss; }

	void SetGrenadeCollision(bool b) { m_bGrenadeCollision = b; }
	bool IsGreneadeCollision() { return m_bGrenadeCollision; }

	void SetForcedCollup120(bool b) { m_bForcedCollup120 = b; }
	bool IsForcedCollup120() { return m_bForcedCollup120; }

	void SetNoShadow(bool b) { m_bNoShadow = b; }
	bool IsNoShadow() { return m_bNoShadow; }

	void SetDieSound(const char* sz) { m_strDieSound = sz; }
	const char* GetDieSound() { return m_strDieSound.c_str(); }

	void SetNeverBlastedSound(const char* sz) { m_strNeverBlastedSound = sz; }
	const char* GetNeverBlastedSound() { return m_strNeverBlastedSound.c_str(); }

	void SetType(int i) { m_nType = i; }
	int GetType() { return m_nType; }
};


class MActorDefManager
{
	typedef map<string, MActorDef*>		MapActorDef;
	typedef MapActorDef::iterator		ItorActorDef;
	MapActorDef m_mapActorDef;

public:
	~MActorDefManager();
	static MActorDefManager* GetInstance();

	bool ReadXml(MZFileSystem* pFileSystem, const char* szFileName);
	bool ParseXmlFromStr(const char* szXmlContent);
	int GetCount() { return (int)m_mapActorDef.size(); }
	void Clear();
	bool AddActorDef(MActorDef* pDef);

	MActorDef* GetDef(const char* szActorName);
};
inline MActorDefManager* MGetActorDefMgr() { return MActorDefManager::GetInstance(); }