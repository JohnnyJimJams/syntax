#pragma once

#include <Syntax.h>
#include <memory.h>

namespace syn
{

class SYNTAX_API BitField
{
public:

	BitField(unsigned int a_bitCount);
	~BitField();

	unsigned char*	getBits() const;

	void	setAll();
	void	clearAll();
	void	flipAll();

	void	set(unsigned int a_bit, bool a_set = true);
	void	clear(unsigned int a_bit);
	void	flip(unsigned int a_bit);

	unsigned int	bitCount() const;
	unsigned int	byteCount() const;

	bool	anySet();

	bool	isSet(unsigned int a_bit) const;

	bool	operator [] (unsigned int a_bit) const;

private:

	BitField(const BitField& a_rhs);
	BitField& operator =(const BitField& a_rhs);

private:

	enum
	{
		BYTE_BITS = 8,
	};

	unsigned int	m_bitCount;
	unsigned char*	m_bits;
};

inline BitField::BitField(unsigned int a_bitCount)
	: m_bitCount(a_bitCount),
	m_bits(0)
{
	unsigned int uiByteCount = (m_bitCount - 1) / BYTE_BITS + 1;
	m_bits = new unsigned char[uiByteCount];
	memset(m_bits, 0, uiByteCount);
}

inline BitField::~BitField()
{
	delete[] m_bits;
}

inline void BitField::setAll()
{
	unsigned int uiByteCount = (m_bitCount - 1) / BYTE_BITS + 1;
	memset(m_bits, 0xff, uiByteCount);
}

inline void BitField::clearAll()
{
	unsigned int uiByteCount = (m_bitCount - 1) / BYTE_BITS + 1;
	memset(m_bits, 0, uiByteCount);
}

inline void BitField::set(unsigned int a_bit, bool a_set /* = true */)
{
	if (a_set == true)
		m_bits[a_bit / BYTE_BITS] |= (1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)));
	else
	{
		unsigned char ubMask = ~(1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)));
		m_bits[a_bit / BYTE_BITS] &= ubMask;
	}
}

inline void BitField::clear(unsigned int a_bit)
{
	unsigned char ubMask = ~(1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)));
	m_bits[a_bit / BYTE_BITS] &= ubMask;
}

inline void BitField::flip(unsigned int a_bit)
{
	unsigned int bit = a_bit / BYTE_BITS;
	unsigned char ubMask = (1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)));
	if ((m_bits[bit] & ubMask) != 0)
	{
		m_bits[bit] &= ~ubMask;
	}
	else
	{
		m_bits[bit] |= ubMask;
	}
}

inline unsigned int BitField::bitCount() const
{
	return m_bitCount;
}

inline unsigned int BitField::byteCount() const
{
	return (m_bitCount - 1) / BYTE_BITS + 1;
}

inline bool BitField::isSet(unsigned int a_bit) const
{
	return (m_bits[a_bit / BYTE_BITS] & (1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)))) != 0;
}

inline bool BitField::operator [] (unsigned int a_bit) const
{
	return (m_bits[a_bit / BYTE_BITS] & (1 << (BYTE_BITS - 1 - (a_bit % BYTE_BITS)))) != 0;
}

inline unsigned char* BitField::getBits() const
{
	return m_bits;
}

} // namespace syn