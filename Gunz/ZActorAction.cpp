#include "stdafx.h"
#include "ZActorAction.h"

ZActorAction::~ZActorAction()
{
	delete m_pMeleeShot;
	
	int num = GetNumRangeShot();
	for (int i = 0; i < num; ++i) {
		delete m_vecRangeShot[i];
	}
}

void ZActorActionMeleeShot::ProcessShot( IGame* pGame, const MUID& uidOwner, float fShotTime ) const
{
	ZObject *pAttacker = ZGetObjectManager()->GetObject(uidOwner);
	if (!pAttacker) {
		return;
	}

	if (m_fRange == 0) {
		return;
	}

	float fRadius = cosf( ToRadian( m_fAngle * 0.5f));

	// �������� ��ġ�� ���⿡ ���� ����ȭ
	rvector AttackerPos		= pAttacker->GetPosition();
	rvector AttackerNorPos	= AttackerPos;
	AttackerNorPos.z		= 0;

	rvector AttackerDir		= pAttacker->m_Direction;
	rvector AttackerNorDir	= AttackerDir;
	AttackerNorDir.z		= 0; 
	Normalize( AttackerNorDir);

	// ���� ���
	bool bPlayer = false;
	rvector Pos = pAttacker->GetPosition();
	ZApplication::GetSoundEngine()->PlaySoundElseDefault("blade_swing", "blade_swing", rvector(Pos.x, Pos.y, Pos.z), bPlayer);

	// ������ Ÿ�ٿ� ��Ʈ �ߴ��� üũ�Ѵ�.
	bool bHit = false;

	// Ÿ�Ͽ� ���� �˻縦 �����Ѵ�.
	for ( ZObjectManager::iterator itor = ZGetObjectManager()->begin(); itor != ZGetObjectManager()->end(); ++itor)
	{
		// Ÿ�ٿ� ���� ������ ��´�.
		ZObject* pVictim = (*itor).second;
		if (!pVictim) {
			continue;
		}

		// Ÿ���� ���� ���¸� ���� Ÿ������ �Ѿ��.
		ZModule_HPAP* pModule = (ZModule_HPAP*)pVictim->GetModule(ZMID_HPAP);
		if (pVictim->IsDie()) {
			continue;
		}

		// Ÿ���� ������ �ڽ��̸� ���� Ÿ������ �Ѿ��.
		if (pAttacker == pVictim) {
			continue;
		}

		if (pAttacker->GetTeamID() == pVictim->GetTeamID()) {
			continue;
		}

		// Ÿ���� ������ ���Ѵ�.
		rvector VictimPos, VictimDir;
		if (!pVictim->GetHistory(&VictimPos, &VictimDir, fShotTime)) {
			continue;
		}

		// �����ڿ� ���� Ÿ���� ��ġ�� ���� ����ȭ
		rvector VictimNorPos	= VictimPos;
		VictimNorPos.z			= 0;

		rvector VictimNorDir	= VictimPos - ( AttackerPos - ( AttackerNorDir * 50.f));
		VictimNorDir.z			= 0;
		Normalize( VictimNorDir);


		// �����ڿ� Ÿ�ٰ��� x,y ��� ���� �Ÿ��� ���� ����, �Ÿ��� ���� ���� �Ÿ����� �ָ� ���� Ÿ������ �Ѿ��.
		float fDist = Magnitude( AttackerNorPos - VictimNorPos);
		if (fDist > m_fRange) {
			continue;
		}

		// Ÿ���� ���� ������ ���� ���̸� ���� Ÿ������ �Ѿ��.
		float fDot = D3DXVec3Dot( &AttackerNorDir, &VictimNorDir);
		if (fDot < fRadius) {
			continue;
		}

		// Ÿ���� �ش� ���� ���� �ȿ��� ���� ���� ���̿� �ִ��� Ȯ���Ѵ�.
		if (!pGame->InRanged(pAttacker, pVictim)) {
			continue;
		}

		// �����ڿ� Ÿ�� ���̿� ���� ���� ������ ���� Ÿ������ �Ѿ��.
		int nDebugRegister = 0;
		if (pGame->IsWallBlocked(pAttacker, pVictim, nDebugRegister, true)) {
			continue;
		}

		if (GetThrust())
		{
			bool bDamage = false;

			if (fDist < GetRange())
			{
				float add_time = 0.3f * (fDist / 600.f);
				float time = fShotTime + add_time;

				ZGetEffectManager()->AddSwordUppercutDamageEffect(VictimPos, pVictim->GetUID(), (DWORD)(add_time * 1000));

				if (pVictim == ZGetGame()->m_pMyCharacter) {
					ZGetGame()->m_pMyCharacter->ReserveDashAttacked(pAttacker->GetUID(), time, AttackerDir);
				}

				pVictim->OnBlastDagger(AttackerDir, AttackerPos);

				if (ZGetGame()->CanAttack(pAttacker, pVictim))  {
					pVictim->OnDamagedSkill(pAttacker, AttackerPos, ZD_MELEE, MWT_DAGGER, m_fDamage, m_fPierce);
				}

				if (!m_strSound.empty()) {
					ZGetSoundEngine()->PlaySound((char*)m_strSound.c_str(), VictimPos);
				}

#ifdef _DEVELOPER
				mlog("CQ NPC Lunge!\n");
#endif
			}
		}
		else if (GetUpperCut())
		{
			if (pVictim == ZGetGame()->m_pMyCharacter) {
				ZGetGame()->m_pMyCharacter->SetLastThrower(pAttacker->GetUID(), fShotTime + 1.0f);
				ZPostReaction(fShotTime, 2);
				ZGetGame()->m_pMyCharacter->AddVelocity(rvector(0, 0, 1700));
			}

			pVictim->OnBlast(AttackerDir);

			if (ZGetGame()->CanAttack(pAttacker, pVictim))	{
				pVictim->OnDamagedSkill(pAttacker, AttackerPos, ZD_MELEE, MWT_DAGGER, m_fDamage, m_fPierce);
			}

			if (!m_strSound.empty()) {
				ZGetSoundEngine()->PlaySound((char*)m_strSound.c_str(), VictimPos);
			}

#ifdef _DEVELOPER
			mlog("CQ NPC Uppercut!\n");
#endif
		}
		else
		{
			if (pVictim->IsGuard() && (DotProduct(pVictim->m_Direction, AttackerNorDir) < 0))
			{
				rvector pos = pVictim->GetPosition();
				pos.z += 120.f;

				// ���� ����Ʈ ǥ��
				ZGetEffectManager()->AddSwordDefenceEffect(pos + (pVictim->m_Direction * 50.f), pVictim->m_Direction);
				pVictim->OnMeleeGuardSuccess();
				return;
			}

			// �밡�� ���¿��� ������ ������... �´� ����Ʈ
			rvector pos = pVictim->GetPosition();
			pos.z += 130.f;
			pos -= AttackerDir * 50.f;
			// ����Ʈ�� ����Ѵ�.
			ZGetEffectManager()->AddBloodEffect(pos, -VictimNorDir);
			ZGetEffectManager()->AddSlashEffect(pos, -VictimNorDir, 0);

			pVictim->OnDamaged(pAttacker, pAttacker->GetPosition(), ZD_MELEE, MWT_DAGGER, m_fDamage, m_fPierce, 0);

			if (m_strSound.empty()) {
				ZGetSoundEngine()->PlaySoundBladeDamage(NULL, pos);	// �⺻ ��Ҹ�
			}
			else {
				ZGetSoundEngine()->PlaySound((char*)m_strSound.c_str(), pos);
			}

			bHit = true;
		}
	}

	return;
}

void ZActorActionRangeShot::ProcessShot(const rvector& pos, const rvector& dir, ZObject* pOwner) const
{
	ZGetGame()->m_WeaponManager.AddNewQuestProjectile(this, pos, dir, pOwner);
}


void ZActorActionGrenadeShot::ProcessShot(const rvector& pos, const rvector& dir, ZObject* pOwner) const
{
	ZGetGame()->m_WeaponManager.AddNewQuestTrap(this, (D3DXVECTOR3)pos, (D3DXVECTOR3)dir * GetForce(), pOwner);

	if (!m_strSound.empty()) {
		ZGetSoundEngine()->PlaySound((char*)m_strSound.c_str(), pos);
	}
}

void ZActorActionEffect::ProcessShot(const rvector& Pos, const rvector& Dir, const MUID& uidOwner) const
{
	if (GetMeshName() == "") {
		mlog("Fatal Attack Error\n");
	}
	else {
		ZGetEffectManager()->AddWithScale(GetMeshName(), Pos, Dir, uidOwner, GetDelay(), GetScale());
	}
}

void ZActorActionSound::ProcessSound(string strSoundPath) const
{
	if (strSoundPath.empty()) {
		mlog("Fatal Sound Error\n");
	}
	else {
		ZGetSoundEngine()->PlaySound((char*)strSoundPath.c_str(), false, GetDelay());
	}
}