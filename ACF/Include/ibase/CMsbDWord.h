#ifndef ibase_CMsbDWord_included
#define ibase_CMsbDWord_included


#include "iser/IArchive.h"


namespace ibase
{


#pragma pack(push, 1)


/**	
	This class represents double word type with net byte order (MSB first).
*/
class CMsbDWord
{
public:
	CMsbDWord();
	CMsbDWord(I_DWORD inputValue);
	CMsbDWord(const CMsbDWord& inputValue);
	operator I_DWORD() const;

	I_DWORD GetLsb() const;

	bool Serialize(iser::IArchive& archive);

private:
	I_BYTE m_bytes[4];
};



#pragma pack(pop)



// inline methods

inline CMsbDWord::CMsbDWord()
{
}


inline CMsbDWord::CMsbDWord(I_DWORD inputValue)
{
	m_bytes[0] = inputValue & 0xff;
	m_bytes[1] = (inputValue >> 8) & 0xff;
	m_bytes[2] = (inputValue >> 16) & 0xff;
	m_bytes[3] = (inputValue >> 24) & 0xff;
}


inline CMsbDWord::CMsbDWord(const CMsbDWord& inputValue)
{
	memcpy(m_bytes, &inputValue.m_bytes, sizeof(m_bytes));
}


inline CMsbDWord::operator I_DWORD() const
{
	I_DWORD retVal = I_DWORD(m_bytes[0]);
	retVal += I_DWORD(m_bytes[1]) << 8;
	retVal += I_DWORD(m_bytes[2]) << 16;
	retVal += I_DWORD(m_bytes[3]) << 24;

	return retVal;
}

	
inline I_DWORD CMsbDWord::GetLsb() const
{
	I_DWORD retVal = I_DWORD(m_bytes[3]);
	retVal += I_DWORD(m_bytes[2]) << 8;
	retVal += I_DWORD(m_bytes[1]) << 16;
	retVal += I_DWORD(m_bytes[0]) << 24;

	return retVal;
}



inline bool CMsbDWord::Serialize(iser::IArchive& archive)
{
	return archive.ProcessData(m_bytes, sizeof(m_bytes));
}


} // namespace ibase


#endif // !ibase_CMsbDWord_included

