#pragma once


// Qt includes
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QPainter>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <iimg/IBitmap.h>
#include <imath/CFixedPointManip.h>
#include <iview/IViewEventObserver.h>
#include <iview/CConsoleBase.h>
#include <iview/CCalibratedViewBase.h>
#include <iview/CImageShape.h>


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

	bool MoveViewBy(int dx, int dy);

	void SetShowInfoText(bool on);

	void SetDrawBorder(bool on);

	void SetMousePointerCursor(int mode, const QCursor& cursor);

	virtual void SetEditMode(int mode);

	// reimplemented (iview::IShapeView)
	virtual void SetFitArea(const i2d::CRectangle& area) override;
	virtual i2d::CRect GetClientRect() const override;
	virtual void Update() override;

	// reimplemented (iview::IViewEventObserver)
	virtual bool OnSelectChange(
				const iview::IShapeView& view,
				const istd::CIndex2d& position,
				const iview::IInteractiveShape& shape,
				bool state) override;
	virtual bool OnViewMouseButton(
				const iview::IShapeView& view,
				const istd::CIndex2d& position,
				Qt::MouseButton buttonType,
				bool state,
				const iview::IInteractiveShape* shapePtr) override;
	virtual bool OnViewMouseMove(
				const iview::IShapeView& view,
				const istd::CIndex2d& position) override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::CCalibratedViewBase)
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr) override;

Q_SIGNALS:
	/**
		Called when some shapes has been changed.
	*/
	void ShapesChanged();

protected:
	virtual void SetBackgroundBufferValid(bool state = true);
	virtual void OnResize();
	virtual bool CanBeMoved() const;

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent (QResizeEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;

	// reimplemented (iview::CViewBase)
	virtual void SetMousePointer(MousePointerMode mode) override;
	virtual void UpdateRectArea(const i2d::CRect& rect) override;
	virtual void OnBoundingBoxChanged() override;

	// reimplemented (iview::IDisplay)
	virtual void OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox) override;

	// static methods
	int GetMouseKeysState(const QMouseEvent& event);

private:
	QCursor m_mousePointerModes[MPM_LAST];

	imath::CFixedPointManip m_pixelPositionFormatter;
	imath::CFixedPointManip m_logicalPositionFormatter;
	CConsoleBase* m_framePtr = nullptr;

	bool m_showInfoText = false;
	bool m_drawBorder = true;
	QString m_infoText;

	friend class CConsoleBase;
};


} // namespace iview


