#ifndef iimg_TBitmapIterator_included
#define iimg_TBitmapIterator_included


#include "i2d/CRectangle.h"

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
	TBitmapIterator(const iimg::IBitmap* bitmapPtr, const i2d::CRectangle* regionPtr = NULL);

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
	TBitmapIterator operator+(int difference) const;
	TBitmapIterator operator-(int difference) const;

	/**
		Returns \c true if the index is valid in the image domain.
	*/
	bool IsValid() const;

	/**
		Returns \c true, if the index is out of the iteration boudaries.
	*/
	bool AtEnd() const;

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
	const PixelType* m_imageBufferEndPtr;
	PixelType* m_imageBufferPtr;
	int m_linesDifference;
	istd::CIndex2d m_startIndex;
	istd::CIndex2d m_index;
	istd::CIndex2d m_endIndex;
};


template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const iimg::IBitmap* bitmapPtr, const i2d::CRectangle* regionPtr)
{
	I_ASSERT(bitmapPtr != NULL);

	i2d::CRectangle imageRegion(bitmapPtr->GetImageSize());
	if (regionPtr != NULL){
		imageRegion = regionPtr->GetIntersection(imageRegion);
	}

	int imageHeight = bitmapPtr->GetImageSize().GetY();
	m_linesDifference = bitmapPtr->GetLinesDifference();
	m_imageBufferBeginPtr = (PixelType*)bitmapPtr->GetLinePtr(0);
	m_imageBufferEndPtr = m_imageBufferBeginPtr + imageHeight * m_linesDifference;

	m_imageBufferPtr = const_cast<PixelType*>(m_imageBufferBeginPtr) + int(imageRegion.GetTop()) * m_linesDifference + int(imageRegion.GetLeft());
	
	m_endIndex = istd::CIndex2d(int(imageRegion.GetWidth()), int(imageRegion.GetHeight()));
	m_index = istd::CIndex2d(int(imageRegion.GetLeft()), int(imageRegion.GetTop()));
	m_startIndex = m_index;
}


template <typename PixelType>
TBitmapIterator<PixelType>::TBitmapIterator(const TBitmapIterator& iterator)
:	m_imageBufferBeginPtr(iterator.m_imageBufferBeginPtr),
	m_imageBufferEndPtr(iterator.m_imageBufferEndPtr),
	m_imageBufferPtr(iterator.m_imageBufferPtr),
	m_linesDifference(iterator.m_linesDifference),
	m_endIndex(iterator.m_endIndex),
	m_index(iterator.m_index),
	m_startIndex(iterator.m_startIndex)
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
	return (m_imageBufferPtr >= m_imageBufferBeginPtr && 
			m_imageBufferPtr < m_imageBufferEndPtr);
}


template <typename PixelType>
inline bool TBitmapIterator<PixelType>::AtEnd() const
{
	return (m_index < m_startIndex || m_index >= m_endIndex);
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
	TBitmapIterator<PixelType> temp(*this);
	
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
	TBitmapIterator<PixelType> temp(*this);
	
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
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator-(int difference) const
{
	TBitmapIterator<PixelType> output(*this);

	output.Move(-difference);

	return output;
}


template <typename PixelType>
inline TBitmapIterator<PixelType> TBitmapIterator<PixelType>::operator+(int difference) const
{
	TBitmapIterator<PixelType> output(*this);

	output.Move(difference);

	return output;
}


// private methods

template <typename PixelType>
inline void TBitmapIterator<PixelType>::Move(int difference)
{
	int currentX = m_index.GetX();
	int currentY = m_index.GetY();

	int dx = difference % m_linesDifference;
	int dy = difference / m_linesDifference;

	currentX += dx;

	if (currentX >= m_linesDifference){
		currentX %= m_linesDifference;
		++dy;
	}
	else if(currentX < 0){
		currentX = m_linesDifference + currentX;
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

	if (currentX >= m_endIndex.GetX()){
		currentX %= m_endIndex.GetX();

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
		currentX = m_endIndex.GetX() + currentX;

		m_index.SetY(m_index.GetY() - 1);
	}

	m_index.SetX(currentX);

	MoveTo(m_index);
}


} // namespace iimg


#endif // !iimg_TBitmapIterator_included

