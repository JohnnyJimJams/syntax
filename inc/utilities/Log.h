#pragma once

#include <Syntax.h>

namespace syn
{

#ifdef _DEBUG

	void SYNTAX_API log(const char* a_format, ...);
	void SYNTAX_API logWarning(const char* a_format, ...);
	void SYNTAX_API logError(const char* a_format, ...);
	void SYNTAX_API logC(bool a_condition, const char* a_format, ...);
	void SYNTAX_API logWarningC(bool a_condition, const char* a_format, ...);
	void SYNTAX_API logErrorC(bool a_condition, const char* a_format, ...);

#else

	static void dummyLog(const char* a_format, ...) {}
	static void dummyLogC(bool a_condition, const char* a_format, ...) {}

	#define log				dummyLog
	#define	logWarning		dummyLog
	#define logError		dummyLog
	#define logC			dummyLogC
	#define	logWarningC		dummyLogC
	#define logErrorC		dummyLogC

#endif // _DEBUG

	void SYNTAX_API __stdcall logGLDebugCallback(unsigned int a_source,
		unsigned int a_type,
		unsigned int a_id,
		unsigned int a_severity,
		int a_length,
		const char* a_message,
		const void* a_userParam);

} // namespace syn