#include <utilities/HashFunctions.h>

namespace syn
{

std::function<unsigned int(const char*,unsigned int)> HashFunctions::hash = HashFunctions::ELFHash;

unsigned int HashFunctions::RSHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int b    = 378551;
	unsigned int a    = 63689;
	unsigned int uiHash = 0;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash = uiHash * a + a_szKey[i];
		a    = a * b;
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::JSHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = 1315423911;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash ^= ((uiHash << 5) + a_szKey[i] + (uiHash >> 2));
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::PJWHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
	unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
	unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int uiHash            = 0;
	unsigned int test              = 0;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash = (uiHash << OneEighth) + a_szKey[i];

		if ((test = uiHash & HighBits)  != 0)
		{
			uiHash = (( uiHash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::ELFHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = 0;
	unsigned int x    = 0;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash = (uiHash << 4) + a_szKey[i];
		if ((x = uiHash & 0xF0000000L) != 0)
		{
			uiHash ^= (x >> 24);
			uiHash &= ~x;
		}
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::BKDRHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int uiHash = 0;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash = (uiHash * seed) + a_szKey[i];
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::SDBMHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = 0;
	
	for(unsigned int i = 0; i < a_uiLength; i++)
	{
		uiHash = a_szKey[i] + (uiHash << 6) + (uiHash << 16) - uiHash;
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::DJBHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = 5381;
	
	for(unsigned int i = 0; i < a_uiLength; i++)
	{
		uiHash = ((uiHash << 5) + uiHash) + a_szKey[i];
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::DEKHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = a_uiLength;

	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash = ((uiHash << 5) ^ (uiHash >> 27)) ^ a_szKey[i];
	}

	return (uiHash & 0x7FFFFFFF);
}

unsigned int HashFunctions::APHash(const char* a_szKey, unsigned int a_uiLength)
{
	unsigned int uiHash = 0;
	
	for ( unsigned int i = 0 ; i < a_uiLength ; ++i )
	{
		uiHash ^= ((i & 1) == 0) ?
				(  (uiHash <<  7) ^ a_szKey[i] ^ (uiHash >> 3)) :
				(~((uiHash << 11) ^ a_szKey[i] ^ (uiHash >> 5)));
	}

	return (uiHash & 0x7FFFFFFF);
}

} // namespace syn