// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once
#define POINTER_64 __ptr64

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션
#include <afxdisp.h>        // MFC 자동화 클래스

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxrich.h>
#include <afxcview.h>

#include "winsock2.h"
#include "windows.h"
#include <comutil.h>
#include <stdio.h>

#include "MBMatchServer.h"

#define _QUEST_ITEM
#define _MONSTER_BIBLE 
#define _MONITORING

#include "MFeatureDefine.h"


//#define _GAMBLEITEM_TEST_LOG	// 겜블 아이템이 문제있을때 확인을 위해(평소에는 주석처리)