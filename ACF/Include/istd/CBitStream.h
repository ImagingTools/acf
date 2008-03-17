#ifndef istd_CBitStream_included
#define istd_CBitStream_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


#include <vector>


namespace istd
{


/**
	This class provides bitwise access to the byte data.
*/
class CBitStream: public istd::IPolymorphic
{
public:
	/**
		Constructs a bit stream from a vector of bytes. 
	*/
	CBitStream(const std::vector<I_BYTE>& data);

	/**
		Constructs an empty bit stream.
	*/
	CBitStream();

	/**
		Removes all data from the stream.
	*/
	void Reset();

	/**
		Reset internal position cursor and moves it to begin of the stream.
	*/
	void ResetPosition();

	/**
		Gets the value of the next \c bits in the stream.
	*/
	I_DWORD GetValue(int bits);

	/**
		Pushes a value of the \c word bounding by N \c bits into the stream.
	*/
	void PushBack(I_DWORD word, int bits);

	/**
		Returns \c true if the end of stream is reached.
	*/
	bool IsEos() const;

private:
	typedef std::vector<I_BYTE> StreamData;
	StreamData m_stream;

	int m_bitPosition;
};



} // namespace istd


#endif // !istd_CBitStream_included

