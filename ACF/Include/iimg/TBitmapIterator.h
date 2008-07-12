#ifndef iimg_TBitmapIterator_included
#define iimg_TBitmapIterator_included


#include "iimg/IBitmap.h"


namespace iimg
{


/**
	Simple iterator for dimensionless iteration through bitmap pixels.
*/
template <typename PixelType>
class TBitmapIterator
{
public:
	typedef PixelType ValueType;
	/**
		Construct image iterator from a bitmap.
	*/
	TBitmapIterator(const iimg::IBitmap* bitmapPtr);

	/**
		Construct image iterator from another interator.
	*/
	TBitmapIterator(const TBitmapIterator& iterator);

	/**
		Access operator
	*/
	ValueType& operator*() const;

	bool operator < (const TBitmapIterator& iterator);
	bool operator > (const TBitmapIterator& iterator);
	bool operator == (const TBitmapIterator& iterator);
	TBitmapIterator operator++(int);
	TBitmapIterator& operator++();
	TBitmapIterator operator--(int);
	TBitmapIterator& operator--();
	TBitmapIterator& operator+=(int difference);
	TBitmapIterator& operator-=(int difference);
	TBitmapIterator operator+(int difference);
	TBitmapIterator operator-(int difference);

	/**
		Returns \c true if the index is valid in the image domain.
	*/
	bool IsValid() const;

	/**
		Move iterator to the position (x, y).
	*/
	void MoveTo(int x, int y);

	/**
		\overload
	*/
	void MoveTo(const istd::CIndex2d& index);

private:
	void Move(int index);
	void IncreaseIndex();
	void DecreaseIndex();

private:
	const PixelType* m_imageBufferBeginPtr;
	PixelType* m_imageBufferPtr;
	int m_linesDifference;
	int m_imageWidth;
	int m_imageHeight;
	istd::CIndex2d m_index;
};



template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const iimg::IBitmap* bitmapPtr)
{
	I_ASSERT(bitmapPtr != NULL);

	m_imageBufferPtr = (PixelType*)(bitmapPtr->GetLinePtr(0));
	m_imageBufferBeginPtr = m_imageBufferPtr;
	m_linesDifference = bitmapPtr->GetLinesDifference();
	m_imageWidth = bitmapPtr->GetImageSize().GetX();
	m_imageHeight = bitmapPtr->GetImageSize().GetY();
}


template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const TBitmapIterator& iterator)
:	m_imageBufferBeginPtr(iterator.m_imageBufferBeginPtr),
	m_imageBufferPtr(iterator.m_imageBufferPtr),
	m_linesDifference(iterator.m_linesDifference),
	m_imageWidth(iterator.m_imageWidth),
	m_imageHeight(iterator.m_imageHeight),
	m_index(iterator.m_index)
{
}


template <typename PixelType>
inline typename TBitmapIterator<PixelType>::ValueType& TBitmapIterator<PixelType>::operator*() const
{
	I_ASSERT(IsValid());
		
	return *m_imageBufferPtr;
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::IsValid() const
{
	return	((m_index.GetX() >= 0 &&  m_index.GetX() < m_imageWidth) &&
			(m_index.GetY() >= 0 &&  m_index.GetY() < m_imageHeight));
}


template <typename PixelType>
inline void TBitmapIterator<PixelType>::MoveTo(int x, int y)
{
	m_imageBufferPtr = const_cast<PixelType*>(m_imageBufferBeginPtr) + x + y * m_linesDifference;
}


template <typename PixelType>
inline void TBitmapIterator<PixelType>::MoveTo(const istd::CIndex2d& index)
{
	MoveTo(index.GetX(), index.GetY());
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator < (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr < iterator.m_imageBufferPtr);
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator > (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr > iterator.m_imageBufferPtr);
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::operator == (const TBitmapIterator& iterator)
{
	I_ASSERT(m_imageBufferBeginPtr == iterator.m_imageBufferBeginPtr);

	return (m_imageBufferPtr == iterator.m_imageBufferPtr);
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator++(int)
{
	TBitmapIterator<PixelType> temp(m_iterator);
	
	IncreaseIndex();

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::operator++()
{
	IncreaseIndex();

	return *this;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator--(int)
{
	TBitmapIterator<PixelType> temp(m_iterator);
	
	DecreaseIndex();

	return temp;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::operator--()
{
	DecreaseIndex();

	return *this;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::operator+=(int difference)
{
	Move(difference);

	return *this;
}


template <typename PixelType>
inline TBitmapIterator<PixelType>& TBitmapIterator<PixelType>::operator-=(int difference)
{
	Move(-difference);

	return *this;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator-(int difference)
{
	TBitmapIterator<PixelType> output(m_iterator);

	output.Move(-difference);

	return output;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator+(int difference)
{
	TBitmapIterator<PixelType> output(m_iterator);

	output.Move(difference);

	return output;
}


// private methods

template <typename PixelType>
inline void TBitmapIterator<PixelType>::Move(int difference)
{
	int currentX = m_index.GetX();
	int currentY = m_index.GetY();

	int dx = difference % m_imageWidth;
	int dy = difference / m_imageWidth;

	currentX += dx;

	if (currentX >= m_imageWidth){
		currentX %= m_imageWidth;
		++dy;
	}
	else if(currentX < 0){
		currentX = m_imageWidth + currentX;
		--dy;
	}

	currentY += dy;

	m_index.SetX(currentX);
	m_index.SetY(currentY);

	MoveTo(m_index);
}

	
template <typename PixelType>
inline void TBitmapIterator<PixelType>::IncreaseIndex()
{
	int currentX = m_index.GetX();
	++currentX;

	if (currentX >= m_imageWidth){
		currentX %= m_imageWidth;

		m_index.SetY(m_index.GetY() + 1);
	}

	m_index.SetX(currentX);

	MoveTo(m_index);
}


template <typename PixelType>
inline void TBitmapIterator<PixelType>::DecreaseIndex()
{
	int currentX = m_index.GetX();
	--currentX;

	if (currentX < 0){
		currentX = m_imageWidth + currentX;

		m_index.SetY(m_index.GetY() - 1);
	}

	m_index.SetX(currentX);

	MoveTo(m_index);
}


} // namespace iimg


#endif // !iimg_TBitmapIterator_included

