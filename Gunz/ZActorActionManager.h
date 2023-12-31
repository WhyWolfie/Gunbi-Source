#pragma once

class IActorAction;
class ZActorAction;
class ZActorActionShotCommon;
class ZActorActionMeleeShot;
class ZActorActionRangeShot;
class ZActorActionEffect;
class ZActorActionSound;

class ZActorActionManager
{
	typedef map<string, ZActorAction*>	MapAction;
	typedef MapAction::iterator			ItorAction;
	MapAction m_mapAction;

public:
	virtual ~ZActorActionManager();

	static ZActorActionManager* GetInstance();

	bool ReadXml(MZFileSystem* pFileSystem, const char* szFileName);
	bool ParseXmlFromStr(const char* szXmlContent);
	bool ParseXml_Action(ZActorAction* pOutAction, MXmlElement& elem);
	bool ParseXml_ActionShotCommonAttributes(ZActorActionShotCommon* pOutShot, MXmlElement& elem);
	bool ParseXml_ActionMeleeShot(ZActorActionMeleeShot* pOutMeleeShot, MXmlElement& elem);
	bool ParseXml_ActionRangeShot(ZActorActionRangeShot* pOutRangeShot, MXmlElement& elem);
	bool ParseXml_ActionGrenadeShot(ZActorActionGrenadeShot* pOutGrenadeShot, MXmlElement& elem);
	bool ParseXml_ActionEffect(ZActorActionEffect* pOutEffect, MXmlElement& elem);
	bool ParseXml_ActionSound(ZActorActionSound* pOutEffect, MXmlElement& elem);

	bool AddAction(ZActorAction* pAction);
	int GetCount() { return (int)m_mapAction.size(); }
	void Clear();

	virtual ZActorAction* GetAction(const char* szName);

	int GetIndexOfAction(IActorAction* pAction);		// 매니저 내에서 액션의 번호를 얻는다
	ZActorAction* GetActionByIndex(int idx);			// 액션 번호로 액션을 찾는다
};
inline ZActorActionManager* ZGetActorActionMgr() { return ZActorActionManager::GetInstance(); }