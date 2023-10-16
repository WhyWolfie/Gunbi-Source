#pragma once

#include "MMatchRule.h"
#include "MMatchRuleDeathMatch.h"

class MNewQuestPlayerManager;
class MNewQuestNpcManager;

///////////////////////////////////////////////////////////////////////////////////////////////
class MMatchRuleWar : public MMatchRuleTeamDeath2
{
private:
	MNewQuestPlayerManager* m_pPlayerMgr;
	MNewQuestNpcManager*	m_pNpcMgr;

	map<MMatchTeam, MUID>	m_vTowers;
	MMatchTeam				m_nLoseTeam;

private:
	void SpawnTower(MUID& uidChar, MMatchTeam nTeam);

public:
	MMatchRuleWar(MMatchStage* pStage);
	virtual ~MMatchRuleWar()
	{

	}

	virtual MMATCH_GAMETYPE GetGameType()
	{
		return MMATCH_GAMETYPE_WAR;
	}

	void OnRoundBegin();
	void OnRoundEnd();

	bool OnCheckRoundFinish();

	virtual void OnEnterBattle(MUID& uidChar) override;
	virtual void OnLeaveBattle(MUID& uidChar) override;

	void RequestNPCDead(MUID uidAttacker, MUID uidNPC);
	void UpdateNPCHpAp(MUID uidOwner, MUID uidNpc, int nHP, int nAP);
};