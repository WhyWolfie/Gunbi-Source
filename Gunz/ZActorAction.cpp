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

	// 공격자의 위치와 방향에 대한 정규화
	rvector AttackerPos		= pAttacker->GetPosition();
	rvector AttackerNorPos	= AttackerPos;
	AttackerNorPos.z		= 0;

	rvector AttackerDir		= pAttacker->m_Direction;
	rvector AttackerNorDir	= AttackerDir;
	AttackerNorDir.z		= 0; 
	Normalize( AttackerNorDir);

	// 사운드 출력
	bool bPlayer = false;
	rvector Pos = pAttacker->GetPosition();
	ZApplication::GetSoundEngine()->PlaySoundElseDefault("blade_swing", "blade_swing", rvector(Pos.x, Pos.y, Pos.z), bPlayer);

	// 공격이 타겟에 히트 했는지 체크한다.
	bool bHit = false;

	// 타켓에 대한 검사를 수행한다.
	for ( ZObjectManager::iterator itor = ZGetObjectManager()->begin(); itor != ZGetObjectManager()->end(); ++itor)
	{
		// 타겟에 대한 정보를 얻는다.
		ZObject* pVictim = (*itor).second;
		if (!pVictim) {
			continue;
		}

		// 타켓이 죽은 상태면 다음 타겟으로 넘어간다.
		ZModule_HPAP* pModule = (ZModule_HPAP*)pVictim->GetModule(ZMID_HPAP);
		if (pVictim->IsDie()) {
			continue;
		}

		// 타겟이 공격자 자신이면 다음 타겟으로 넘어간다.
		if (pAttacker == pVictim) {
			continue;
		}

		if (pAttacker->GetTeamID() == pVictim->GetTeamID()) {
			continue;
		}

		// 타겟의 정보를 구한다.
		rvector VictimPos, VictimDir;
		if (!pVictim->GetHistory(&VictimPos, &VictimDir, fShotTime)) {
			continue;
		}

		// 공격자에 대한 타켓의 위치와 방향 정규화
		rvector VictimNorPos	= VictimPos;
		VictimNorPos.z			= 0;

		rvector VictimNorDir	= VictimPos - ( AttackerPos - ( AttackerNorDir * 50.f));
		VictimNorDir.z			= 0;
		Normalize( VictimNorDir);


		// 공격자와 타겟간의 x,y 평면 상의 거리를 구한 다음, 거리가 공격 가능 거리보다 멀면 다음 타겟으로 넘어간다.
		float fDist = Magnitude( AttackerNorPos - VictimNorPos);
		if (fDist > m_fRange) {
			continue;
		}

		// 타켓이 공격 가능한 각도 밖이면 다음 타겟으로 넘어간다.
		float fDot = D3DXVec3Dot( &AttackerNorDir, &VictimNorDir);
		if (fDot < fRadius) {
			continue;
		}

		// 타겟이 해당 공격 범위 안에서 일정 영역 높이에 있는지 확인한다.
		if (!pGame->InRanged(pAttacker, pVictim)) {
			continue;
		}

		// 공격자와 타겟 사이에 벽이 막고 있으면 다음 타겟으로 넘어간다.
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

				// 가드 이펙트 표시
				ZGetEffectManager()->AddSwordDefenceEffect(pos + (pVictim->m_Direction * 50.f), pVictim->m_Direction);
				pVictim->OnMeleeGuardSuccess();
				return;
			}

			// 노가드 상태에서 공격이 들어오면... 맞는 이펙트
			rvector pos = pVictim->GetPosition();
			pos.z += 130.f;
			pos -= AttackerDir * 50.f;
			// 이펙트를 출력한다.
			ZGetEffectManager()->AddBloodEffect(pos, -VictimNorDir);
			ZGetEffectManager()->AddSlashEffect(pos, -VictimNorDir, 0);

			pVictim->OnDamaged(pAttacker, pAttacker->GetPosition(), ZD_MELEE, MWT_DAGGER, m_fDamage, m_fPierce, 0);

			if (m_strSound.empty()) {
				ZGetSoundEngine()->PlaySoundBladeDamage(NULL, pos);	// 기본 쇳소리
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