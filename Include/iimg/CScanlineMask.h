#ifndef iimg_CScanlineMask_included
#define iimg_CScanlineMask_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/TRanges.h"
#include "i2d/CObject2dBase.h"
#include "i2d/CRect.h"
#include "i2d/CCircle.h"
#include "i2d/CAnnulus.h"
#include "i2d/CPolygon.h"
#include "i2d/CTubePolyline.h"
#include "iimg/IRasterImage.h"


namespace iimg
{


/**
	Representation of a 2D-region as container of bitmap line scans.
*/
class CScanlineMask:
			public i2d::CObject2dBase,
			virtual public IRasterImage
{
public:
	typedef QList<istd::CIntRanges> RangesContainer;
	typedef QVector<const istd::CIntRanges*> Scanlines;

	CScanlineMask();

	/**
		Check if region is empty.
	*/
	bool IsBitmapRegionEmpty() const;

	/**
		Set this mask to empty set for some vertical range.
	*/
	void ResetScanlines(const istd::CIntRange& verticalRange);

	/**
		Get the list of pixel ranges per given line.
	*/
	const istd::CIntRanges* GetPixelRanges(int lineIndex) const;

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
		\param	annulus		annulus object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr);
	/**
		Create 2D-region from polygon.
		\param	polygon		polygon object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr);
	/**
		Create 2D-region from tube.
		\param	tube		tube object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromTube(const i2d::CTubePolyline& tube, const i2d::CRect* clipAreaPtr);

	/**
		Get union of two masks.
	*/
	CScanlineMask GetUnion(const CScanlineMask& mask) const;
	/**
		Get union of two masks.
		\overload
	*/
	void GetUnion(const CScanlineMask& mask, CScanlineMask& result) const;
	/**
		Calculate union of this mask and the other one.
		Result is stored in this object.
	*/
	void Union(const CScanlineMask& mask);
	/**
		Get intersection of two masks.
	*/
	CScanlineMask GetIntersection(const CScanlineMask& mask) const;
	/**
		Get intersection of two masks.
		\overload
	*/
	void GetIntersection(const CScanlineMask& mask, CScanlineMask& result) const;
	/**
		Calculate intersection of this mask and the other one.
		Result is stored in this object.
	*/
	void Intersection(const CScanlineMask& mask);

	/**
		Calculate translated (moved) mask.
	*/
	CScanlineMask GetTranslated(int dx, int dy) const;
	/**
		Calculate translated (moved) mask.
		\overload
	*/
	void GetTranslated(int dx, int dy, CScanlineMask& result) const;
	/**
		Translated (move) this mask.
	*/
	void Translate(int dx, int dy);

	/**
		Get access to internal range container.
	*/
	RangesContainer& GetRangesContainer();
	/**
		Get access to internal scan lines.
	*/
	Scanlines& GetScanLines();

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveCenterTo(const i2d::CVector2d& position);
	virtual i2d::CRectangle GetBoundingBox() const;

	// reimplemented (iimg::IRasterImage)
	virtual bool IsEmpty() const;
	virtual void ResetImage();
	virtual void ClearImage();
	virtual istd::CIndex2d GetImageSize() const;
	virtual int GetComponentsCount() const;
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const;
	virtual bool SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void InitFromBoudingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr);

private:
	RangesContainer m_rangesContainer;

	Scanlines m_scanlines;

	int m_firstLinePos;

	mutable i2d::CRect m_boundingBox;
	mutable bool m_isBoundingBoxValid;
};


// public inline methods

inline CScanlineMask::RangesContainer& CScanlineMask::GetRangesContainer()
{
	return m_rangesContainer;
}


inline CScanlineMask::Scanlines& CScanlineMask::GetScanLines()
{
	return m_scanlines;
}


} // namespace iimg


#endif // !iimg_CScanlineMask_included



