#ifndef _ZRULE_QUESTCHALLENGE_H
#define _ZRULE_QUESTCHALLENGE_H

#include "ZRule.h"

class MNewQuestScenario;


class ZRuleQuestChallenge : public ZRule
{
private:
	MNewQuestScenario* m_pScenario;	// �� ����Ʈ�� �ó�����
	int m_nCurrSector;				// ���� �������� ���� ��ȣ

public:
	ZRuleQuestChallenge(ZMatch* pMatch);
	virtual ~ZRuleQuestChallenge();

public:
	static bool LoadScenarioMap(const char* szScenarioName);

	virtual bool Init();
	virtual void OnUpdate(float fDelta);
	virtual void OnSetRoundState(MMATCH_ROUNDSTATE roundState);

	virtual bool OnCommand(MCommand* pCommand);

	virtual int GetRoundMax();
	virtual int GetCurrRound();

	void SpawnActor(const char* szActorDefName, MUID& uid, int nCustomSpawnTypeIndex, int nSpawnIndex, bool bMyControl);
	void MoveToNextSector();
};



#endif