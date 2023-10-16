#include "stdafx.h"
#include "ZWeaponScreenEffect.h"
#include "ZConfiguration.h"
#include "ZGame.h"
#include "ZMyInfo.h"
#include "ZMyItemList.h"

ZWeaponScreenEffect::ZWeaponScreenEffect()
{
	m_nWeaponType = MWT_NONE;
	m_pSniferPanel = NULL;
	m_nStatus = ZCS_NORMAL;
}

ZWeaponScreenEffect::~ZWeaponScreenEffect()
{
	m_pSniferPanel = NULL;
	m_nStatus = ZCS_NORMAL;
}

void ZWeaponScreenEffect::Draw(MDrawContext* pDC)
{
	if (ZGetGame()->m_pMyCharacter == NULL) return;

	switch (m_nWeaponType)
	{
	case MWT_SNIFER:
			{
				switch (m_nStatus)
				{
					case ZCS_NORMAL:
						{
							m_pSniferPanel = MBitmapManager::Get("crosshair_snifer.png");
							if (m_pSniferPanel == NULL)
								break;

							pDC->SetBitmap(m_pSniferPanel);
							pDC->Draw(0, 0, RGetScreenWidth(), RGetScreenHeight());
						}
						break;
					case ZCS_PICKENEMY:
						{
							m_pSniferPanel = MBitmapManager::Get("crosshair_snifer_hover.png");
							if (m_pSniferPanel == NULL)
								break;
							
							pDC->SetBitmap(m_pSniferPanel);
							pDC->Draw(0, 0, RGetScreenWidth(), RGetScreenHeight());
							
						}
						break;
				}
			}
		break;
	}
}

void ZWeaponScreenEffect::InitWeapon(MMatchWeaponType nWeaponType)
{
	switch (nWeaponType)
	{
	case MWT_SNIFER:
		{
			PrevMouseSens = Z_MOUSE_SENSITIVITY;
			g_fFOV = (12.0f / 180.0f * pi);
			ZGetConfiguration()->SetMouseSensitivityInFloat(ZGetConfiguration()->GetMouseSensitivityInFloat() / 10.0f);
		}
		break;
	}

}


void ZWeaponScreenEffect::FinalWeapon(MMatchWeaponType nWeaponType)
{
	switch (nWeaponType)
	{
	case MWT_SNIFER:
		{
			g_fFOV = (70.0f / 180.0f * pi);
			ZGetConfiguration()->SetMouseSensitivityInFloat(ZGetConfiguration()->GetMouseSensitivityInFloat() * 10.0f);
		}
		break;
	}
}

void ZWeaponScreenEffect::OnGadget(MMatchWeaponType nWeaponType)
{
	if (m_nWeaponType == nWeaponType) return;
	if (m_nWeaponType != MWT_NONE) OnGadgetOff();

	InitWeapon(nWeaponType);
	m_nWeaponType = nWeaponType;
}

void ZWeaponScreenEffect::OnGadgetOff()
{
	FinalWeapon(m_nWeaponType);
	m_nWeaponType = MWT_NONE;
}

void ZWeaponScreenEffect::Create()
{
	return;
}

void ZWeaponScreenEffect::Destroy()
{
	m_nWeaponType = MWT_NONE;
	if (m_pSniferPanel)
	{
		m_pSniferPanel = NULL;
		ZGetConfiguration()->SetMouseSensitivityInFloat(PrevMouseSens);
	}
	m_nStatus = ZCS_NORMAL;
}