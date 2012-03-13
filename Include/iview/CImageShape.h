#ifndef iview_CImageShape_included
#define iview_CImageShape_included


// ACF includes
#include "icmm/IColorTransformation.h"

#include "iimg/IBitmap.h"

#include "iview/CShapeBase.h"


namespace iview
{


class CImageShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

	CImageShape(const icmm::IColorTransformation* colorTransformationPtr = NULL);

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iview::CShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

protected:
	virtual void DrawBitmap(
				QPainter& painter,
				const iimg::IBitmap& bitmap,
				const i2d::CRect& bitmapArea,
				const i2d::CAffine2d& destTransform) const;
private:
	void SetLookupTableToImage(QImage& image, const icmm::IColorTransformation& colorTransformation);

private:
	QPixmap m_pixmap;

	const icmm::IColorTransformation* m_colorTransformationPtr;

};


} // namespace iview


#endif // !iview_CImageShape_included



