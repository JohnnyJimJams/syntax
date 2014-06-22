#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

namespace syn
{

class Property
{
public:

	enum PropertyType : unsigned int
	{
		eBOOL = 0,
		eBYTE,
		eUBYTE,
		eSHORT,
		eUSHORT,
		eINT,
		eUINT,
		eFLOAT,
		eDOUBLE,

		eIVEC2,
		eIVEC3,
		eIVEC4,
		eVEC2,
		eVEC3,
		eVEC4,

		eMAT2,
		eMAT3,
		eMAT4,
		eQUAT,

		eSTRING,

		eBOOLPTR,
		eBYTEPTR,
		eUBYTEPTR,
		eSHORTPTR,
		eUSHORTPTR,
		eINTPTR,
		eUINTPTR,
		eFLOATPTR,
		eDOUBLEPTR,

		eIVEC2PTR,
		eIVEC3PTR,
		eIVEC4PTR,
		eVEC2PTR,
		eVEC3PTR,
		eVEC4PTR,

		eMAT2PTR,
		eMAT3PTR,
		eMAT4PTR,
		eQUATPTR,

		eSTRINGPTR,

		eFUNCTION,
		eVOIDPTR,
	};

	Property(bool a_data) : m_type(eBOOL), m_bData(a_data)	{}
	Property(char a_data) : m_type(eBYTE), m_i8Data(a_data)	{}
	Property(unsigned char a_data) : m_type(eUBYTE), m_ui8Data(a_data)	{}
	Property(short a_data) : m_type(eSHORT), m_sData(a_data)	{}
	Property(unsigned short a_data) : m_type(eUSHORT), m_usData(a_data)	{}
	Property(int a_data) : m_type(eINT), m_iData(a_data)	{}
	Property(unsigned int a_data) : m_type(eUINT), m_uiData(a_data)	{}
	Property(float a_data) : m_type(eFLOAT), m_fData(a_data)	{}
	Property(double a_data) : m_type(eDOUBLE), m_dData(a_data)	{}
	Property(const glm::ivec2& a_data) : m_type(eIVEC2), m_iv2Data(new glm::ivec2()) { *m_iv2Data = a_data; }
	Property(const glm::ivec3& a_data) : m_type(eIVEC3), m_iv3Data(new glm::ivec3()) { *m_iv3Data = a_data; }
	Property(const glm::ivec4& a_data) : m_type(eIVEC4), m_iv4Data(new glm::ivec4()) { *m_iv4Data = a_data; }
	Property(const glm::vec2& a_data) : m_type(eVEC2), m_v2Data(new glm::vec2()) { *m_v2Data = a_data; }
	Property(const glm::vec3& a_data) : m_type(eVEC3), m_v3Data(new glm::vec3()) { *m_v3Data = a_data; }
	Property(const glm::vec4& a_data) : m_type(eVEC4), m_v4Data(new glm::vec4()) { *m_v4Data = a_data; }
	Property(const glm::mat2& a_data) : m_type(eMAT2), m_m2Data(new glm::mat2()) { *m_m2Data = a_data; }
	Property(const glm::mat3& a_data) : m_type(eMAT3), m_m3Data(new glm::mat3()) { *m_m3Data = a_data; }
	Property(const glm::mat4& a_data) : m_type(eMAT4), m_m4Data(new glm::mat4()) { *m_m4Data = a_data; }
	Property(const glm::quat& a_data) : m_type(eQUAT), m_qData(new glm::quat()) { *m_qData = a_data; }
	Property(const char* a_data) : m_type(eSTRING), m_stData(new std::string())	{ *m_stData = a_data; }
	Property(bool* a_data) : m_type(eBOOLPTR), m_pbData(a_data)	{}
	Property(char* a_data) : m_type(eBYTEPTR), m_pi8Data(a_data)	{}
	Property(unsigned char* a_data) : m_type(eUBYTEPTR), m_pui8Data(a_data)	{}
	Property(short* a_data) : m_type(eSHORTPTR), m_psData(a_data)	{}
	Property(unsigned short* a_data) : m_type(eUSHORTPTR), m_pusData(a_data)	{}
	Property(int* a_data) : m_type(eINTPTR), m_piData(a_data)	{}
	Property(unsigned int* a_data) : m_type(eUINTPTR), m_puiData(a_data)	{}
	Property(float* a_data) : m_type(eFLOATPTR), m_pfData(a_data)	{}
	Property(double* a_data) : m_type(eDOUBLEPTR), m_pdData(a_data)	{}
	Property(glm::ivec2* a_data) : m_type(eIVEC2PTR), m_iv2Data(a_data) {}
	Property(glm::ivec3* a_data) : m_type(eIVEC3PTR), m_iv3Data(a_data) {}
	Property(glm::ivec4* a_data) : m_type(eIVEC4PTR), m_iv4Data(a_data) {}
	Property(glm::vec2* a_data) : m_type(eVEC2PTR), m_v2Data(a_data) {}
	Property(glm::vec3* a_data) : m_type(eVEC3PTR), m_v3Data(a_data) {}
	Property(glm::vec4* a_data) : m_type(eVEC4PTR), m_v4Data(a_data) {}
	Property(glm::mat2* a_data) : m_type(eMAT2PTR), m_m2Data(a_data) {}
	Property(glm::mat3* a_data) : m_type(eMAT3PTR), m_m3Data(a_data) {}
	Property(glm::mat4* a_data) : m_type(eMAT4PTR), m_m4Data(a_data) {}
	Property(glm::quat* a_data) : m_type(eQUATPTR), m_qData(a_data) {}
	Property(std::string* a_data) : m_type(eSTRINGPTR), m_stData(a_data)	{}
	Property(void* a_data) : m_type(eVOIDPTR), m_pData(a_data)	{}
	~Property()
	{
		switch (m_type)
		{
		case eIVEC2: delete m_iv2Data; break;
		case eIVEC3: delete m_iv3Data; break;
		case eIVEC4: delete m_iv4Data; break;
		case eVEC2: delete m_v2Data; break;
		case eVEC3: delete m_v3Data; break;
		case eVEC4: delete m_v4Data; break;
		case eMAT2: delete m_m2Data; break;
		case eMAT3: delete m_m3Data; break;
		case eMAT4: delete m_m4Data; break;
		case eQUAT: delete m_qData; break;
		case eSTRING: delete m_stData; break;
		default:	break;
		};
	}

	PropertyType	type() const	{ return m_type; }

	bool	get(bool& a_data)			{ if (m_type == eBOOL) { a_data = m_bData; return true; } else if (m_type == eBOOLPTR) { a_data = *m_pbData; return true; } return false; }
	bool	get(char& a_data)			{ if (m_type == eBYTE) { a_data = m_i8Data; return true; } else if (m_type == eBYTEPTR) { a_data = *m_pi8Data; return true; } return false; }
	bool	get(unsigned char& a_data)	{ if (m_type == eUBYTE) { a_data = m_ui8Data; return true; } else if (m_type == eUBYTEPTR) { a_data = *m_pui8Data; return true; } return false; }
	bool	get(short& a_data)			{ if (m_type == eSHORT) { a_data = m_sData; return true; } else if (m_type == eSHORTPTR) { a_data = *m_psData; return true; } return false; }
	bool	get(unsigned short& a_data)	{ if (m_type == eUSHORT) { a_data = m_usData; return true; } else if (m_type == eUSHORTPTR) { a_data = *m_pusData; return true; } return false; }
	bool	get(int& a_data)			{ if (m_type == eINT) { a_data = m_iData; return true; } else if (m_type == eINTPTR) { a_data = *m_piData; return true; } return false; }
	bool	get(unsigned int& a_data)	{ if (m_type == eUINT) { a_data = m_uiData; return true; } else if (m_type == eUINTPTR) { a_data = *m_puiData; return true; } return false; }
	bool	get(float& a_data)			{ if (m_type == eFLOAT) { a_data = m_fData; return true; } else if (m_type == eFLOATPTR) { a_data = *m_pfData; return true; } return false; }
	bool	get(double& a_data)			{ if (m_type == eDOUBLE) { a_data = m_dData; return true; } else if (m_type == eDOUBLEPTR) { a_data = *m_pdData; return true; } return false; }
	bool	get(glm::ivec2& a_data)		{ if (m_type == eIVEC2 || m_type == eIVEC2PTR) { a_data = *m_iv2Data; return true; } return false; }
	bool	get(glm::ivec3& a_data)		{ if (m_type == eIVEC3 || m_type == eIVEC3PTR) { a_data = *m_iv3Data; return true; } return false; }
	bool	get(glm::ivec4& a_data)		{ if (m_type == eIVEC4 || m_type == eIVEC4PTR) { a_data = *m_iv4Data; return true; } return false; }
	bool	get(glm::vec2& a_data)		{ if (m_type == eVEC2 || m_type == eVEC2PTR) { a_data = *m_v2Data; return true; } return false; }
	bool	get(glm::vec3& a_data)		{ if (m_type == eVEC3 || m_type == eVEC3PTR) { a_data = *m_v3Data; return true; } return false; }
	bool	get(glm::vec4& a_data)		{ if (m_type == eVEC4 || m_type == eVEC4PTR) { a_data = *m_v4Data; return true; } return false; }
	bool	get(glm::mat2& a_data)		{ if (m_type == eMAT2 || m_type == eMAT2PTR) { a_data = *m_m2Data; return true; } return false; }
	bool	get(glm::mat3& a_data)		{ if (m_type == eMAT3 || m_type == eMAT3PTR) { a_data = *m_m3Data; return true; } return false; }
	bool	get(glm::mat4& a_data)		{ if (m_type == eMAT4 || m_type == eMAT4PTR) { a_data = *m_m4Data; return true; } return false; }
	bool	get(glm::quat& a_data)		{ if (m_type == eQUAT || m_type == eQUATPTR) { a_data = *m_qData; return true; } return false; }
	bool	get(std::string& a_data)	{ if (m_type == eSTRING || m_type == eSTRINGPTR) { a_data = *m_stData; return true; } return false; }
	bool	get(void** a_data)			{ if (m_type == eVOIDPTR) { *a_data = m_pData; return true; } return false; }

	bool	set(bool a_data)				{ if (m_type == eBOOL) { m_bData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pbData = a_data; return true; } return false; }
	bool	set(char a_data)				{ if (m_type == eBYTE) { m_i8Data = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pi8Data = a_data; return true; } return false; }
	bool	set(unsigned char a_data)		{ if (m_type == eUBYTE) { m_ui8Data = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pui8Data = a_data; return true; } return false; }
	bool	set(short a_data)				{ if (m_type == eSHORT) { m_sData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_psData = a_data; return true; } return false; }
	bool	set(unsigned short a_data)		{ if (m_type == eUSHORT) { m_usData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pusData = a_data; return true; } return false; }
	bool	set(int a_data)					{ if (m_type == eINT) { m_iData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_piData = a_data; return true; } return false; }
	bool	set(unsigned int a_data)		{ if (m_type == eUINT) { m_uiData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_puiData = a_data; return true; } return false; }
	bool	set(float a_data)				{ if (m_type == eFLOAT) { m_fData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pfData = a_data; return true; } return false; }
	bool	set(double a_data)				{ if (m_type == eDOUBLE) { m_dData = a_data; return true; } else if (m_type == eBOOLPTR) { *m_pdData = a_data; return true; } return false; }
	bool	set(const glm::ivec2& a_data)	{ if (m_type == eIVEC2 || m_type == eIVEC2PTR) { *m_iv2Data = a_data; return true; } return false; }
	bool	set(const glm::ivec3& a_data)	{ if (m_type == eIVEC3 || m_type == eIVEC3PTR) { *m_iv3Data = a_data; return true; } return false; }
	bool	set(const glm::ivec4& a_data)	{ if (m_type == eIVEC4 || m_type == eIVEC4PTR) { *m_iv4Data = a_data; return true; } return false; }
	bool	set(const glm::vec2& a_data)	{ if (m_type == eVEC2 || m_type == eVEC2PTR) { *m_v2Data = a_data; return true; } return false; }
	bool	set(const glm::vec3& a_data)	{ if (m_type == eVEC3 || m_type == eVEC3PTR) { *m_v3Data = a_data; return true; } return false; }
	bool	set(const glm::vec4& a_data)	{ if (m_type == eVEC4 || m_type == eVEC4PTR) { *m_v4Data = a_data; return true; } return false; }
	bool	set(const glm::mat2& a_data)	{ if (m_type == eMAT2 || m_type == eMAT2PTR) { *m_m2Data = a_data; return true; } return false; }
	bool	set(const glm::mat3& a_data)	{ if (m_type == eMAT3 || m_type == eMAT3PTR) { *m_m3Data = a_data; return true; } return false; }
	bool	set(const glm::mat4& a_data)	{ if (m_type == eMAT4 || m_type == eMAT4PTR) { *m_m4Data = a_data; return true; } return false; }
	bool	set(const glm::quat& a_data)	{ if (m_type == eQUAT || m_type == eQUATPTR) { *m_qData = a_data; return true; } return false; }
	bool	set(const char* a_data)			{ if (m_type == eSTRING || m_type == eSTRINGPTR) { *m_stData = a_data; return true; } return false; }
	bool	set(void* a_data)				{ if (m_type == eVOIDPTR) { m_pData = a_data; return true; } return false; }

	// these ignore type rules! you have been warned! 
	bool&			dataAsBool()		{ if (m_type == eBOOLPTR) return *m_pbData; return m_bData; }
	char&			dataAsByte()		{ if (m_type == eBYTEPTR) return *m_pi8Data; return m_i8Data; }
	unsigned char&	dataAsUByte()		{ if (m_type == eUBYTEPTR) return *m_pui8Data; return m_ui8Data; }
	short&			dataAsShort()		{ if (m_type == eSHORTPTR) return *m_psData; return m_sData; }
	unsigned short&	dataAsUShort()		{ if (m_type == eUSHORTPTR) return *m_pusData; return m_usData; }
	int&			dataAsInt()			{ if (m_type == eINTPTR) return *m_piData; return m_iData; }
	unsigned int&	dataAsUInt()		{ if (m_type == eUINTPTR) return *m_puiData; return m_uiData; }
	float&			dataAsFloat()		{ if (m_type == eFLOATPTR) return *m_pfData; return m_fData; }
	double&			dataAsDouble()		{ if (m_type == eDOUBLEPTR) return *m_pdData; return m_dData; }
	glm::ivec2&		dataAsIVec2()		{ return *m_iv2Data; }
	glm::ivec3&		dataAsIVec3()		{ return *m_iv3Data; }
	glm::ivec4&		dataAsIVec4()		{ return *m_iv4Data; }
	glm::vec2&		dataAsVec2()		{ return *m_v2Data; }
	glm::vec3&		dataAsVec3()		{ return *m_v3Data; }
	glm::vec4&		dataAsVec4()		{ return *m_v4Data; }
	glm::mat2&		dataAsMat2()		{ return *m_m2Data; }
	glm::mat3&		dataAsMat3()		{ return *m_m3Data; }
	glm::mat4&		dataAsMat4()		{ return *m_m4Data; }
	glm::quat&		dataAsQuat()		{ return *m_qData; }
	std::string&	dataAsString()		{ return *m_stData; }

	bool*			dataAsBoolPtr()		{ if (m_type == eBOOLPTR) return m_pbData; return &m_bData; }
	char*			dataAsBytePtr()		{ if (m_type == eBYTEPTR) return m_pi8Data; return &m_i8Data; }
	unsigned char*	dataAsUBytePtr()	{ if (m_type == eUBYTEPTR) return m_pui8Data; return &m_ui8Data; }
	short*			dataAsShortPtr()	{ if (m_type == eSHORTPTR) return m_psData; return &m_sData; }
	unsigned short*	dataAsUShortPtr()	{ if (m_type == eUSHORTPTR) return m_pusData; return &m_usData; }
	int*			dataAsIntPtr()		{ if (m_type == eINTPTR) return m_piData; return &m_iData; }
	unsigned int*	dataAsUIntPtr()		{ if (m_type == eUINTPTR) return m_puiData; return &m_uiData; }
	float*			dataAsFloatPtr()	{ if (m_type == eFLOATPTR) return m_pfData; return &m_fData; }
	double*			dataAsDoublePtr()	{ if (m_type == eDOUBLEPTR) return m_pdData; return &m_dData; }
	glm::ivec2*		dataAsIVec2Ptr()	{ return m_iv2Data; }
	glm::ivec3*		dataAsIVec3Ptr()	{ return m_iv3Data; }
	glm::ivec4*		dataAsIVec4Ptr()	{ return m_iv4Data; }
	glm::vec2*		dataAsVec2Ptr()		{ return m_v2Data; }
	glm::vec3*		dataAsVec3Ptr()		{ return m_v3Data; }
	glm::vec4*		dataAsVec4Ptr()		{ return m_v4Data; }
	glm::mat2*		dataAsMat2Ptr()		{ return m_m2Data; }
	glm::mat3*		dataAsMat3Ptr()		{ return m_m3Data; }
	glm::mat4*		dataAsMat4Ptr()		{ return m_m4Data; }
	glm::quat*		dataAsQuatPtr()		{ return m_qData; }
	std::string*	dataAsStringPtr()	{ return m_stData; }
	void*			dataAsVoidPtr()		{ return m_pData; }

private:

	PropertyType		m_type;

	union
	{
		bool			m_bData;
		char			m_i8Data;
		unsigned char	m_ui8Data;
		short			m_sData;
		unsigned short	m_usData;
		int				m_iData;
		unsigned int	m_uiData;
		float			m_fData;
		double			m_dData;

		bool*			m_pbData;
		char*			m_pi8Data;
		unsigned char*	m_pui8Data;
		short*			m_psData;
		unsigned short*	m_pusData;
		int*			m_piData;
		unsigned int*	m_puiData;
		float*			m_pfData;
		double*			m_pdData;

		glm::ivec2*		m_iv2Data;
		glm::ivec3*		m_iv3Data;
		glm::ivec4*		m_iv4Data;
		glm::vec2*		m_v2Data;
		glm::vec3*		m_v3Data;
		glm::vec4*		m_v4Data;

		glm::mat2*		m_m2Data;
		glm::mat3*		m_m3Data;
		glm::mat4*		m_m4Data;
		glm::quat*		m_qData;

		std::string*	m_stData;

		void*			m_pData;
	};
};

} // namespace syn