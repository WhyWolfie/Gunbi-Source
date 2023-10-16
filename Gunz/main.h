#ifndef _MAIN_H
#define _MAIN_H

#pragma warning(disable:4099)

#ifdef _DEVELOPER2
	#pragma comment(lib,"Cml_Developer.lib")
	#pragma comment(lib,"Mint2_Developer.lib")
	#pragma comment(lib,"Realsound_Developer.lib")
	#pragma comment(lib,"RealSpace2_Developer.lib")
	#pragma comment(lib,"CSCommon_Developer.lib")
	#pragma comment(lib,"SafeUDP_Developer.lib")
	#pragma comment(lib,"MCountryFilter_Developer.lib")
#elif _DEBUG
	#pragma comment(lib,"Cml_Debug.lib")
	#pragma comment(lib,"Mint2_Debug.lib")
	#pragma comment(lib,"RealSound_Debug.lib")
	#pragma comment(lib,"RealSpace2_Debug.lib")
	#pragma comment(lib,"CSCommon_Debug.lib")
	#pragma comment(lib,"SafeUDP_Debug.lib")
	#pragma comment(lib,"MCountryFilter_Debug.lib")
#else
	#pragma comment(lib,"Cml_Release.lib")
	#pragma comment(lib,"Mint2_Release.lib")
	#pragma comment(lib,"RealSound_Release.lib")
	#pragma comment(lib,"RealSpace2_Release.lib")
	#pragma comment(lib,"CSCommon_Release.lib")
	#pragma comment(lib,"SafeUDP_Release.lib")
	#pragma comment(lib,"MCountryFilter_Release.lib")
#endif

#pragma comment(lib,"DxErr9.lib")
#pragma comment(lib,"winmm.lib")

#if (_MSC_VER == 1900)
	#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif

#endif