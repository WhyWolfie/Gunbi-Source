#pragma once

class MNewQuestNpcObject
{
private:
	MUID m_uid;
	//MQUEST_NPC m_nType;
	MUID m_uidController;
	int m_nDropItemId;
	int m_nAp;
	int m_nHp;

public:
	MNewQuestNpcObject(MUID& uid) : m_uid(uid), m_uidController(0,0), m_nDropItemId(0), m_nAp(0), m_nHp(0) {}
	~MNewQuestNpcObject() {}

	MUID GetUID() { return m_uid; }

	// 이 npc를 조종할 플레이어
	void AssignControl(MUID& uidPlayer) { m_uidController=uidPlayer; }
	void ReleaseControl() { m_uidController.SetInvalid(); }
	MUID& GetController() { return m_uidController; }

	int GetDropItemId()			{ return m_nDropItemId; }
	void SetDropItemId(int id)	{ m_nDropItemId = id; }
	bool HasDropItem()			{ return m_nDropItemId != 0; }

	void SetAp(int i) { m_nAp = i; }
	void SetHp(int i) { m_nHp = i; }

	int GetAp() { return m_nAp; }
	int GetHp() { return m_nHp; }
};

class MNewQuestNpcManager
{
	typedef map<MUID, MNewQuestNpcObject*>	MapNpc;
	typedef MapNpc::iterator				ItorNpc;

	MapNpc m_mapNpc;

public:
	void AddNpcObject(MUID uidNpc, MUID uidController, int nDropItemId);
	void DeleteNpcObject(MUID uidNpc);

	int GetNumNpc() { return (int)m_mapNpc.size(); }
	MNewQuestNpcObject* GetNpc(MUID uid);
};