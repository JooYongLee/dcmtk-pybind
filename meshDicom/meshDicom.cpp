// meshDicom.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "meshDicom.h"
//#include "dicominlines.h"
#include "meshlibs_logger.h"

// 내보낸 변수의 예제입니다.

namespace meshlibs 
{	;
	namespace meshDicom
	{
		MESHDICOM_API bool initLogger(plog::IAppender* pAppender, const plog::Severity& maxSeverity)
		{
			if (plog::get() == nullptr)
			{
				initialize_logger(pAppender, maxSeverity);
			}
			return true;
		}
	}
}
