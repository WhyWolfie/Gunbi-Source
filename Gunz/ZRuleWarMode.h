#pragma once

#include "ZRuleDeathMatch.h"

class MCommand;
class ZDummyNPC;

class ZRuleWarMode : public ZRuleTeamDeathMatch2
{
private:
	map<MMatchTeam, MUID> m_vTowers;

public:
	ZRuleWarMode(ZMatch* pMatch);
	virtual ~ZRuleWarMode();

	virtual bool OnCommand(MCommand* pCommand);

	ZDummyNPC* GetTower(MMatchTeam nTeamId);
};