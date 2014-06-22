#include <utilities/Assert.h>

#ifdef _DEBUG

#include <windows.h>
#include <string>

bool syn::synAssert( const char* a_expression, const char* a_description, 
	int a_line, const char* a_file, const char* a_function)
{
	// convert the line number into a character
	char line[32];
	sprintf(line, "%i", a_line);

	//shorten a lengthy file directory
	const char* shortFileName = strrchr(a_file, '\\');
	if (shortFileName == nullptr)
		shortFileName = a_file;
	else
		++shortFileName;

	// create the display message
	std::string msg("Assertion failed!: ");
	msg += "\nDescription: ";
	msg += a_description;
	msg += "\nExpression: ";
	msg += a_expression;
	msg += "\nFile: ";
	msg += shortFileName;
	msg += "\nFunction: ";
	msg += a_function;
	msg += "\nLine: ";
	msg += line;
	msg += "\nWould you like to break to assert location?";

	// display message to output window
	OutputDebugStringA(msg.c_str());

	// display the message box and grab the result
	int result = MessageBoxA(0, msg.c_str(), "Assertion failed!", MB_YESNO);

	if (result == IDYES)
		return true;
	else if (result == IDNO)
		return false;

	return false;
}

#endif // _DEBUG