#include <iview/CViewport.h>


// Qt includes
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>


namespace iview
{


// public methods

CViewport::CViewport(CConsoleBase* framePtr, QWidget* parent)
:	BaseClass2(parent),
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

	setMouseTracking(true);

	AddViewEventObserver(this);
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
	}

	m_framePtr->UpdateButtonsState();
}


void CViewport::SetEditMode(int mode)
{
	if (mode != GetEditMode()){
		BaseClass::SetEditMode(mode);

		m_framePtr->UpdateEditModeButtons();
	}
}


// reimplemented (iview::IShapeView)

void CViewport::SetFitArea(const i2d::CRectangle& area)
{
	i2d::CRectangle prevFitArea = GetFitArea();

	BaseClass::SetFitArea(area);

	if (prevFitArea != area){
		if (m_framePtr->IsZoomToFit()){
			UpdateFitTransform();
		}
		else{
			SetBackgroundBufferValid(false);

			m_framePtr->UpdateButtonsState();
		}
	}
}


i2d::CRect CViewport::GetClientRect() const
{
	return i2d::CRect(iqt::GetCIndex2d(size()));
}


// reimplemented (iview::IViewEventObserver)

bool CViewport::OnSelectChange(
		const iview::IShapeView& view,
		const istd::CIndex2d& position,
		const iview::IInteractiveShape& shape,
		bool state)
{
	return m_framePtr->OnSelectChange(view, position, shape, state);
}


bool CViewport::OnViewMouseButton(
		const IShapeView& view,
		const istd::CIndex2d& position,
		Qt::MouseButton buttonType,
		bool state,
		const IInteractiveShape* shapePtr)
{
	if (!state){
		m_framePtr->UpdateButtonsState();
	}

	return m_framePtr->OnViewMouseButton(view, position, buttonType, state, shapePtr);
}


// reimplemented (iview::IMouseActionObserver)

bool CViewport::OnMouseMove(istd::CIndex2d position)
{
	bool retVal = BaseClass::OnMouseMove(position);

	// info text
	QString infoText;

	// pixel position
	const iview::CScreenTransform& transform = GetTransform();
	i2d::CVector2d  pixelPosition = transform.GetClientPosition(position);

	// logical position
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		i2d::CVector2d logPosition;
		if (calibrationPtr->GetInvPositionAt(pixelPosition, logPosition)){
			const imath::IUnitInfo* logicalUnitInfoPtr = calibrationPtr->GetArgumentUnitInfo();

			QString textX = QString::number(logPosition.GetX(), 'f', 2);
			QString textY = QString::number(logPosition.GetY(), 'f', 2);
			QString logicalUnitName = (logicalUnitInfoPtr != NULL)?
						logicalUnitInfoPtr->GetUnitName():
						tr("mm");
			infoText += tr("[%1, %2] %3").arg(textX, 6).arg(textY, 6).arg(logicalUnitName);
		}
	}

	for (int i = 0; i < GetLayersCount(); i++){
		QString infoPart = GetLayer(i).GetShapeDescriptionAt(position);
		if (!infoPart.isEmpty()){
			if (!infoText.isEmpty()){
				infoText += tr(", ");
			}
			infoText += infoPart;
		}
	}

	m_framePtr->UpdateCursorInfo(infoText);

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

void CViewport::SetBackgroundBufferValid(bool state)
{
	BaseClass::SetBackgroundBufferValid(state);
	if (!state && (m_framePtr != NULL)){
		int backgroundLayerIndex = GetBackgroundLayerIndex();
		if (backgroundLayerIndex >= 0){
			i2d::CRect fitArea = GetFitArea();

			bool isBkActive = !fitArea.IsEmpty();
			if (isBkActive != m_framePtr->m_isBkActive){
				m_framePtr->m_isBkActive = isBkActive;
				if (m_framePtr->m_isZoomToFit){
					if (isBkActive){
						UpdateFitTransform();
					}
					else{
						SetZoom(iview::CViewBase::ZM_RESET);
					}
				}
			}
		}
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


// reimplemented Qt (QWidget)

void CViewport::paintEvent(QPaintEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	DrawBuffers(*this, iqt::GetCRect(eventPtr->rect()));
}


void CViewport::resizeEvent(QResizeEvent* /*eventPtr*/)
{
	CheckResize();
}


void CViewport::mousePressEvent(QMouseEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	SetKeysState(GetMouseKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());
	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = BaseClass::OnMouseButton(pos, buttonState, true);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass2::mousePressEvent(eventPtr);
	}
}


void CViewport::mouseReleaseEvent(QMouseEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	SetKeysState(GetMouseKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	Qt::MouseButton buttonState = eventPtr->button();

	bool isConsumed = BaseClass::OnMouseButton(pos, buttonState, false);
	if (isConsumed){
		Update();
	}
	else{
		BaseClass2::mousePressEvent(eventPtr);
	}
}


void CViewport::mouseMoveEvent(QMouseEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	SetKeysState(GetMouseKeysState(*eventPtr));

	istd::CIndex2d pos = iqt::GetCIndex2d(eventPtr->pos());

	OnMouseMove(pos);

	BaseClass2::mouseMoveEvent(eventPtr);
}


// reimplemented (iview::CViewBase)

void CViewport::SetMousePointer(MousePointerMode mode)
{
	Q_ASSERT(mode >= 0);
	Q_ASSERT(mode < MPM_LAST);

	BaseClass2::setCursor(m_mousePointerModes[mode]);
}


void CViewport::UpdateRectArea(const i2d::CRect& rect)
{
	QRect qrect(iqt::GetQRect(rect));

	Q_EMIT update(qrect);
}


void CViewport::OnBoundingBoxChanged()
{
	if (m_framePtr != NULL){
		m_framePtr->OnBoundingBoxChanged();
	}
}


// static protected methods

int CViewport::GetMouseKeysState(const QMouseEvent& mouseEvent)
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


// reimplemented (iview::IDisplay)

void CViewport::OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox)
{
	BaseClass::OnAreaInvalidated(beforeBox, afterBox);

	EnsureBoundingBoxValid();

	Q_EMIT ShapesChanged();
}



} // namespace iview


