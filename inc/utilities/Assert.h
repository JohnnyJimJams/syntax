#pragma once

#include <Syntax.h>

namespace syn
{

#ifdef assert
	#undef assert
#endif // assert

#ifdef _DEBUG

	bool SYNTAX_API synAssert(const char* a_expression, 
		const char* a_description, 
		int a_line, 
		const char* a_file, 
		const char* a_function);

	#define assert(exp) \
			{	\
				if (!(exp)){\
					if (syn::synAssert(#exp, "Expression failed!", __LINE__, __FILE__, __FUNCTION__))\
					{_asm{int 3}}}\
			}

	#define assertMessage(exp, description) \
			{	\
				if (!(exp)){\
					if (syn::synAssert(#exp, description, __LINE__, __FILE__, __FUNCTION__))\
					{_asm{int 3}}}\
			}

	#define validPtr(ptr) \
			{	\
				if ((ptr) == nullptr){\
					if (syn::synAssert(#ptr, "Invalid pointer!", __LINE__, __FILE__, __FUNCTION__))\
					{_asm{int 3}}}\
			}

#else

	#define assert(exp)
	#define assertMessage(exp, description)
	#define validPtr(ptr)

#endif // _DEBUG

} // namespace syn