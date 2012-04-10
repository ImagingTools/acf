#ifndef ibase_TMsbWord_included
#define ibase_TMsbWord_included


// ACF includes
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
	TMsbWord(quint32 inputValue);
	TMsbWord(const TMsbWord& inputValue);
	
	/**
		Returns value as MSB.
	*/
	operator quint32() const;

	/**
		Returns the value as LSB.
	*/
	quint32 GetLsb() const;

	// pseudo-reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

private:
	quint8 m_bytes[Size];
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
inline TMsbWord<Size>::TMsbWord(quint32 inputValue)
{
	I_ASSERT(Size > 0);
	I_ASSERT(Size <= 4);

	for (int i = 0; i < Size; i++){
		m_bytes[i] = quint8((inputValue >> (i * 8)) & 0xff);
	}
}


template <int Size>
inline TMsbWord<Size>::TMsbWord(const TMsbWord& inputValue)
{
	I_ASSERT(Size > 0);
	I_ASSERT(Size <= 4);

	std::memcpy(m_bytes, &inputValue.m_bytes, sizeof(m_bytes));
}


template <int Size>
inline TMsbWord<Size>::operator quint32() const
{
	quint32 retVal = 0;

	for (int i = 0; i < Size; i++){
		retVal += quint32(m_bytes[i]) << (i * 8);
	}

	return retVal;
}

	
template <int Size>
inline quint32 TMsbWord<Size>::GetLsb() const
{
	quint32 retVal = 0;

	for (int i = 0; i < Size; i++){
		retVal += quint32(m_bytes[Size - i - 1])  << (i * 8);
	}

	return retVal;
}

	
// pseudo-reimplemented (iser::ISerializable)

template <int Size>
inline bool TMsbWord<Size>::Serialize(iser::IArchive& archive)
{
	return archive.ProcessData(m_bytes, int(sizeof(m_bytes)));
}


typedef TMsbWord<4> CMsbDWord;
typedef TMsbWord<2> CMsbWord;


} // namespace ibase


#endif // !ibase_TMsbWord_included


