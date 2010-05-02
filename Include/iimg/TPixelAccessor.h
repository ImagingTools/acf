#ifndef iimg_TPixelAccessor_included
#define iimg_TPixelAccessor_included


namespace iimg
{

/**
	Access to the pixel data of an image.
*/
template <typename PixelType>
class TPixelAccessor
{
public:
	typedef typename PixelType ValueType;

	TPixelAccessor(int componentsCount);

	void SetPixelBuffer(ValueType* imageBufferPtr);
	ValueType* GetPixelBuffer();
	const ValueType* GetPixelBuffer() const;
	int GetComponentsCount() const;
	const ValueType& operator[](int index) const;
	ValueType& operator[](int index);

private:
	PixelType* m_imageBufferPtr;

	int m_componentsCount;
};


// public methods

template <typename PixelType>
TPixelAccessor<PixelType>::TPixelAccessor(int componentsCount)
	:m_imageBufferPtr(NULL),
	m_componentsCount(componentsCount)
{
}


template <typename PixelType>
inline void TPixelAccessor<PixelType>::SetPixelBuffer(ValueType* imageBufferPtr)
{
	m_imageBufferPtr = imageBufferPtr;
}


template <typename PixelType>
inline typename TPixelAccessor<PixelType>::ValueType* TPixelAccessor<PixelType>::GetPixelBuffer()
{
	return m_imageBufferPtr;
}


template <typename PixelType>
inline const typename TPixelAccessor<PixelType>::ValueType* TPixelAccessor<PixelType>::GetPixelBuffer() const
{
	return m_imageBufferPtr;
}


template <typename PixelType>
inline int TPixelAccessor<PixelType>::GetComponentsCount() const
{
	return m_componentsCount;
}


template <typename PixelType>
inline const typename TPixelAccessor<PixelType>::ValueType& TPixelAccessor<PixelType>::operator[](int index) const
{
	return *(m_imageBufferPtr + index);
}


template <typename PixelType>
inline typename TPixelAccessor<PixelType>::ValueType& TPixelAccessor<PixelType>::operator[](int index)
{
	return *(m_imageBufferPtr + index);
}


} // namespace iimg


#endif // !iimg_TPixelAccessor_included

