#include "stdafx.h"
#include "ZRule.h"
#include "ZMatch.h"
#include "ZRuleDeathMatch.h"
#include "ZRuleGladiator.h"
#include "ZRuleAssassinate.h"
#include "ZRuleTraining.h"
#include "ZRuleQuest.h"
#include "ZRuleSurvival.h"
#include "ZRuleQuestChallenge.h"
#include "ZRuleBerserker.h"
#include "ZRuleDuel.h"
#include "ZRuleDuelTournament.h"
#include "ZRuleDropGunGame.h"
#include "ZRuleBlitzKrieg.h"
#include "ZRuleWarMode.h"

ZRule::ZRule(ZMatch* pMatch)
{
	m_pMatch = pMatch;				// �ʱ�ȭ =_=
}

ZRule::~ZRule()
{

}

void ZRule::Update(float fDelta)
{
	OnUpdate(fDelta);
}

ZRule* ZRule::CreateRule(ZMatch* pMatch, MMATCH_GAMETYPE nGameType)
{
	switch(nGameType)
	{
	case MMATCH_GAMETYPE_DEATHMATCH_SOLO:
		{
			return (new ZRuleSoloDeathMatch(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_DEATHMATCH_TEAM:
		{
			return (new ZRuleTeamDeathMatch(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_GLADIATOR_SOLO:
		{
			return (new ZRuleSoloGladiator(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_GLADIATOR_TEAM:
		{
			return (new ZRuleTeamGladiator(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_ASSASSINATE:
		{
			return (new ZRuleAssassinate(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_TRAINING:
		{
			return (new ZRuleTraining(pMatch));
		}
		break;
#ifdef _QUEST
	case MMATCH_GAMETYPE_SURVIVAL:
		{
			return (new ZRuleSurvival(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_QUEST:
		{
			return (new ZRuleQuest(pMatch));
		}
		break;
#endif
	case MMATCH_GAMETYPE_QUEST_CHALLENGE:
		{
			return (new ZRuleQuestChallenge(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_BERSERKER:
		{
			return (new ZRuleBerserker(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_DEATHMATCH_TEAM2:
		{
			return (new ZRuleTeamDeathMatch2(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_DUEL:
		{
			return (new ZRuleDuel(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_DUELTOURNAMENT:
		{
			return (new ZRuleDuelTournament(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_DROPGUNGAME:
		{
			return (new ZRuleDropGunGame(pMatch));
		}
		break;

	case MMATCH_GAMETYPE_CTF:
		{
			return (new ZRuleTeamCTF(pMatch));
		}
		break;

	case MMATCH_GAMETYPE_CLASSICMODE:
		{
			return (new ZRuleSoloDeathMatch(pMatch)); // No Need for own rule
		}
		break;
	case MMATCH_GAMETYPE_SNIPERMODE:
		{
			return (new ZRuleTeamDeathMatch(pMatch)); // No Need for own rule
		}
		break;

	case MMATCH_GAMETYPE_CLASSICMODE_TEAM:
		{
			return (new ZRuleTeamDeathMatch(pMatch)); // No Need for own rule
		}
		break;

	case MMATCH_GAMETYPE_TRAINING_TEAM:
		{
			return (new ZRuleTeamDeathMatch(pMatch)); // No Need for own rule
		}
		break;

	case MMATCH_GAMETYPE_BLITZKRIEG: {
			return (new ZRuleBlitzKrieg(pMatch));
		}
		break;
	case MMATCH_GAMETYPE_WAR: {
			return (new ZRuleWarMode(pMatch));
		}
		break;
	default:
		{
			// ���� Ÿ���� �����ϴ�.
			_ASSERT(0);
		}
	}
	return NULL;

}

float ZRule::FilterDelayedCommand( MCommand* pCommand )
{
	return 0;
}

bool ZRule::OnCommand(MCommand* pCommand)
{
	return false;
}

void ZRule::AfterCommandProcessed( MCommand* pCommand )
{

}

void ZRule::OnResponseRuleInfo(MTD_RuleInfo* pInfo)
{

}