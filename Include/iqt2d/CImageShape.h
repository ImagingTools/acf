#ifndef iqt2d_CImageShape_included
#define iqt2d_CImageShape_included


// Qt includes
#include <QGraphicsRectItem>


// ACF includes
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
	enum PositionMode
	{
		/**
			Left top corner will be associated with position (0, 0).
		*/
		PM_CORNER_LT,
		/**
			Center of bitmap will be associated with position (0, 0).
		*/
		PM_CENTER
	};

	CImageShape(const icmm::IColorTransformation* colorTransformationPtr = NULL);

	istd::CIndex2d GetSize() const;

	/**
		Check if frame is visible.
		The otlining frame is used to show real image borders.
	*/
	bool IsFrameVisible() const;
	/**
		Turn frame visibility on/off.
		The otlining frame is used to show real image borders.
	*/
	void SetFrameVisible(bool state = true);

	/**
		Get position mode defined as PositionMode.
	*/
	int GetPositionMode() const;
	/**
		Set position mode defined as PositionMode.
	*/
	void SetPositionMode(int mode);

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

private:
	void SetLookupTableToImage(QImage& image, const icmm::IColorTransformation& colorTransformation);

private:
	bool m_isFrameVisible;
	int m_positionMode;

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


inline int CImageShape::GetPositionMode() const
{
	return m_positionMode;
}


inline void CImageShape::SetPositionMode(int mode)
{
	if (mode != m_positionMode){
		m_positionMode = mode;

		update();
	}
}


} // namespace iqt2d


#endif // !iqt2d_CImageShape_included


