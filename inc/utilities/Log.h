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

} // namespace syn