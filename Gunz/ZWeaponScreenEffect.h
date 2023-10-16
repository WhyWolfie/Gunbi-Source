#ifndef _ZWEAPONSCREENEFFECT_H
#define _ZWEAPONSCREENEFFECT_H

#include "ZPrerequisites.h"

class ZScreenEffect;

class ZWeaponScreenEffect
{
private:
	MMatchWeaponType		m_nWeaponType;
	MBitmap*				m_pSniferPanel;
	ZCROSSHAIR_STATUS		m_nStatus;
	float					PrevMouseSens;

	void InitWeapon(MMatchWeaponType nWeaponType);
	void FinalWeapon(MMatchWeaponType nWeaponType);
public:
	ZWeaponScreenEffect();
	virtual ~ZWeaponScreenEffect();
public:
	void SetState(ZCROSSHAIR_STATUS nState)	{ m_nStatus = nState; }
	void Draw(MDrawContext* pDC);
	void OnGadget(MMatchWeaponType nWeaponType);
	void OnGadgetOff();
	void Create();
	void Destroy();
};


#endif
