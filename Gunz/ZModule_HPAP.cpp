#include "stdafx.h"
#include "ZModule_HPAP.h"
#include "ZGame.h"
#include "ZApplication.h"
#include "CheckReturnCallStack.h"
#include "ZActor.h"

ZModule_HPAP::ZModule_HPAP() : m_LastAttacker(MUID(0,0)), m_bRealDamage_DebugRegister(false), m_fAccumulationDamage(0.f), m_bAccumulationDamage(false)
{
	float f = float(rand() % 1000); 
	f += float(rand() % 100) * 0.01f;
	m_fMask.Set_MakeCrc(f);

	m_bRealDamage.Set_MakeCrc(false);
	m_fHP.Set_MakeCrc(1000.f);
	m_fAP.Set_MakeCrc(1000.f);
	m_fMaxHP.Set_MakeCrc(100);
	m_fMaxAP.Set_MakeCrc(100);
}



ZModule_HPAP::~ZModule_HPAP()
{
}

float ZModule_HPAP::GetHP() 
{
	return  m_fHP.Ref() - GetMask();
}

float	ZModule_HPAP::GetAP() 
{
	return m_fAP.Ref() - GetMask();
}

void ZModule_HPAP::OnDamage(MUID uidAttacker,float fDamage, float fRatio)
{
	_ASSERT(fDamage >= 0);
	if (fDamage < 0) fDamage *= -1.f;	// ��ŷ���� ����������� �־ ȸ���� �� �����Ƿ�..

	m_LastAttacker = uidAttacker;

	// ����Ʈ �׽�Ʈ�� ġƮ üũ
#ifndef _PUBLISH
	if (CheckQuestCheet() == true) return;
#endif
	//jintriple3 ����� �������� �� ���� �ڵ�
	//�� ĳ���Ͱ� �ƴϰų� npc�� �ƴϸ�....
	//m_bRealDamage�� ��� �� Ŭ���̾�Ʈ���� ������ �������� �����ϳ� ���ϳ��� üũ�ϴ� ����.
	//MyCharacter �̰ų� NPC�� �� Ŭ���̾�Ʈ���� ������ ����.
	if(!m_bRealDamage.Ref())	
		PROTECT_DEBUG_REGISTER(!m_bRealDamage_DebugRegister)	//�̰� ���� ���ο� ���� �ϳ� �� ���.
			return;

	// NPC�� ���̵� ������������� ����
	ZObject* pAttacker = ZGetObjectManager()->GetObject(uidAttacker);
	if ((pAttacker) && (!IsPlayerObject(pAttacker)))
	{
		ZActorBase* pActor = (ZActorBase*)pAttacker;
		//fDamage = (int)(fDamage * (pActor->GetTC()));
		fDamage = pActor->ModifyAttackDamage(fDamage);
	}

	// ���� �ױ������� ���� �������� �����Ѵ�(�ϴ� �����ʸ�Ʈ ������)
	if(IsAccumulationDamage())
	{
		AccumulateDamage(fDamage);
#ifndef _PUBLISH	// ���κ��忡�� ���� ����� ���� ���
		char szAccumulationDamagePrint[256];
		sprintf(szAccumulationDamagePrint, "���� ���� ���������[%2.1f]", GetAccumulationDamage());
		ZChatOutput(MCOLOR(255, 200, 100), szAccumulationDamagePrint);
#endif
	}

#ifdef _ROOMTAGS
#ifdef _ROOMTAG_SHIELD
	if (ZApplication::GetInstance()->GetTagScanner()->Find(ZTN_SHIELD, TRUE) == true)
	{
		float fHPDamage = (float)((float)fDamage * fRatio);
		float fAPDamage = fDamage - fHPDamage;

		if ((GetAP() - fAPDamage) > 0)
		{
			SetAP(fAPDamage);
		}
		else
		{
			if ((GetAP() - fAPDamage) < 0)
			{
				fHPDamage += (fAPDamage - GetAP());
				fAPDamage -= (fAPDamage - GetAP());
			}
			SetHP(GetHP() - fHPDamage);
			SetAP(GetAP() - fAPDamage);
		}
	}
	else
#endif
#endif
	{
		float fHPDamage = (float)((float)fDamage * fRatio);
		float fAPDamage = fDamage - fHPDamage;

		if ((GetAP() - fAPDamage) < 0)
		{
			fHPDamage += (fAPDamage - GetAP());
			fAPDamage -= (fAPDamage - GetAP());
		}

		SetHP(GetHP() - fHPDamage);
		SetAP(GetAP() - fAPDamage);
	}
}


void ZModule_HPAP::InitStatus()
{
	m_LastAttacker = MUID(0,0);
}

bool ZModule_HPAP::CheckQuestCheet()
{
#ifdef _PUBLISH
	return false;
#endif

	// ����Ʈ �׽�Ʈ�� ġƮ üũ
	if (IsMyCharacter((ZObject*)m_pContainer))
	{
		if ((ZIsLaunchDevelop()) && (ZGetGameClient()->GetServerMode() == MSM_TEST))
		{
			if (ZGetGameTypeManager()->IsQuestDerived(ZGetGameClient()->GetMatchStageSetting()->GetGameType()))
			{
				if (ZGetQuest()->GetCheet(ZQUEST_CHEET_GOD) == true) return true;
			}
		}
	}

	return false;
}

void ZModule_HPAP::ShiftFugitiveValues()
{
	m_fMask.ShiftHeapPos_CheckCrc();

	m_fMaxHP.ShiftHeapPos_CheckCrc();
	m_fMaxAP.ShiftHeapPos_CheckCrc();

	m_fHP.ShiftHeapPos_CheckCrc();
	m_fAP.ShiftHeapPos_CheckCrc();

	m_bRealDamage.ShiftHeapPos_CheckCrc();
}