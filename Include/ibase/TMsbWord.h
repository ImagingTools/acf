#ifndef ibase_TMsbWord_included
#define ibase_TMsbWord_included


#include "iser/IArchive.h"


namespace ibase
{


#pragma pack(push, 1)


/**	
	This class represents double word type with net byte order (MSB first).
*/
template <int Size>
class TMsbWord
{
public:
	TMsbWord();
	TMsbWord(I_DWORD inputValue);
	TMsbWord(const TMsbWord& inputValue);
	
	/**
		Returns value as MSB.
	*/
	operator I_DWORD() const;

	/**
		Returns the value as LSB.
	*/
	I_DWORD GetLsb() const;

	// pseudo-reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

private:
	I_BYTE m_bytes[Size];
};


#pragma pack(pop)


// inline methods

template <int Size>
inline TMsbWord<Size>::TMsbWord()
{
	I_ASSERT(Size > 0);
	I_ASSERT(Size <= 4);
}


template <int Size>
inline TMsbWord<Size>::TMsbWord(I_DWORD inputValue)
{
	I_ASSERT(Size > 0);
	I_ASSERT(Size <= 4);

	for (int i = 0; i < Size; i++){
		m_bytes[i] = (inputValue >> (i * 8)) & 0xff;
	}
}


template <int Size>
inline TMsbWord<Size>::TMsbWord(const TMsbWord& inputValue)
{
	I_ASSERT(Size > 0);
	I_ASSERT(Size <= 4);

	memcpy(m_bytes, &inputValue.m_bytes, sizeof(m_bytes));
}


template <int Size>
inline TMsbWord<Size>::operator I_DWORD() const
{
	I_DWORD retVal = 0;

	for (int i = 0; i < Size; i++){
		retVal += I_DWORD(m_bytes[i]) << (i * 8);
	}

	return retVal;
}

	
template <int Size>
inline I_DWORD TMsbWord<Size>::GetLsb() const
{
	I_DWORD retVal = 0;

	for (int i = 0; i < Size; i++){
		retVal += I_DWORD(m_bytes[Size - i - 1])  << (i * 8);
	}

	return retVal;
}

	
// pseudo-reimplemented (iser::ISerializable)

template <int Size>
inline bool TMsbWord<Size>::Serialize(iser::IArchive& archive)
{
	return archive.ProcessData(m_bytes, sizeof(m_bytes));
}


typedef TMsbWord<4> CMsbDWord;
typedef TMsbWord<2> CMsbWord;


} // namespace ibase


#endif // !ibase_TMsbWord_included

