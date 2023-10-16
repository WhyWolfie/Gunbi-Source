#include "stdafx.h"
#include "ZDummyNPC.h"
#include "ZModule_HPAP.h"
#include "ZModule_Resistance.h"
#include "ZModule_FireDamage.h"
#include "ZModule_ColdDamage.h"
#include "ZModule_LightningDamage.h"
#include "ZModule_PoisonDamage.h"

ZDummyNPC::ZDummyNPC(IGame* pGame)
	: ZActorBase(pGame)
	, m_bMyControl(false)
	, m_nLastTimePostBasicInfo(0)
	, m_nLastTimePostSyncInfo(0)
{
	m_bIsNPC = true;
	SetVisible(true);
	m_bInitialized = true;

	m_pModule_HPAP = new ZModule_HPAP;
	m_pModule_Resistance = new ZModule_Resistance;
	m_pModule_FireDamage = new ZModule_FireDamage;
	m_pModule_ColdDamage = new ZModule_ColdDamage;
	m_pModule_PoisonDamage = new ZModule_PoisonDamage;
	m_pModule_LightningDamage = new ZModule_LightningDamage;

	AddModule(m_pModule_HPAP);
	AddModule(m_pModule_Resistance);
	AddModule(m_pModule_FireDamage);
	AddModule(m_pModule_ColdDamage);
	AddModule(m_pModule_PoisonDamage);
	AddModule(m_pModule_LightningDamage);

	m_pModule_Movable->Active(false);

	m_nTeamID.Set_MakeCrc(MMT_ALL);
}

ZDummyNPC::~ZDummyNPC()
{
	EmptyHistory();
	DestroyShadow();

	RemoveModule(m_pModule_HPAP);
	RemoveModule(m_pModule_Resistance);
	RemoveModule(m_pModule_FireDamage);
	RemoveModule(m_pModule_ColdDamage);
	RemoveModule(m_pModule_PoisonDamage);
	RemoveModule(m_pModule_LightningDamage);

	delete m_pModule_HPAP;
	delete m_pModule_Resistance;
	delete m_pModule_FireDamage;
	delete m_pModule_ColdDamage;
	delete m_pModule_PoisonDamage;
	delete m_pModule_LightningDamage;
}

void ZDummyNPC::OnDraw()
{
	if (m_pVMesh == NULL) {
		return;
	}

	Draw_SetLight(GetPosition());

	m_pVMesh->Render();
}

void ZDummyNPC::OnUpdate(float fDelta)
{
	if (m_pVMesh && !IsDie()) {
		m_pVMesh->SetVisibility(1.f);
	}
	else {
		m_pVMesh->SetVisibility(0.f);
	}

	if (IsMyControl()) {
		PostBasicInfo();
		SyncHpApInfo();
		CheckDead();
	}
	else {
		m_pModule_Movable->Update(fDelta);
	}
}

void ZDummyNPC::SetMyControl(bool bMyControl)
{
	m_bMyControl = bMyControl;
}

bool ZDummyNPC::IsMyControl()
{
	return m_bMyControl;
}

bool ZDummyNPC::Init(char* szModelName, ZDUMMY_INFO* pInfo)
{
	rvector nPos = rvector(pInfo->posx, pInfo->posy, pInfo->posy);
	rvector nDir = rvector(pInfo->dirx, pInfo->diry, pInfo->diry);

	AllocObjectVMesh();

	D3DXMATRIX world;
	MakeWorldMatrix(&world, nPos, nDir, rvector(0, 0, 1));

	m_pVMesh->InitNpcMesh(this, szModelName);
	m_pVMesh->SetScale(rvector(pInfo->fScale, pInfo->fScale, pInfo->fScale));
	m_pVMesh->SetWorldMatrix(world);

	CreateShadow();

	m_Collision.SetHeight(pInfo->fCollHeight);
	m_Collision.SetRadius(pInfo->fCollRadius);

	m_pModule_HPAP->SetMaxHP(pInfo->fHP);
	m_pModule_HPAP->SetHP(pInfo->fHP);

	m_pModule_HPAP->SetMaxAP(pInfo->fAP);
	m_pModule_HPAP->SetAP(pInfo->fAP);

	m_pModule_HPAP->SetRealDamage(true);
	
	SetDirection(nDir);
	SetPosition(nPos);

	return true;
}

void ZDummyNPC::SyncHpApInfo()
{
	DWORD nNowTime = timeGetTime();
	if (GetInitialized() == false) return;

	if (IsDie() && ZGetGame()->GetTime() - GetDeadTime() > 5.f) {
		return;
	}

	if ((int)(nNowTime - m_nLastTimePostSyncInfo) >= (60 * 1000 / 2)) {
		m_nLastTimePostSyncInfo = nNowTime;

		ZPOSTCMD3(MC_WAR_SYNC_HPAP, MCommandParameterUID(GetUID()), MCommandParameterInt(GetActualHP()), MCommandParameterInt(GetActualHP()));
	}
}

void ZDummyNPC::PostBasicInfo()
{
	DWORD nNowTime = timeGetTime();
	if (GetInitialized() == false) return;

	if (IsDie() && ZGetGame()->GetTime() - GetDeadTime() > 5.f) {
		return;
	}

	int nMoveTick = ZGetGame()->GetCharacterBasicInfoTick();

	if ((int)(nNowTime - m_nLastTimePostBasicInfo) >= nMoveTick) {
		m_nLastTimePostBasicInfo = nNowTime;

		ZDUMMY_BASICINFO pbi;
		pbi.uidNPC = GetUID();

		pbi.posx = GetPosition().x;
		pbi.posy = GetPosition().y;
		pbi.posz = GetPosition().z;

		pbi.velx = GetVelocity().x;
		pbi.vely = GetVelocity().y;
		pbi.velz = GetVelocity().z;

		pbi.dirx = GetDirection().x*32000.0f;
		pbi.diry = GetDirection().y*32000.0f;
		pbi.dirz = GetDirection().z*32000.0f;

		pbi.fAP = (float)GetActualAP();
		pbi.fHP = (float)GetActualHP();

		ZPOSTCMD1(MC_WAR_DUMMY_BASICINFO, MCommandParameterBlob(&pbi, sizeof(ZDUMMY_BASICINFO)));

		// HISTORY COUNT
		ZBasicInfoItem *pitem = new ZBasicInfoItem;
		pitem->info.position = GetPosition();
		pitem->info.direction = GetDirection();
		pitem->info.velocity = GetVelocity();

		pitem->fSendTime = pitem->fReceivedTime = ZGetGame()->GetTime();
		m_BasicHistory.push_back(pitem);

		while (m_BasicHistory.size() > 100)
		{
			delete *m_BasicHistory.begin();
			m_BasicHistory.pop_front();
		}
	}
}

void ZDummyNPC::CheckDead() {

	if (IsMyControl() == false) {
		return;
	}

	if (IsDie()) {
		MUID uidAttacker = m_pModule_HPAP->GetLastAttacker();
		if (uidAttacker == MUID(0, 0)) {
			uidAttacker = ZGetGame()->GetMyUid();
		}

		ZPOSTCMD2(MC_WAR_NPC_DEAD, MCommandParameterUID(uidAttacker), MCommandParameterUID(GetUID()));

		SetMyControl(false);
	}
}

void ZDummyNPC::OnBasicInfo(ZDUMMY_BASICINFO* pbi)
{
	if (!IsMyControl())
	{
		SetPosition(rvector(pbi->posx, pbi->posy, pbi->posz));
		SetVelocity(rvector(pbi->velx, pbi->vely, pbi->velz));
		SetDirection(1.f / 32000.f * rvector(pbi->dirx, pbi->diry, pbi->dirz));
		
		m_pModule_HPAP->SetHP(pbi->fHP);
		m_pModule_HPAP->SetAP(pbi->fAP);
	}
}

bool ZDummyNPC::IsCollideable()
{
	if (!m_Collision.IsCollideable()) {
		return false;
	}
	return !IsDie();
}

bool ZDummyNPC::IsDie()
{
	if (GetActualHP() <= 0) {
		return true;
	}
	return false;
}

ZOBJECTHITTEST ZDummyNPC::HitTest(const rvector& origin, const rvector& to, float fTime, rvector *pOutPos)
{
	return HitTest_Actor(false, origin, to, fTime, pOutPos);
}

void ZDummyNPC::OnDamaged(ZObject* pAttacker, rvector srcPos, ZDAMAGETYPE damageType, MMatchWeaponType weaponType, float fDamage, float fPiercingRatio, int nMeleeType)
{
	if (pAttacker && pAttacker->GetTeamID() != GetTeamID() && fDamage > 0) {
		ZObject::OnDamaged(pAttacker, srcPos, damageType, weaponType, fDamage, fPiercingRatio, nMeleeType);
	}
}