#ifndef iimg_CScanlineMask_included
#define iimg_CScanlineMask_included


// STL includes
#include <vector>

// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/TRanges.h>
#include <i2d/CObject2dBase.h>
#include <i2d/CRect.h>
#include <i2d/CCircle.h>
#include <i2d/CAnnulus.h>
#include <i2d/CPolygon.h>
#include <i2d/CTubePolyline.h>
#include <iimg/IBitmap.h>


namespace iimg
{


/**
	Representation of a 2D-region as container of bitmap line scans.

	\ingroup ImageProcessing
	\ingroup Geometry
*/
class CScanlineMask:
			public i2d::CObject2dBase,
			virtual public IRasterImage
{
public:
	typedef i2d::CObject2dBase BaseClass;
	typedef QList<istd::CIntRanges> RangesContainer;
	typedef std::vector<int> Scanlines;	// Index of container element for each scan line, negative value for empty element

	CScanlineMask();

	/**
		Check if region is empty.
	*/
	bool IsBitmapRegionEmpty() const;

	/**
		Get rectangle containing all active pixels.
		If this mask is infinite, it returns \c i2d::CRect::GetInvalid() rectangle.
		Agains the method \c GetBoundingBox() from \c i2d::IObject2d it returns integer rectangle.
	*/
	i2d::CRect GetBoundingRect() const;

	/**
		Set this mask to empty set for some vertical range.
	*/
	void ResetScanlines(const istd::CIntRange& verticalRange);

	/**
		Get the list of pixel ranges per given line.
	*/
	const istd::CIntRanges* GetPixelRanges(int lineIndex) const;

	/**
		Create filled 2D-region clipped to rectangle area.
		\param	clipArea	clipping area.
	*/
	void CreateFilled(const i2d::CRect& clipArea);
	/**
		Create 2D-region from some geometrical object.
		\param	geometry	some geometrical object.
		\param	clipAreaPtr	optional clipping area.
	*/
	bool CreateFromGeometry(const i2d::IObject2d& geometry, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from circle.
		\param	circle		circle object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromCircle(const i2d::CCircle& circle, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from rectangle.
		\param	rect		rectangle object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromRectangle(const i2d::CRectangle& rect, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from annulus.
		\param	annulus		annulus object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromAnnulus(const i2d::CAnnulus& annulus, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from polygon.
		\param	polygon		polygon object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromPolygon(const i2d::CPolygon& polygon, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from tube.
		\param	tube		tube object.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromTube(const i2d::CTubePolyline& tube, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Create 2D-region from a bitmap.
		\param	bitmap		Mask bitmap. Non-zero pixels of the image represent the object in the resulting mask.
		\param	clipAreaPtr	optional clipping area.
	*/
	void CreateFromBitmap(const iimg::IBitmap& bitmap, const i2d::CRect* clipAreaPtr = NULL);

	/**
		Get inverted mask.
		\param	clipArea	clipping area.
		\param	result		result object, will be filled with inverted mask.
	*/
	void GetInverted(const i2d::CRect& clipArea, CScanlineMask& result) const;

	/**
		Invert mask on place.
		\param	clipArea	clipping area.
	*/
	void Invert(const i2d::CRect& clipArea);

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
		Calculate erosion of this range list with rectangle structured element.
		Result is stored in this object.
	*/
	void Erode(int leftValue, int rightValue, int topValue, int bottomValue);

	/**
		Calculate dilatation of this range list with rectangle structured element.
		Result is stored in this object.
	*/
	void Dilate(int leftValue, int rightValue, int topValue, int bottomValue);

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const i2d::CVector2d& position) override;
	virtual i2d::CRectangle GetBoundingBox() const override;

	// reimplemented (iimg::IRasterImage)
	virtual bool IsEmpty() const override;
	virtual void ResetImage() override;
	virtual void ClearImage() override;
	virtual istd::CIndex2d GetImageSize() const override;
	virtual int GetComponentsCount() const override;
	virtual icmm::CVarColor GetColorAt(const istd::CIndex2d& position) const override;
	virtual bool SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color) override;
	virtual const icmm::IColorModel* GetColorModel() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

	bool operator==(const CScanlineMask& mask) const;
	bool operator!=(const CScanlineMask& mask) const;

	friend uint qHash(const CScanlineMask& key, uint seed);

protected:
	void EnsureBoundingBoxValid() const;
	void CalcBoundingBox() const;
	void InitFromBoundingBox(const i2d::CRectangle& objectBoundingBox, const i2d::CRect* clipAreaPtr);

	template <typename PixelType>
	void CalculateMaskFromBitmap(const iimg::IBitmap& bitmap, const i2d::CRect* clipAreaPtr = NULL);

private:
	RangesContainer m_rangesContainer;

	Scanlines m_scanlines;

	int m_firstLinePos;

	mutable i2d::CRect m_boundingBox;
	mutable bool m_isBoundingBoxValid;
};


// public inline methods

inline bool CScanlineMask::operator!=(const CScanlineMask& mask) const
{
	return !operator==(mask);
}


// protected inline methods

inline void CScanlineMask::EnsureBoundingBoxValid() const
{
	if (!m_isBoundingBoxValid){
		CalcBoundingBox();
	}
}


// protected methods

template <typename PixelType>
void CScanlineMask::CalculateMaskFromBitmap(const iimg::IBitmap& bitmap, const i2d::CRect* clipAreaPtr)
{
	InitFromBoundingBox(bitmap.GetBoundingBox(), clipAreaPtr);

	int linesCount = int(m_scanlines.size());
	if (linesCount <= 0){
		ResetImage();

		return;
	}

#if QT_VERSION >= 0x040700
	m_rangesContainer.reserve(linesCount);
#endif

	Q_ASSERT(bitmap.GetImageSize().GetY() == (linesCount));

	int imageWidth = bitmap.GetImageSize().GetX();

	for (int lineIndex = 0; lineIndex < linesCount; lineIndex++){
		istd::CIntRanges rangeList;

		PixelType* imageLinePtr = (PixelType*)bitmap.GetLinePtr(lineIndex);
		Q_ASSERT(imageLinePtr != NULL);

		int left = -1;
		int right = -1;

		for (int x = 0; x < imageWidth; ++x){
			PixelType pixel = *(imageLinePtr + x);
			if ((pixel > 0) && (left < 0)){
				left = x;
			}

			if ((pixel == 0) && (left >= 0)){
				right = x;
				if (clipAreaPtr != NULL){
					if (left < clipAreaPtr->GetLeft()){
						left = clipAreaPtr->GetLeft();
					}

					if (right > clipAreaPtr->GetRight()){
						right = clipAreaPtr->GetRight();
					}
				}

				if (left < right){
					rangeList.InsertSwitchPoint(left);
					rangeList.InsertSwitchPoint(right);
				}

				left = -1;
				right = -1;
			}
		}

		if ((left >= 0) && (right < 0)){
			rangeList.InsertSwitchPoint(left);
		}

		if (!rangeList.IsEmpty()){
			m_rangesContainer.push_back(rangeList);

			m_scanlines[lineIndex] = m_rangesContainer.size() - 1;
		}
		else{
			m_scanlines[lineIndex] =  -1;
		}
	}
}


// related global functions

uint qHash(const CScanlineMask& key, uint seed = 0);


} // namespace iimg


#endif // !iimg_CScanlineMask_included


