#pragma once

class MBitmapR2;
#include "Mint4r2.h"
#include "MThread.h"

enum LOADING_SCENE_MODE
{
	MODE_DEFAULT,
	MODE_FADEOUT,
	MODE_FADEIN,				
	MODE_BLENDING,
};

enum LOADING_DRAW_MODE
{
	MODE_BAR,
	MODE_FULLSCREEN,
};

#define GRADE_TIMER		5000

class ZInitialLoading
{
protected:
	MBitmapR2*	mBitmaps;
	MBitmapR2*	mBitmap_Bar;
	MBitmapR2*	mBitmap_Grade;		// ���ӹ� ��� ����ȸ �̹���
	DWORD		m_Grade_Time;
	bool		m_bGradeFadeOutStart;
	bool		m_bGrade;
	int			miNumScene;
	int			miCurrScene;
	bool		mbUseEnable;
	bool		mbText;
	bool		mbBitmap;
	bool		mbBitmapBar;
	float		mx, my, mw, mh, msx, msy, msw, msh;
	float		mtx, mty, mtw, mth;
	MFontR2*	mpDC;
	char*		mpStr;
	int			m_nTipNum;
	static ZInitialLoading	msInstance;
	float		mPercentage;

	bool		m_bTipsVisible;
	char		m_szTips[256];
	char		m_szNotice[ 256];
	const char*	m_pLoadingStr;

public:
	#ifdef _WHOLE_SCREEN_LOADING
	static LOADING_DRAW_MODE drawMode;
	#endif

protected:
	void	DrawTips();
	void	SetRandomTipsText();
	void	DrawDuelTournamentCharacterList();
public:
	void	Draw( LOADING_SCENE_MODE mode_ = MODE_DEFAULT, int destIndex_ = -1, bool bflip_ = true, bool fromThread_ = false );
	void	DrawBar( float x_, float y_, float w_, float h_, float percent_ );
	void	DrawGrade(float blendFactor); // ���ӹ� ��� ����ȸ �̹���
	void	DrawText();
	void	SetText( MFontR2* pDc, float x, float y, char* str );
	void	SetTipNum( int nTipNum)		{ m_nTipNum = nTipNum;	}
	bool	Initialize( int numScene_, float x_, float y_, float w_, float h_, 	float sx_, float sy_, float sw_, float sh_, bool bTipsVisible=false);
	bool	AddBitmap( int index_, const char* bitmapName_ );
	bool	AddBitmapBar( const char* bitmapName_ );
	bool	AddBitmapGrade( const char* bitmapName_ ); // ���ӹ� ��� ����ȸ �̹���
	bool	IsUseEnable() const	{		return mbUseEnable;	}
	bool	isText() const	{		return mbText;				}
	void	SetPercentage( float p_ ) 	{		mPercentage = p_;	}
	void	SetLoadingStr(const char *pStr) { m_pLoadingStr = pStr; }
	void	SetDuelTournamentCharacterList(char szCharacterNameList[8][32]);

	void	Release();

	
	static ZInitialLoading*		GetInstance()
	{
		return &msInstance;
	}

public:
	ZInitialLoading();
    ~ZInitialLoading();
};

ZInitialLoading*	ZGetInitialLoading();


class ZLoadingProgress {
	char	m_szName[64];			// �̸�
	ZLoadingProgress *m_pParent;
	float m_fTotalProgressStart;	// ���� progress �������� (��ü����)
	float m_fProgressStart;			// progress �������� (�θ����)
	float m_fThisAmount;			// ��ü�߿� ���� ������ �����ϴ� ����
	float m_fCurrentProgress;		// ���� ������
	
#ifdef _DEBUG
	static float ZLoadingProgress::m_fLastProgress;	// �ǵ��ư��°�찡 �ֳ� üũ
#endif

public:
	ZLoadingProgress(const char *szName,ZLoadingProgress *pParent=NULL,float fRatio=1.f);

	void Update(float fProgress);
	void Draw();
	void UpdateAndDraw(float fProgress) {
		Update(fProgress);
		Draw();
	}
};