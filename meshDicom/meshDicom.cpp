// meshDicom.cpp : DLL ���� ���α׷��� ���� ������ �Լ��� �����մϴ�.
//

#include "stdafx.h"
#include "meshDicom.h"
//#include "dicominlines.h"
#include "meshlibs_logger.h"

// ������ ������ �����Դϴ�.

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
