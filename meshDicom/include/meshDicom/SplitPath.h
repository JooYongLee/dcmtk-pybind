
#ifndef __SPLITPATH_H__
#define __SPLITPATH_H__
// https://www.codeproject.com/Articles/316/CSplitPath

//#include "common_def.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

namespace meshlibs { ;
/*
 * @brief 경로 분리
 * @example
 * @code
 * CSplitPath sp( "C:\\Temp\\Foo.txt" );
 * cout << "Drive: " << sp.GetDrive()
 * cout << "Directory: " << sp.GetDirectory();
 * cout << "File Name: " << sp.GetFileName();
 * cout << "Extension: " << sp.GetExtension();
 * @endcode
 */
class CSplitPath
{
	// Construction
public:
	CSplitPath();
	CSplitPath(const std::string &strPath);

	// Operations
	bool    Split(const std::string &strPath);

	std::string path;
	std::string drive;
	std::string dir;
	std::string fname;
	std::string ext;
	std::string dirname;
};

}
#endif  // __SPLITPATH_H__