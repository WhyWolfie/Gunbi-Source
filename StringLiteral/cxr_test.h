/////////////////////////////////////////////////////////////
// cxr_test.h
//
// This file was generated by CXR, the literal string encryptor.
// CXR, Copyright 2002, Smaller Animals Software, Inc., all rights reserved.
//
// Please do not edit this file. Any changes here will be overwritten on the next compile.
// If you wish to make changes to a string, please edit:
//     test.cxr
//

/////////////////////////////////////////////////////////////

#pragma once
#include "cxr_inc.h"

// cxr 적용 테스트입니당

///////////////////////////
#ifdef _USING_CXR
#define STR_CXRTEST1 	_CXR("\x8a\x86\x89\x80\x8e\x8b\x8b\x8f\x83\x84\x84\x83\x8f\x87\x89\x80\x83\x85\x8b\x89\x83\x88\x80\x85\x80\x81\x8d\x86\x86\x8d\x89\x89\x8c\x85\x88\x89\x87\x8c\x87\x8c\x81\x8e\x87\x8a\x8e\x81\x8d\x85\x87\x8e\x82\x87\x8d\x84\x8f\x8d\x87\x80\x80\x84\x89\x80\x87\x84\x89\x85")
#else
#define STR_CXRTEST1 _CXR("Your all bases are belong to us.")
#endif

///////////////////////////
#ifdef _USING_CXR
#define STR_CXRTEST2 	_CXR("\x89\x8d\x8e\x8f\x83\x8d\x8f\x84\x89\x81\x88\x8a\x8e\x84\x8d\x84\x87\x8e\x8f\x83")// 디파인 뒤에 주석이 달린 경우를 테스트
#else
#define STR_CXRTEST2 _CXR("It's You!")// 디파인 뒤에 주석이 달린 경우를 테스트
#endif

///////////////////////////
#ifdef _USING_CXR
#define STR_CXRTEST3 	_CXR("\x8a\x84\x85\x8f\x89\x8f\x80\x8d\x88\x83")		// 디파인 뒤에 공백을 두고 주석이 달린 경우를 테스트
#else
#define STR_CXRTEST3 _CXR("Aye.")		// 디파인 뒤에 공백을 두고 주석이 달린 경우를 테스트
#endif


