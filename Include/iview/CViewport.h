#ifndef iview_CConsoleWidget_included
#define iview_CConsoleWidget_included


// Qt includes
#include <QtGui/QPainter>
#include <QtGui/QWidget>
#include <QtGui/QCursor>
#include <QtGui/QPainter>

// ACF includes
#include "iimg/IBitmap.h"

#include "imath/CFixedPointManip.h"

#include "iview/IViewEventObserver.h"
#include "iview/CConsoleBase.h"
#include "iview/CCalibratedViewBase.h"
#include "iview/CImageShape.h"


namespace iview
{


class CViewport:
			public QWidget,
			public CCalibratedViewBase,
			virtual public IViewEventObserver
{
	Q_OBJECT

public:
	typedef CCalibratedViewBase BaseClass;
	typedef QWidget BaseClass2;

	CViewport(CConsoleBase* framePtr, QWidget* parent = NULL);
	virtual ~CViewport();

	CConsoleBase* GetFramePtr() const;

	void UpdateFitTransform();

	virtual void SetEditMode(int mode);

	/**
		Get number of digits after point used to display mouse position in pixel.
	*/
	int GetPixelPositionExact() const;
	
	/**
		Get number of digits after point used to display mouse position in logical units.
	*/
	int GetLogicalPositionExact() const;

	// reimplemented (iview::IShapeView)
	virtual void SetFitArea(const i2d::CRectangle& area);
	virtual i2d::CRect GetClientRect() const;

	// reimplemented (IViewEventObserver)
	virtual bool OnSelectChange(
				const iview::IShapeView& view,
				const istd::CIndex2d& position,
				const iview::IInteractiveShape& shape,
				bool state);
	virtual bool OnMouseButton(
				const iview::IShapeView& view,
				const istd::CIndex2d& position,
				Qt::MouseButton buttonType,
				bool state,
				const iview::IInteractiveShape* shapePtr);

	// reimplemented (IMouseActionObserver)
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::CCalibratedViewBase)
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr);

Q_SIGNALS:
	/**
		Called when some shapes has been changed.
	*/
	void ShapesChanged();

protected:
	virtual void SetBackgroundBufferValid(bool state = true);
	virtual void OnBoundingBoxChanged();
	virtual void OnResize();
	virtual bool CanBeMoved() const;

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event);
	virtual void resizeEvent (QResizeEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);

	// reimplemented (iview::CViewBase)
	virtual void SetMousePointer(MousePointerMode mode);
	void UpdateRectArea(const i2d::CRect& rect);

	// reimplemented (CViewBase)
	virtual i2d::CRect CalcBoundingBox() const;

	// reimplemented (IDisplay)
	virtual void OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox);

	// static methods
	int GetKeysState(const QMouseEvent& event);

private:
	QCursor m_mousePointerModes[MPM_LAST + 1];

	mutable bool m_blockBBoxEvent;
	mutable i2d::CRect m_lastBoundingBox;

	imath::CFixedPointManip m_pixelPositionFormatter;
	imath::CFixedPointManip m_logicalPositionFormatter;
	CConsoleBase* m_framePtr;

	friend class CConsoleBase;
};


} // namespace iview


#endif // !iview_CConsoleWidget_included


