#include <utilities/BitField.h>

namespace syn
{

void BitField::flipAll()
{
	unsigned int uiByteCount = (m_bitCount - 1) / BYTE_BITS + 1;
	for (unsigned int uiByte = 0; uiByte < uiByteCount; ++uiByte)
	{
		m_bits[uiByte] = ~m_bits[uiByte];
	}
}

bool BitField::anySet()
{
	unsigned int uiByteCount = (m_bitCount - 1) / BYTE_BITS + 1;
	for (unsigned int uiByte = 0; uiByte < uiByteCount; ++uiByte)
	{
		if (m_bits[uiByte] != 0)
			return true;
	}
	return false;
}

} // namespace syn