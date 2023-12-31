#ifndef _ZSCREENDEBUGGER_H
#define _ZSCREENDEBUGGER_H


class MDrawContext;

class ZScreenDebugger
{
private:
	int		m_nShowDebugInfo;
	int		m_nY;

	void OnDrawAIDebugInfo(MDrawContext *pDC);
	void DrawActorInfo(int num, MDrawContext *pDC, ZActorBase* pActor);
	void PrintText(MDrawContext* pDC, const char* buffer);
public:
	ZScreenDebugger();
	~ZScreenDebugger();
	void DrawDebugInfo(MDrawContext *pDC);
	void SwitchDebugInfo();
	void DeveloperDebugInfo(MDrawContext *pDC);

	bool IsVisible()	{ return (m_nShowDebugInfo != 0); }
};




#endif