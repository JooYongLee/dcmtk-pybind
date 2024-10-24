#include "SplitPath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//
namespace meshlibs {;
CSplitPath::CSplitPath(const std::string &strPath)
	: path(strPath)
{
	this->Split(strPath);
}

bool CSplitPath::Split(const std::string &strPath)
{
	this->path = strPath;
	//char path_buffer[MAX_PATH];
	char cdrive[MAX_PATH];
	char cdir[MAX_PATH];
	char cfname[MAX_PATH];
	char cext[MAX_PATH];

	_splitpath(strPath.c_str(), cdrive, cdir, cfname, cext);


	this->drive = cdrive;
	this->dir = cdir;
	this->fname = cfname;
	this->ext = cext;
	this->dirname= this->drive + this->dir;
}

}