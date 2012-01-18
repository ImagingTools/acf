#ifndef iimg_CBitmapRegion_included
#define iimg_CBitmapRegion_included


// STL includes
#include <vector>


// ACF includes
#include "i2d/CRectangle.h"
#include "i2d/CCircle.h"
#include "i2d/CAnnulus.h"

#include "iimg/IBitmap.h"


namespace iimg
{


/**
	Representation of a 2D-region as container of bitmap line scans.
*/
class CBitmapRegion: virtual public istd::IPolymorphic
{
public:
	struct PixelRange
	{
		PixelRange()
			:pixelBufferPtr(NULL)
		{
		}

		istd::CRange range;
		const void* pixelBufferPtr;
	};

	typedef std::vector<PixelRange> PixelRanges;

	CBitmapRegion(const IBitmap* bitmapPtr = NULL);

	/**
		Create 2D-region from the geometrical object.
	*/
	bool CreateFromGeometry(const i2d::IObject2d& geometry);

	/**
		Get the list of pixel ranges per given line.
	*/
	const PixelRanges* GetPixelRanges(int lineIndex) const;

	const i2d::CRectangle& GetBoundingBox() const;
	void ResetBitmapRegion();
	bool IsBitmapRegionEmpty() const;

	const IBitmap* GetBitmapPtr() const;

private:
	void CalculateRegionBoundingBox(const i2d::CRectangle& objectBoundingBox);
	void CreateFromCircle(const i2d::CCircle& circle);
	void CreateFromRectangle(const i2d::CRectangle& rect);
	void CreateFromAnnulus(const i2d::CAnnulus& annulus);

private:
	const IBitmap* m_bitmapPtr;
	std::vector<PixelRanges> m_rangesContainer;
	std::vector<const PixelRanges*> m_lineRangePtr;
	i2d::CRectangle m_boundingBox;
	bool m_isEmpty;
};


// public inline methods

inline const IBitmap* CBitmapRegion::GetBitmapPtr() const
{
	return m_bitmapPtr;
}


} // namespace iimg


#endif // !iimg_CBitmapRegion_included



