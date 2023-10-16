#pragma once

#include "MBlitzKriegDef.h"
#include "MMatchBlitzKrieg.h"
#include "MMatchRule.h"

class MNewQuestPlayerManager;
class MNewQuestNpcManager;

class MMatchRuleBlitzKrieg : public MMatchRule {
private:
	MNewQuestPlayerManager* m_pPlayerMgr;
	MNewQuestNpcManager*	m_pNpcMgr;

protected:
	bool GetAliveCount(int* pRedAliveCount, int* pBlueAliveCount);
	virtual void OnBegin();
	virtual void OnEnd();
	virtual bool OnRun();
	virtual void OnRoundBegin();
	virtual void OnRoundEnd();
	virtual bool OnCheckRoundFinish();
	virtual void OnRoundTimeOut();
	virtual bool RoundCount();
	virtual bool OnCheckEnableBattleCondition();

private:
	void NPCSpawn();
	void SpawnActor(const char* szActorName, MMatchTeam nTeam, int nPosIndex);

public:
	MMatchRuleBlitzKrieg(MMatchStage* pStage);
	virtual ~MMatchRuleBlitzKrieg() {}
	virtual void CalcTeamBonus(MMatchObject* pAttacker, MMatchObject* pVictim, int nSrcExp, int* poutAttackerExp, int* poutTeamExp);
	virtual MMATCH_GAMETYPE GetGameType() { return MMATCH_GAMETYPE_BLITZKRIEG; }
};