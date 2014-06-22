#pragma once

#define SYNTAX_VERSION 0x00010000L

#define _CRT_SECURE_NO_WARNINGS

// Disable Warnings
#pragma warning ( disable : 4996 )	// This function or variable may be unsafe. Consider using 'function' instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning ( disable : 4251 )	// Template warning when building DLL

#ifndef SYNTAX_API
	#if defined(SYNTAX_API_EXPORT)
		#define SYNTAX_API __declspec(dllexport)
	#elif defined(SYNTAX_API_IMPORT)
		#define SYNTAX_API __declspec(dllimport)
	#else
		#define SYNTAX_API
	#endif
#endif

#define SAFEDELETE(ptr)			if ((ptr)!=nullptr) { delete (ptr); (ptr) = nullptr; }
#define SAFEDELETEARRAY(ptr)	if ((ptr)!=nullptr) { delete[] (ptr); (ptr) = nullptr; }
#define SAFERELEASE(ptr)		if ((ptr)!=nullptr) { (ptr)->Release(); (ptr) = nullptr; }
#define OFFSETOF(s,m)			offsetof(s,m)
#define STRINGIFY(str)			#str
