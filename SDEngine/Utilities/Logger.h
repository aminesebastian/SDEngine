#pragma once
#include "Utilities/TypeDefenitions.h"
#include "Include/FMT/core.h"
#include <iostream>

#define __JUST_FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define SD_LOG(Outer, CategoryName, Verbosity, Format, ...) \
	{ \
		TString adjustedFormat = "{0:<18} {1} {2}({3}): "; \
		TString message = fmt::format(adjustedFormat, Verbosity, CategoryName, __JUST_FILENAME__, __LINE__);\
		std::cout << fmt::format(message + Format, ##__VA_ARGS__)  << std::endl; \
	}

#define SD_ENGINE_DEBUG(Format, ...) \
	{ \
		SD_LOG(this, "\033[1;34mENGINE\033[0m", "\033[1;37m[DEBUG]\033[0m", Format, ##__VA_ARGS__) \
	}

#define SD_ENGINE_INFO(Format, ...) \
	{ \
		SD_LOG(this, "\033[1;34mENGINE\033[0m", "\033[1;32m[INFO]\033[0m", Format, ##__VA_ARGS__) \
	}

#define SD_ENGINE_WARN(Format, ...) \
	{ \
		SD_LOG(this, "\033[1;34mENGINE\033[0m", "\033[1;33m[WARNING]\033[0m", Format, ##__VA_ARGS__) \
	}

#define SD_ENGINE_ERROR(Format, ...) \
	{ \
		SD_LOG(this, "\033[1;34mENGINE\033[0m", "\033[1;31m[ERROR]\033[0m", Format, ##__VA_ARGS__) \
	}


class Logger {};

