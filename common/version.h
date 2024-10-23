#pragma once

// http://ramu492.blogspot.com/2014/03/auto-increment-build-numbers-in-visual.html
#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_VERSION			 1
#define VERSION_MAJOR            1
#define VERSION_MINOR            0
#define VERSION_BUILD         295		// same with revision of "SVN"


#define VER_FILE_DESCRIPTION_STR "DIO AI TEAM-meshlibs"
#define VER_FILE_VERSION         VERSION_VERSION, VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD
#define VER_FILE_VERSION_STR     STRINGIZE(VERSION_VERSION)     \
                                    "." STRINGIZE(VERSION_MAJOR) \
                                    "." STRINGIZE(VERSION_MINOR) \
                                    "." STRINGIZE(VERSION_BUILD) \

#define VER_PRODUCTNAME_STR      "DIO AI TEAM mesh-library"
#define VER_PRODUCT_VERSION      VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR  VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR    VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR        "Copyright DIO(C) 2021"

#ifdef _DEBUG
#define VER_VER_DEBUG          VS_FF_DEBUG
#else
#define VER_VER_DEBUG          0
#endif

#define VER_FILEOS               VOS_NT_WINDOWS32
#define VER_FILEFLAGS            VER_VER_DEBUG
#define VER_FILETYPE             VFT_APP
