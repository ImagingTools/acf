#include "iview/CViewport.h"


// Qt includes
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>


namespace iview
{


// public methods

CViewport::CViewport(CConsoleBase* framePtr, QWidget* parent)
	:BaseClass(parent),
	m_pixelPositionFormatter(0),
	m_logicalPositionFormatter(2),
	m_framePtr(framePtr)
{
	m_mousePointerModes[MPM_NONE] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_DEFAULT] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_DESELECT] = QCursor(Qt::UpArrowCursor);
	m_mousePointerModes[MPM_POINTER] = QCursor(Qt::ArrowCursor);
	m_mousePointerModes[MPM_WAIT] = QCursor(Qt::WaitCursor);
	m_mousePointerModes[MPM_CROSS] = QCursor(Qt::CrossCursor);
	m_mousePointerModes[MPM_HAND] = QCursor(Qt::PointingHandCursor);
	m_mousePointerModes[MPM_DRAG] = QCursor(Qt::SizeAllCursor);
	m_mousePointerModes[MPM_SCREEN_MOVE] = QCursor(Qt::PointingHandCursor);

	setAttribute(Qt::WA_NoBackground);
	setAttribute(Qt::WA_PaintOnScreen);

	setMouseTracking(true);

	m_blockBBoxEvent = false;

	AddViewEventObserver(this);

	m_lastBoundingBox.Reset();
}


CViewport::~CViewport()
{
	m_framePtr = NULL;
}



CConsoleBase* CViewport::GetFramePtr() const
{
	return m_framePtr;
}




void CViewport::UpdateFitTransform()
{
	if (m_framePtr->m_isZoomToFit){
		m_framePtr->m_isZoomToFit = false;
		switch (m_framePtr->m_fitMode){
		case CConsoleBase::FM_RESET:
			SetZoom(iview::CViewBase::ZM_RESET);
			break;

		case CConsoleBase::FM_BOTH:
			SetZoom(iview::CViewBase::ZM_FIT);
			break;

		case CConsoleBase::FM_HORIZONTAL:
			SetZoom(iview::CViewBase::ZM_FIT_H);
			break;

		case CConsoleBase::FM_VERTICAL:
			SetZoom(iview::CViewBase::ZM_FIT_V);
			break;

		case CConsoleBase::FM_UNPROP:
			SetZoom(iview::CViewBase::ZM_FIT_UNPROP);
			break;

		case CConsoleBase::FM_COVER:
			SetZoom(iview::CViewBase::ZM_FIT_COVER);
			break;
		}
		m_framePtr->m_isZoomToFit = true;
	}

	CalcBoundingBox();

	i2d::CRect boundingBox = GetBoundingBox();
	if (boundingBox != GetBoundingBox()){
		OnBoundingBoxChanged();
	}
}


void CViewport::SetEditMode(int mode)
{
	if (mode != GetEditMode()){
		BaseClass::SetEditMode(mode);
		
		m_framePtr->UpdateEditModeButtons();
	}
}


void CViewport::SetFitArea(const i2d::CRectangle& area)
{
	i2d::CRectangle prevFitArea = GetFitArea();

	BaseClass::SetFitArea(area);

	if ((prevFitArea != area) && m_framePtr->IsZoomToFit()){
		UpdateFitTransform();
	}
}


// reimplemented (iview::IShapeView)

i2d::CRect CViewport::GetClientRect() const
{
	return i2d::CRect(iqt::GetCIndex2d(size()));
}


// reimplement (IViewEventObserver)

bool CViewport::OnSelectChange(
			const iview::IShapeView& view,
			const istd::CIndex2d& position,
			const iview::IInteractiveShape& shape,
			bool state)
{
	return m_framePtr->OnSelectChange(view, position, shape, state);
}



bool CViewport::OnMouseButton(
			const IShapeView& view,
			const istd::CIndex2d& position,
			Qt::MouseButton buttonType,
			bool state,
			const IInteractiveShape* shapePtr)
{
	if (!state){
		m_framePtr->UpdateButtonsState();
	}

	return m_framePtr->OnMouseButton(view, position, buttonType, state, shapePtr);
}


// reimplemented (IMouseActionObserver)

bool CViewport::OnMouseMove(istd::CIndex2d position)
{
	bool retVal = BaseClass::OnMouseMove(position);

	CConsoleBase::CursorInfo info;

	// pixel position
	const iview::CScreenTransform& transform = GetTransform();
	info.pixelPos = transform.GetClientPosition(position);

	// color info
	int backgroundLayerIndex = GetBackgroundLayerIndex();
	if (backgroundLayerIndex >= 0){
		const iview::CSingleLayer* layerPtr = dynamic_cast<const iview::CSingleLayer*>(&GetLayer(backgroundLayerIndex));
		if (layerPtr != NULL){
			const iview::CImageShape* backgroundShapePtr = dynamic_cast<const iview::CImageShape*>(layerPtr->GetShapePtr());
			if (m_framePtr->IsPixelValueVisible() && backgroundShapePtr != NULL){
				const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(backgroundShapePtr->GetModelPtr());
				if (bitmapPtr != NULL){
					istd::CIndex2d bitmapPos = info.pixelPos.ToIndex2d();
					ibase::CSize bitmapSize = bitmapPtr->GetImageSize();
					if (			(bitmapPos.GetX() >= 0) &&
									(bitmapPos.GetY() >= 0) &&
									(bitmapPos.GetX() < bitmapSize.GetX()) &&
									(bitmapPos.GetY() < bitmapSize.GetY())){
						int pixelMode = bitmapPtr->GetPixelFormat();

						icmm::CVarColor pixelValue = bitmapPtr->GetColorAt(bitmapPos);
						
						switch (pixelMode){
							case iimg::IBitmap::PF_GRAY:
							{
								info.pixelBrightness = pixelValue[0] * 100;
								break;
							}

							case iimg::IBitmap::PF_RGB:
							{
								info.red = pixelValue[0] * 100;
								info.green = pixelValue[1] * 100;
								info.blue = pixelValue[2] * 100;
								break;
							}

							case iimg::IBitmap::PF_RGBA:
							{
								info.red = pixelValue[0] * 100;
								info.green = pixelValue[1] * 100;
								info.blue = pixelValue[2] * 100;
								info.alpha = pixelValue[3] * 100;
								break;
							}
						}
					}
				}
			}
		}
	}

	i2d::CVector2d logPosition;
	if (m_framePtr->IsMmPositionVisible()){
		const iview::CScreenTransform& logToScreenTransform = GetLogToScreenTransform();

		const i2d::ITransformation2d* calibrationPtr = GetCalibration();
		const i2d::ITransformation2d* isomorphCalibPtr = dynamic_cast<const i2d::ITransformation2d*>(calibrationPtr);
		if (isomorphCalibPtr != NULL){
			isomorphCalibPtr->GetPositionAt(info.pixelPos, info.logicalPos);
		}
		else{
			info.logicalPos = logToScreenTransform.GetClientPosition(position);
		}
	}

	m_framePtr->UpdateCursorInfo(info);

	return retVal;
}

	
// reimplemented (iview::CCalibratedViewBase)

void CViewport::ConnectCalibrationShape(iview::IShape* shapePtr)
{
	BaseClass::ConnectCalibrationShape(shapePtr);

	CConsoleBase* framePtr = GetFramePtr();
	if (framePtr != NULL){
		framePtr->UpdateButtonsState();
	}
}


// protected methods

// reimplemented Qt (QWidget)

void CViewport::paintEvent(QPaintEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	DrawBuffers(*this, iqt::GetCRect(eventPtr->rect()));
}


void CViewport::resizeEvent(QResizeEvent* /*eventPtr*/)
{
	CheckResize();
}


void CViewport::mousePressEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());
	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = BaseClass::OnMouseButton(pos, buttonState, true);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass::mousePressEvent(eventPtr);
	}
}


void CViewport::mouseReleaseEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = BaseClass::OnMouseButton(pos, buttonState, false);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass::mousePressEvent(eventPtr);
	}
}


void CViewport::mouseMoveEvent(QMouseEvent* eventPtr)
{
	I_ASSERT(eventPtr != NULL);

	SetKeysState(GetKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	OnMouseMove(pos);

	BaseClass::mouseMoveEvent(eventPtr);
}


// reimplemented (iview::CViewBase)

void CViewport::SetMousePointer(MousePointerMode mode)
{
	I_ASSERT(mode >= 0);
	I_ASSERT(mode < MPM_LAST);

	BaseClass::setCursor(m_mousePointerModes[mode]);
}


void CViewport::UpdateRectArea(const i2d::CRect& rect)
{
	QRect qrect(iqt::GetQRect(rect));

	Q_EMIT update(qrect);
}


// static protected methods

int CViewport::GetKeysState(const QMouseEvent& mouseEvent)
{
	Qt::MouseButtons buttons = mouseEvent.buttons();
	Qt::KeyboardModifiers modifiers = mouseEvent.modifiers();

	int retVal = 0;

	if ((buttons & Qt::LeftButton) != 0){
		retVal |= Qt::LeftButton;
	}
	if ((buttons & Qt::MidButton) != 0){
		retVal |= Qt::MidButton;
	}
	if ((buttons & Qt::RightButton) != 0){
		retVal |= Qt::RightButton;
	}
	if ((modifiers & Qt::ShiftModifier) != 0){
		retVal |= Qt::ShiftModifier;
	}
	if ((modifiers & Qt::ControlModifier) != 0){
		retVal |= Qt::ControlModifier;
	}

	return retVal;
}


// protected methods

void CViewport::SetBackgroundBufferValid(bool state)
{
	BaseClass::SetBackgroundBufferValid(state);
	if (!state){
		int backgroundLayerIndex = GetBackgroundLayerIndex();
		if (backgroundLayerIndex >= 0){
			const iview::CSingleLayer* layerPtr = dynamic_cast<const iview::CSingleLayer*>(&GetLayer(backgroundLayerIndex));
			if (layerPtr != NULL){
				const iview::CImageShape* backgroundPtr = dynamic_cast<const iview::CImageShape*>(layerPtr->GetShapePtr());
				bool isBkActive = (backgroundPtr != NULL) && !backgroundPtr->GetBoundingBox().IsEmpty();
				if ((m_framePtr != NULL) && (isBkActive != m_framePtr->m_isBkActive)){
					m_framePtr->m_isBkActive = isBkActive;
					if (m_framePtr->m_isZoomToFit){
						if (isBkActive){
							UpdateFitTransform();
						}
						else{
							SetZoom(iview::CViewBase::ZM_RESET);
						}
					}
					m_framePtr->UpdateButtonsState();
				}
			}
		}
	}
}


void CViewport::OnBoundingBoxChanged()
{
	if (m_framePtr != NULL){
		m_framePtr->OnBoundingBoxChanged();
	}
}


void CViewport::OnResize()
{
	BaseClass::OnResize();

	UpdateFitTransform();
	if (m_framePtr != NULL){
		m_framePtr->UpdateComponentsPosition();
	}
}


bool CViewport::CanBeMoved() const
{
	if (m_framePtr != NULL){
		return !m_framePtr->m_isZoomToFit && BaseClass::CanBeMoved();
	}

	return false;
}


// reimplemented (iview::CViewBase)

void CViewport::CalcBoundingBox() const
{
	if (!m_blockBBoxEvent){
		m_blockBBoxEvent = true;

		const i2d::CRect& lastBox = GetBoundingBox();
		BaseClass::CalcBoundingBox();
		if (lastBox != GetBoundingBox()){
			const_cast<CViewport*>(this)->OnBoundingBoxChanged();
		}

		m_blockBBoxEvent = false;
	}
	else{
		BaseClass::CalcBoundingBox();
	}
}


// reimplemented (iview::IDisplay)

void CViewport::OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox)
{
	BaseClass::OnAreaInvalidated(beforeBox, afterBox);

	if (!m_blockBBoxEvent){
		m_blockBBoxEvent = true;
		i2d::CRect boundingBox = GetBoundingBox();
		if (boundingBox != m_lastBoundingBox){
			OnBoundingBoxChanged();
			m_lastBoundingBox = boundingBox;
		}

		m_blockBBoxEvent = false;
	}
}



} // namespace iview


