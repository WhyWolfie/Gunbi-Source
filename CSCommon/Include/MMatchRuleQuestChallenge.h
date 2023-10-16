#ifndef _MMATCHRULEQUESTCHALLENGE_H
#define _MMATCHRULEQUESTCHALLENGE_H

#include "MMatchRule.h"
#include "MActorDef.h"
#include "MNewQuestScenario.h"

class MNewQuestPlayerManager;
class MNewQuestNpcManager;

class MMatchRuleQuestChallenge : public MMatchRule
{
private:
	MNewQuestPlayerManager* m_pPlayerMgr;
	MNewQuestNpcManager* m_pNpcMgr;
	MNewQuestScenario* m_pScenario;

	int m_nCurrSector;		// ���� �������� ����

protected:
	virtual bool RoundCount();										///< ���� ī��Ʈ. ��� ���尡 ������ false�� ��ȯ�Ѵ�.
	virtual void OnBegin();											///< ��ü ���� ���۽� ȣ��
	virtual void OnEnd();											///< ��ü ���� ����� ȣ��
	virtual void OnRoundBegin();									///< ���� ������ �� ȣ��
	virtual void OnRoundEnd();										///< ���� ���� �� ȣ��
	virtual bool OnRun();											///< ����ƽ�� ȣ��
protected:
	virtual bool OnCheckRoundFinish();					

public:
	virtual MMATCH_GAMETYPE GetGameType() { return MMATCH_GAMETYPE_QUEST_CHALLENGE; }

public:
	MMatchRuleQuestChallenge(MMatchStage* pStage);
	virtual ~MMatchRuleQuestChallenge();

	virtual void OnCommand(MCommand* pCommand);

private:
	void RouteSpawnNpc(MUID uidNPC, MUID uidController, const char* szNpcDefName, int nCustomSpawnTypeIndex, int nSpawnIndex);
	void RouteNpcDead(MUID uidNPC, MUID uidKiller);
	void RouteMoveToNextSector();

	void ProcessNpcSpawning();
	void SpawnNpc(const char* szActorDef, int nCustomSpawnTypeIndex, int nSpawnIndex, int nDropItemId);
	void DropItemByNpcDead(const MUID& uidKiller, int nWorldItemId, const MVector& pos);
};

#endif