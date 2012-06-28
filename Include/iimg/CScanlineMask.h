#ifndef iimg_CScanlineMask_included
#define iimg_CScanlineMask_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "i2d/CRect.h"
#include "i2d/CCircle.h"
#include "i2d/CAnnulus.h"
#include "i2d/CPolygon.h"

#include "iimg/IBitmap.h"


namespace iimg
{


/**
	Representation of a 2D-region as container of bitmap line scans.
*/
class CScanlineMask: virtual public istd::IPolymorphic
{
public:
	typedef QList<istd::CIntRange> PixelRanges;

	CScanlineMask();

	/**
		Check if region is empty.
	*/
	bool IsBitmapRegionEmpty() const;

	/**
		Get bounding box of this region.
	*/
	i2d::CRect GetBoundingBox() const;

	/**
		Get the list of pixel ranges per given line.
	*/
	const PixelRanges* GetPixelRanges(int lineIndex) const;

	void ResetBitmapRegion();

	/**
		Create 2D-region from some geometrical object.
		\param	geometry	some geometrical object.
		\param	clipAreaPtr	optional clipping area.
	*/
	bool CreateFromGeometry(const i2d::IObject2d& geometry, const i2d::CRect* clipAreaPtr);

	/**
		Create 2D-region from circle.
		\param	circle		circle object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr);
	/**
		Create 2D-region from rectangle.
		\param	rect		rectangle object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr);
	/**
		Create 2D-region from annulus.
		\param	rect		annulus object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr);
	/**
		Create 2D-region from polygon.
		\param	rect		polygon object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr);

	// static methods
	static void UnionLine(const PixelRanges& line, PixelRanges& result);
	static void IntersectLine(const PixelRanges& line, PixelRanges& result);

protected:
	void SetBoundingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr);

private:
	QVector<PixelRanges> m_rangesContainer;
	QVector<const PixelRanges*> m_lineRangePtr;
	i2d::CRect m_boundingBox;
	bool m_isEmpty;
};


} // namespace iimg


#endif // !iimg_CScanlineMask_included



