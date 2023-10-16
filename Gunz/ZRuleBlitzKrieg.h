#pragma once

#include "ZRule.h"

class ZRuleBlitzKrieg : public ZRule
{
private:
	ZMapSpawnData* DataRadar(MMatchTeam nTeamId);
	ZMapSpawnData* DataBarricade(MMatchTeam nTeamId, int nPosIndex);
	ZMapSpawnData* DataGuardian(MMatchTeam nTeamId);

private:
	void SpawnActor(MCommand* pCommand);

public:
	ZRuleBlitzKrieg(ZMatch* pMatch);
	virtual ~ZRuleBlitzKrieg();

	virtual bool OnCommand(MCommand* pCommand);
};