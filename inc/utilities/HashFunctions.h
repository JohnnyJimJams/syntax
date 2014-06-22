#pragma once

#include <Syntax.h>
#include <functional>

namespace syn
{

class SYNTAX_API HashFunctions
{
public:	
	
	// Default hash
	static std::function<unsigned int(const char*, unsigned int)>	hash;

	// Collection of hash functions found at http://www.partow.net/programming/hashfunctions/
	static unsigned int	RSHash	(const char* a_key, unsigned int a_length);
	static unsigned int	JSHash  (const char* a_key, unsigned int a_length);
	static unsigned int	PJWHash (const char* a_key, unsigned int a_length);
	static unsigned int	ELFHash (const char* a_key, unsigned int a_length);
	static unsigned int	BKDRHash(const char* a_key, unsigned int a_length);
	static unsigned int	SDBMHash(const char* a_key, unsigned int a_length);
	static unsigned int	DJBHash (const char* a_key, unsigned int a_length);
	static unsigned int	DEKHash (const char* a_key, unsigned int a_length);
	static unsigned int	APHash  (const char* a_key, unsigned int a_length);
};

#define SynHash(bytes,byteLength) syn::HashFunctions::hash(bytes,byteLength)
#define SynStringHash(str) syn::HashFunctions::hash(str,strlen(str))

} // namespace syn