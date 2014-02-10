#ifndef iimg_CMultiLayerBitmap_included
#define iimg_CMultiLayerBitmap_included


// ACF includes
#include "iimg/IMultiLayerBitmap.h"
#include "istd/TPointerVector.h"


namespace iimg
{


class CMultiLayerBitmap: public iimg::IMultiLayerBitmap
{
public:
	// reimplemented (iimg::IMultiBitmapProvider)
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;

	// reimplemented (iimg::IMultiLayerBitmap)
	virtual void Reset();
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size);
	virtual iimg::IBitmap* InsertBitmap(
		iimg::IBitmap::PixelFormat pixelFormat,
		const istd::CIndex2d& size,
		void* dataPtr,
		bool releaseFlag,
		int linesDifference = 0);
	virtual void RemoveBitmap(int index);

protected:
	typedef istd::TPointerVector<iimg::IBitmap> BitmapVector;

	BitmapVector m_bitmaps;
};



// reimplemented (iimg::IMultiBitmapProvider)

inline int CMultiLayerBitmap::GetBitmapsCount() const
{
	return m_bitmaps.GetCount();
}


inline const iimg::IBitmap* CMultiLayerBitmap::GetBitmap(int bitmapIndex) const
{
	if (bitmapIndex >= 0 && bitmapIndex < m_bitmaps.GetCount()){
		return m_bitmaps.GetAt(bitmapIndex);
	}

	return NULL;
}


inline const iprm::IOptionsList* CMultiLayerBitmap::GetBitmapListInfo() const
{
	return NULL;
}


// reimplemented (iimg::IMultiLayerBitmap)

inline void CMultiLayerBitmap::Reset()
{
	m_bitmaps.Reset();
}


inline void CMultiLayerBitmap::RemoveBitmap(int index)
{
	m_bitmaps.RemoveAt(index);
}


} // namespace iimg


#endif
