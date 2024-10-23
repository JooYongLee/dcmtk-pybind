// dllmain.cpp: DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"

#include "DICOMINIT.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DICOMINIT::InitializeDICOM();
		break;
	case DLL_THREAD_ATTACH:
		//DICOMINIT::InitializeDICOM();
		break;
	case DLL_THREAD_DETACH:
		//DICOMINIT::ReleaseDICOM();
		break;
	case DLL_PROCESS_DETACH:
		DICOMINIT::ReleaseDICOM();
		
		break;
	}
	return TRUE;
}

