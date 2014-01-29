// Copyright (c) 2010 Jeroen Dierckx - Expertise Centre for Digital Media. All Rights reserved.
// Part of this source code is developed for the flemish (Belgian) OSMA project (http://osma.phl.be/)
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UNITYCHROMIUMPLUGIN_GLOBAL_H
#define UNITYCHROMIUMPLUGIN_GLOBAL_H

//#define _ITERATOR_DEBUG_LEVEL 2//deal with MSVC stupidity
//#define _DEBUG


// Windows and pocket pc
#if defined(_WIN32_WCE) || defined(WIN32)
	// Identifier was truncated to '255' characters in the debug information
	#pragma warning (disable: 4786)

	// STL classes are not exported in the dll
	/*!
		@note This can be ignored, since none of the stl members are public,
		so they shouldn't be exported in the dll
	*/
	#pragma warning (disable: 4251)

	#define WIN32_LEAN_AND_MEAN
	#define _CRT_SECURE_NO_WARNINGS
	#include <windows.h>

	// Windows plugin defines
	#define PLUGIN_API extern "C" __declspec(dllexport)

// Linux, Mac OS, ...
#else
	// Make compatible with the windows function
	#define stricmp strcasecmp

	// Plugin defines
	#define PLUGIN_API extern "C"
#endif


#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::wcout;
using std::wcerr;

#include <cassert>



#include "include/cef_task.h"

#if defined(OS_WIN)

#include <windows.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) if (!(condition)) { DebugBreak(); }

#else
#define ASSERT(condition) ((void)0)
#endif

#else  // !OS_WIN

#include <assert.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) if (!(condition)) { assert(false); }
#else
#define ASSERT(condition) ((void)0)
#endif

#endif  // !OS_WIN

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));




#endif // UNITYCHROMIUMPLUGIN_GLOBAL_H
