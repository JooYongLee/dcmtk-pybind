#pragma once

#include "version.h"

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/IAppender.h>

namespace meshlibs {	;

inline bool log_initialized_message()
{
	LOGI.printf("\n"\
		"========================================================================================\n"\
		"====================  %s : %s===========================\n"\
		"========================================================================================\n"\
		, VER_FILE_DESCRIPTION_STR, VER_FILE_VERSION_STR);

	return true;
}

inline bool initialize_logger(plog::IAppender *pAppender, plog::Severity maxSeverity = plog::debug)
{
	if (plog::get() == nullptr)
	{
		plog::init(maxSeverity, pAppender);
		//log_initialized_message();
	}
	return true;
}

inline bool init_get_logger(const std::string &filename = "meshlibs.log", 
	size_t max_file_size = 1000000, int maxFiles = 3)
{
	if (plog::get() == nullptr)
	{
		//const std::string filename = "meshlibs.log";
		static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.
		static plog::ColorConsoleAppender<plog::TxtFormatter> colorConsoloAppender;


		static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(filename.c_str(), max_file_size, maxFiles); // Create the 1st appender.
		plog::init(plog::info, &fileAppender);

		plog::get()->addAppender(&colorConsoloAppender);
	}
	return true;
}

}