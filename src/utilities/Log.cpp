#pragma once

#include <utilities/Log.h>
#include <gl_core_4_4.h>
#include <windows.h>
#include <string>

namespace syn
{

#ifdef _DEBUG

#define	TEMP_DBG_STRING_SIZE	4096
char	g_tempDebugString[TEMP_DBG_STRING_SIZE];

void syn::log(const char* a_format, ...)
{	
	va_list argList;

	va_start(argList, a_format);

	_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
	OutputDebugStringA(g_tempDebugString);
	printf("%s", g_tempDebugString);

	va_end(argList);
}

void syn::logWarning(const char* a_format, ...)
{	
	HANDLE consoleHandle;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (INVALID_HANDLE_VALUE != consoleHandle)
	{
		SetConsoleTextAttribute(consoleHandle,FOREGROUND_GREEN);
	}

	va_list argList;

	va_start(argList, a_format);

	_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
	OutputDebugStringA(g_tempDebugString);
	printf("%s", g_tempDebugString);

	va_end(argList);

	if (INVALID_HANDLE_VALUE != consoleHandle)
	{
		SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	}
}

void syn::logError(const char* a_format, ...)
{	
	HANDLE consoleHandle;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (INVALID_HANDLE_VALUE != consoleHandle)
	{
		SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED);
	}

	va_list argList;

	va_start(argList, a_format);

	_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
	OutputDebugStringA(g_tempDebugString);
	printf("%s", g_tempDebugString);

	va_end(argList);

	if (INVALID_HANDLE_VALUE != consoleHandle)
	{
		SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	}
}

void syn::logC(bool a_condition, const char* a_format, ...)
{	
	if (a_condition == true)
	{
		va_list argList;

		va_start(argList, a_format);

		_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
		OutputDebugStringA(g_tempDebugString);
		printf("%s", g_tempDebugString);

		va_end(argList);
	}
}

void syn::logWarningC(bool a_condition, const char* a_format, ...)
{	
	if (a_condition == true)
	{
		HANDLE consoleHandle;
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (INVALID_HANDLE_VALUE != consoleHandle)
		{
			SetConsoleTextAttribute(consoleHandle,FOREGROUND_GREEN);
		}

		va_list argList;

		va_start(argList, a_format);

		_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
		OutputDebugStringA(g_tempDebugString);
		printf("%s", g_tempDebugString);

		va_end(argList);

		if (INVALID_HANDLE_VALUE != consoleHandle)
		{
			SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
		}
	}
}

void syn::logErrorC(bool a_condition, const char* a_format, ...)
{	
	if (a_condition == true)
	{
		HANDLE consoleHandle;
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (INVALID_HANDLE_VALUE != consoleHandle)
		{
			SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED);
		}

		va_list argList;

		va_start(argList, a_format);

		_vsnprintf(g_tempDebugString,TEMP_DBG_STRING_SIZE,a_format,argList);
		OutputDebugStringA(g_tempDebugString);
		printf("%s", g_tempDebugString);

		va_end(argList);

		if (INVALID_HANDLE_VALUE != consoleHandle)
		{
			SetConsoleTextAttribute(consoleHandle,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
		}
	}
}

#endif // _DEBUG

void __stdcall logGLDebugCallback(unsigned int a_source,
	unsigned int a_type,
	unsigned int a_id,
	unsigned int a_severity,
	int a_length,
	const char* a_message,
	const void* a_userParam)
{
	char source[16], type[20];
	if (a_source == GL_DEBUG_SOURCE_API)
		strcpy(source, "OpenGL");
	else if (a_source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
		strcpy(source, "Windows");
	else if (a_source == GL_DEBUG_SOURCE_SHADER_COMPILER)
		strcpy(source, "Shader Compiler");
	else if (a_source == GL_DEBUG_SOURCE_THIRD_PARTY)
		strcpy(source, "Third Party");
	else if (a_source == GL_DEBUG_SOURCE_APPLICATION)
		strcpy(source, "Application");
	else if (a_source == GL_DEBUG_SOURCE_OTHER)
		strcpy(source, "Other");

	if (a_type == GL_DEBUG_TYPE_ERROR)
		strcpy(type, "Error");
	else if (a_type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
		strcpy(type, "Deprecated Behavior");
	else if (a_type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		strcpy(type, "Undefined Behavior");
	else if (a_type == GL_DEBUG_TYPE_PORTABILITY)
		strcpy(type, "Portability");
	else if (a_type == GL_DEBUG_TYPE_PERFORMANCE)
		strcpy(type, "Performance");
	else if (a_type == GL_DEBUG_TYPE_MARKER)
		strcpy(type, "Marker");
	else if (a_type == GL_DEBUG_TYPE_PUSH_GROUP)
		strcpy(type, "Push Group");
	else if (a_type == GL_DEBUG_TYPE_POP_GROUP)
		strcpy(type, "Pop Group");
	else if (a_type == GL_DEBUG_TYPE_OTHER)
		strcpy(type, "Other");

	if (a_severity == GL_DEBUG_SEVERITY_HIGH)
		syn::logError("GL Error: %d\n\tType: %s\n\tSource: %s\n\tMessage: %s\n", a_id, type, source, a_message);
	else if (a_severity == GL_DEBUG_SEVERITY_MEDIUM)
		syn::logWarning("GL Warning: %d\n\tType: %s\n\tSource: %s\n\tMessage: %s\n", a_id, type, source, a_message);
	else if (a_severity == GL_DEBUG_SEVERITY_LOW)
		syn::log("GL: %d\n\tType: %s\n\tSource: %s\n\tMessage: %s\n", a_id, type, source, a_message);
	else if (a_severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		syn::log("GL Message: %d\n\tType: %s\n\tSource: %s\n\tMessage: %s\n", a_id, type, source, a_message);

}


} // namespace syn