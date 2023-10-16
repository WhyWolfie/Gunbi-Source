#pragma once
#include "ZActorBase.h"

struct ZDUMMY_INFO {
	float	fCollRadius;
	float	fCollHeight;

	float fHP, fAP;

	float	fScale;

	short	posx, posy, posz;
	short	dirx, diry, dirz;
};

struct ZDUMMY_BASICINFO {
	MUID	uidNPC;
	short	posx, posy, posz;
	short	dirx, diry, dirz;
	short	velx, vely, velz;

	float fHP, fAP;
};

class ZDummyNPC : public ZActorBase
{
private:
	ZModule_HPAP			*m_pModule_HPAP;
	ZModule_Resistance		*m_pModule_Resistance;
	ZModule_FireDamage		*m_pModule_FireDamage;
	ZModule_ColdDamage		*m_pModule_ColdDamage;
	ZModule_PoisonDamage	*m_pModule_PoisonDamage;
	ZModule_LightningDamage	*m_pModule_LightningDamage;

	bool m_bMyControl;
	MProtectValue<MMatchTeam> m_nTeamID;

	unsigned long int m_nLastTimePostBasicInfo;
	unsigned long int m_nLastTimePostSyncInfo;

public:
	ZDummyNPC(IGame* pGame);
	~ZDummyNPC();

	virtual MMatchTeam GetTeamID() { return m_nTeamID.Ref(); }
	virtual void SetTeamID(MMatchTeam i) { m_nTeamID.Set_CheckCrc(i); }

	virtual void OnDraw();
	virtual void OnUpdate(float fDelta);

	virtual void SetMyControl(bool bMyControl);
	virtual bool IsMyControl();

	virtual bool IsCollideable();
	virtual bool IsDie();

	virtual bool IsNeverPushed() { return false; }
	virtual float GetCollideRadius() { return 35.f; }
	virtual float ModifyAttackDamage(float fDamage) { return fDamage; }

	virtual int GetActualHP() { return m_pModule_HPAP->GetHP(); }
	virtual int GetActualAP() { return m_pModule_HPAP->GetAP(); }
	virtual int GetActualMaxHP() { return m_pModule_HPAP->GetMaxHP(); }
	virtual int GetActualMaxAP() { return m_pModule_HPAP->GetMaxAP(); }

	virtual void InputBasicInfo(ZBasicInfo* pni, BYTE anistate) {}
	virtual void InputBossHpAp(float fHp, float fAp) {}

	virtual void OnPeerDie(MUID& uidKiller) {}
	virtual void OnDie() {}

	virtual void GetDebugInfoString(char* szBuf1, char* szBuf2, int index) {}

public:
	bool Init(char* szModelName, ZDUMMY_INFO* pInfo);
	void PostBasicInfo();
	void SyncHpApInfo();

	void CheckDead();
	void OnBasicInfo(ZDUMMY_BASICINFO* pbi);

	virtual ZOBJECTHITTEST HitTest(const rvector& origin, const rvector& to, float fTime, rvector *pOutPos);
	void OnDamaged(ZObject* pAttacker, rvector srcPos, ZDAMAGETYPE damageType, MMatchWeaponType weaponType, float fDamage, float fPiercingRatio, int nMeleeType);
};