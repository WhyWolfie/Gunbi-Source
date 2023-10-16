#pragma once


class ZActorActionMeleeShot;
class ZActorActionRangeShot;
class ZActorActionGrenadeShot;
class ZActorActionEffect;
class ZActorActionSound;


class IActorAction
{
public:
	virtual ~IActorAction() {}

	virtual const char* GetName() = 0;
	virtual const char* GetAnimationName() = 0;
	virtual bool IsMovingAnimation() = 0;
	virtual const ZActorActionMeleeShot* GetMeleeShot() = 0;
	virtual const ZActorActionRangeShot* GetRangeShot(int idx) = 0;
	virtual const ZActorActionGrenadeShot* GetGrenadeShot(int idx) = 0;
	virtual const ZActorActionEffect* GetEffect(int idx) = 0;
	virtual const ZActorActionSound* GetSound(int idx) = 0;
	virtual int GetNumEffect() = 0;
	virtual int GetNumGrenadeShot() = 0;
	virtual int GetNumRangeShot() = 0;
	virtual int GetNumSound() = 0;
};

// �׼��� ������ npc�� Ư���ൿ�� �����ϱ� ���� ����ߴ� ZSkill�� ����ϴ�.
// �׷��� �׼��� �� ���ݽ�ų�� �ʿ�� ������, � �ִϸ��̼��� �ܼ��� ����ϴ� �뵵�� ���� ���� �ִ�.
// ���ݽ�ų�� ���� ZSkill���� �ٸ��� ���������� �׼� ���� ���� �������� ������ ���������� ���� ��ü�� 
// ���忡 �����ϱ� ���� �������� ���� �ȴ�.


class ZActorAction : public IActorAction
{
	string m_strName;
	string m_strAnimation;
	bool m_bMovingAnimation;

	ZActorActionMeleeShot* m_pMeleeShot;

	typedef vector<ZActorActionRangeShot*>	VecRangeShot;
	typedef VecRangeShot::iterator			ItorRangeShot;
	VecRangeShot m_vecRangeShot;

	typedef vector<ZActorActionGrenadeShot*> VecGrenadeShot;
	typedef VecGrenadeShot::iterator		 ItorGrenadeShot;
	VecGrenadeShot m_vecGrenadeShot;

	typedef vector<ZActorActionEffect*>	VecEffect;
	typedef VecEffect::iterator			ItorEffect;
	VecEffect m_vecEffect;

	typedef vector<ZActorActionSound*>	VecSound;
	typedef VecSound::iterator			ItorSound;
	VecSound m_vecSound;

public:
	ZActorAction() 
		: m_pMeleeShot(0)
		, m_bMovingAnimation(false) {}
	virtual ~ZActorAction();

	void SetName(const char* sz) { m_strName=sz; }
	virtual const char* GetName() { return m_strName.c_str(); }

	void SetAnimationName(const char* sz) { m_strAnimation=sz; }
	virtual const char* GetAnimationName() { return m_strAnimation.c_str(); }

	void SetMovingAnimation(bool b) { m_bMovingAnimation = b; }
	virtual bool IsMovingAnimation() { return m_bMovingAnimation; }

	void SetMeleeShot(ZActorActionMeleeShot* p) { m_pMeleeShot = p; }
	virtual const ZActorActionMeleeShot* GetMeleeShot() { return m_pMeleeShot; }

	void AddRangeShot(ZActorActionRangeShot* p) { m_vecRangeShot.push_back(p); }
	virtual const ZActorActionRangeShot* GetRangeShot(int idx) { 
		if (idx < 0 || GetNumRangeShot() <= idx) { _ASSERT(0); return NULL; }
		return m_vecRangeShot[idx];
	}

	void AddGrenadeShot(ZActorActionGrenadeShot* p) { m_vecGrenadeShot.push_back(p); }
	virtual const ZActorActionGrenadeShot* GetGrenadeShot(int idx) { 
		if (idx < 0 || GetNumGrenadeShot() <= idx) { _ASSERT(0); return NULL; }
		return m_vecGrenadeShot[idx];
	}

	void AddEffect(ZActorActionEffect* p) { m_vecEffect.push_back(p); }
	virtual const ZActorActionEffect* GetEffect(int idx) { 
		if (idx < 0 || GetNumEffect() <= idx) { _ASSERT(0); return NULL; }
		return m_vecEffect[idx];
	}

	void AddSound(ZActorActionSound* p) { m_vecSound.push_back(p); }
	virtual const ZActorActionSound* GetSound(int idx) { 
		if (idx < 0 || GetNumSound() <= idx) { _ASSERT(0); return NULL; }
		return m_vecSound[idx];
	}

	virtual int GetNumRangeShot() { return (int)m_vecRangeShot.size(); }
	virtual int GetNumGrenadeShot() { return (int)m_vecGrenadeShot.size(); }
	virtual int GetNumEffect() { return (int)m_vecEffect.size(); }
	virtual int GetNumSound() { return (int)m_vecSound.size(); }
};

// �и����� ���������� ���� �Ӽ� 
class ZActorActionShotCommon
{
protected:
	float m_fDelay;		// �׼��� ������ �� �� �����̸�ŭ ��ٷȴٰ� ���� �߻�
	float m_fDamage;
	float m_fPierce;	// �����
	string m_strSound;	// ��Ʈ ����(�������� ���� ��� �⺻ ���� �߻�)
	float m_fKnockbackForce;	// ��Ʈ�� �˹� ��
	bool m_bUpperCut;
	bool m_bThrust;

public:
	ZActorActionShotCommon()
		: m_fDelay(0)
		, m_fDamage(0)
		, m_fPierce(0.5f)
		, m_fKnockbackForce(0)
		, m_bUpperCut(false)
		, m_bThrust(false) {}
	virtual ~ZActorActionShotCommon() {}

	float GetDelay() const { return m_fDelay; }
	float GetDamage() const { return m_fDamage; }
	float GetPierce() const { return m_fPierce; }
	const char* GetSound() const { return m_strSound.c_str(); }
	float GetKnockbackForce() const { return m_fKnockbackForce; }
	bool GetUpperCut() const { return m_bUpperCut; }
	bool GetThrust() const { return m_bThrust; }

	void SetDelay(float f) { m_fDelay = f; }
	void SetDamage(float f) { m_fDamage = f; }
	void SetPierce(float f) { m_fPierce = f; }
	void SetSound(const char* sz) { m_strSound = sz; }
	void SetKnockbackForce(float f) { m_fKnockbackForce = f; }
	void SetUpperCut(bool b) { m_bUpperCut = b; }
	void SetThrust(bool b) { m_bThrust = b; }
};

class ZActorActionMeleeShot : public ZActorActionShotCommon
{
	float m_fRange;		// ���� �ִ� �Ÿ�
	float m_fAngle;		// ���� �ݰ� radian

public:
	ZActorActionMeleeShot()
		: m_fRange(0)
		, m_fAngle(0) {}
	
	float GetRange() const { return m_fRange; }
	float GetAngle() const { return m_fAngle; }
	
	void SetRange(float f) { m_fRange = f; }
	void SetAngle(float f) { m_fAngle = f; }

	void ProcessShot(IGame* pGame, const MUID& uidOwner, float fShotTime) const;
};

class ZActorActionRangeShot : public ZActorActionShotCommon
{
	string m_strMeshName;
	_RMeshPartsPosInfoType m_posPartsType;	// źȯ�� �߻���ġ
	float m_fSpeed;
	float m_fCollRadius;	//todok �̰��� ������(���÷���) �϶��� ����Ѵ�. ���߿� ���� �ʿ�
	rvector m_vDirectionMod;	// �ڽ��� ���� ���� �������� �߻� ���� ������

public:
	ZActorActionRangeShot()
		: m_fSpeed(100.f)
		, m_fCollRadius(100.f)
		, m_vDirectionMod(0,0,0)
		{}

	const char* GetMeshName() const  { return m_strMeshName.c_str(); }
	_RMeshPartsPosInfoType GetPosPartsType() const { return m_posPartsType; }
	float GetSpeed() const { return m_fSpeed; }
	float GetCollRadius() const  { return m_fCollRadius; }
	const rvector& GetDirectionMod() const { return m_vDirectionMod; }

	void SetMeshName(const char* sz) { m_strMeshName = sz; }
	void SetPosPartsType(_RMeshPartsPosInfoType t) { m_posPartsType = t; }
	void SetSpeed(float f) { m_fSpeed = f; }
	void SetCollRadius(float f) { m_fCollRadius = f; }
	void SetDirectionMod(const rvector& v) { m_vDirectionMod = v; }

	void ProcessShot(const rvector& pos, const rvector& dir, ZObject* pOwner) const;
};

class ZActorActionGrenadeShot : public ZActorActionShotCommon
{
protected:
	float m_fDelay;
	float m_fDamage;
	float m_fPierce;
	int m_nGrenadeType;
	int m_nItemID;
	float m_fZAxis;
	float m_fYAxis;
	float m_fForce;
	_RMeshPartsPosInfoType m_posPartsType;
	rvector m_vPosMod;
	rvector m_vDirMod;
	string m_strSound;
public:
	void SetGrenadeType(int i) {m_nGrenadeType = i;}
	void SetItemID(int i) {m_nItemID = i;}
	void SetZAxis(int i) {m_fZAxis = i;}
	void SetYAxis(float f) {m_fYAxis = f;}
	void SetForce(float f) { m_fForce = f;}
	void SetPosPartsType(_RMeshPartsPosInfoType t) { m_posPartsType = t; }
	void SetPositionMod(const rvector& v) { m_vPosMod = v; }
	void SetDirectionMod(const rvector& v) { m_vDirMod = v; }

	int GetGrenadeType() { return m_nGrenadeType; }
	int GetItemID() const { return m_nItemID; }
	float GetYAxis() const { return m_fYAxis; }
	float GetZAxis() const { return m_fZAxis; }
	float GetForce() const { return m_fForce; }
	const rvector& GetPosMod() const { return m_vPosMod; }
	const rvector& GetDirMod() const { return m_vDirMod; }
	_RMeshPartsPosInfoType GetPosPartsType() const { return m_posPartsType; }


	void ProcessShot(const rvector& pos, const rvector& dir, ZObject* pOwner) const;
};

class ZActorActionEffect
{
protected:
	string m_strMeshName;
	_RMeshPartsPosInfoType m_posPartsType;	// źȯ�� �߻���ġ
	rvector m_vDirMod;	// �ڽ��� ���� ���� �������� �߻� ���� ������
	rvector m_vPosMod;	// �ڽ��� ���� ���� �������� �߻� ���� ������
	rvector m_vScale;
	float m_fDelay;		// �׼��� ������ �� �� �����̸�ŭ ��ٷȴٰ� ���� �߻�

public:
	ZActorActionEffect()
		: m_fDelay(100.f)
		, m_vDirMod(0,0,0)
		, m_vPosMod(0,0,0)
		, m_vScale(1.f,1.f,1.f)
		{}
	const char* GetMeshName() const  { return m_strMeshName.c_str(); }
	float GetDelay() const { return m_fDelay; }
	const rvector& GetScale() const {return m_vScale;}
	_RMeshPartsPosInfoType GetPosPartsType() const { return m_posPartsType; }
	const rvector& GetDirMod() const { return m_vDirMod; }
	const rvector& GetPosMod() const { return m_vPosMod; }

	void SetScale(rvector v) { m_vScale = v; }
	void SetMeshName(const char* sz) { m_strMeshName = sz; }
	void SetPosPartsType(_RMeshPartsPosInfoType t) { m_posPartsType = t; }
	void SetDirMod(const rvector& v) { m_vDirMod = v; }
	void SetPosMod(const rvector& v) { m_vPosMod = v; }
	void SetDelay(float f) { m_fDelay = f; }
	void ProcessShot(const rvector& pos, const rvector& dir, const MUID& uidOwner) const;
};

class ZActorActionSound
{
protected:
	string m_strSoundPath;
	float m_fDelay;

public:
	ZActorActionSound() 
		: m_fDelay(100.f)
		{}
	const char* GetSoundPath() const { return m_strSoundPath.c_str(); }
	float GetDelay() const { return m_fDelay; }

	void SetSoundPath(const char* sz) { m_strSoundPath = sz; }
	void SetDelay(float f) { m_fDelay = f; }

	void ProcessSound(string m_strSoundPath) const;
};