#pragma once

#include <utilities/Log.h>

#ifdef _DEBUG

#include <windows.h>
#include <string>

namespace syn
{

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

} // namespace syn

#endif // _DEBUG