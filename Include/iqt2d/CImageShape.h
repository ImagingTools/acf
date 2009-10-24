#ifndef iqt2d_CImageShape_included
#define iqt2d_CImageShape_included


#include <QGraphicsRectItem>

#include "istd/CIndex2d.h"

#include "imod/TSingleModelObserverBase.h"

#include "iimg/IBitmap.h"

#include "icmm/IColorTransformation.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CImageShape: public QGraphicsItem, public imod::TSingleModelObserverBase<iimg::IBitmap>
{
public:
	CImageShape(const icmm::IColorTransformation* colorTransformationPtr = NULL);

	istd::CIndex2d GetSize() const;

	bool IsFrameVisible() const;
	void SetFrameVisible(bool state = true);

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void SetLookupTableToImage(QImage& image, const icmm::IColorTransformation& colorTransformation);

private:
	bool m_isFrameVisible;
	QPixmap m_backgroundPixmap;

	QPixmap m_bitmap;

	const icmm::IColorTransformation* m_colorTransformationPtr;
};


// inline methods

inline istd::CIndex2d CImageShape::GetSize() const
{
	return iqt::GetCIndex2d(m_bitmap.size());
}


inline bool CImageShape::IsFrameVisible() const
{
	return m_isFrameVisible;
}


inline void CImageShape::SetFrameVisible(bool state)
{
	if (state != m_isFrameVisible){
		m_isFrameVisible = state;

		update();
	}
}


} // namespace iqt2d


#endif // !iqt2d_CImageShape_included


