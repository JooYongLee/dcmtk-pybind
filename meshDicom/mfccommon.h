#pragma once

// 컴파일을 위해 몇가지 선언
#include <string>
#include <vector>



#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef AFX_EXT_CLASS
#define AFX_EXT_CLASS
#endif // 

#ifndef ASSERT
#define ASSERT(f)          //DEBUG_ONLY((void) ((f) || !::AfxAssertFailedLine(THIS_FILE, __LINE__) || (AfxDebugBreak(), 0)))
#endif //

typedef std::string CString;
//typedef CStringList std::vector<std::string>;