#include "iview/CConsoleBase.h"


// Qt includes
#include <QtCore/QString>
#include <QtGui/QWidget>


// ACF includes
#include "i2d/CRectangle.h"

#include "iview/IViewEventObserver.h"
#include "iview/CScreenTransform.h"
#include "iview/CViewport.h"


namespace iview
{


// public methods


CConsoleBase::CConsoleBase(QWidget* parent)
	:QWidget(parent)
{
	Init();
}


void CConsoleBase::SetZoomToFit(bool state)
{
	if (state != m_isZoomToFit){
		m_isZoomToFit = state;
		
		CViewport& view = GetViewRef();
		
		if (m_isZoomToFit){
			m_storedTransform = view.GetTransform();

			UpdateComponentsPosition();

			view.UpdateFitTransform();
		}
		else{
			UpdateComponentsPosition();
		
			view.SetTransform(m_storedTransform);
		}
		
		UpdateButtonsState();
	}
}


void CConsoleBase::SetFitMode(FitMode mode)
{
	if (m_fitMode != mode){
		m_fitMode = mode;
		CViewport& view = GetViewRef();
		view.UpdateFitTransform();
		if (m_isZoomToFit){
			UpdateComponentsPosition();
		}
	}
}


void CConsoleBase::SetGridVisible(bool state)
{
	CViewport& view = GetViewRef();
	view.SetGridVisible(state);
}


void CConsoleBase::SetRulerVisible(bool state)
{
	if (state != m_isRulerVisible){
		m_isRulerVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


void CConsoleBase::SetGridInMm(bool state)
{
	CViewport& view = GetViewRef();

	if (state != view.IsGridInMm()){
		view.SetGridInMm(state);
		UpdateButtonsState();
	}
}


// UI visibility

void CConsoleBase::SetButtonsPanelVisible(bool state)
{
	if (state != m_isButtonsPanelVisible){
		m_isButtonsPanelVisible = state;
		UpdateComponentsPosition();
	}
}


void CConsoleBase::SetButtonsPanelVertical(bool state)
{
	if (state != m_isButtonsPanelVertical){
		m_isButtonsPanelVertical = state;
		UpdateComponentsPosition();
	}
}


void CConsoleBase::SetScrollbarsVisible(bool state)
{
	if (state != m_areScollbarsVisible){
		m_areScollbarsVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


void CConsoleBase::SetStatusVisible(bool state)
{
	if (state != m_isStatusVisible){
		m_isStatusVisible = state;
		UpdateComponentsPosition();
		UpdateButtonsState();
	}
}


// Buttons visibility

void CConsoleBase::SetZoomsVisible(bool state)
{
	if (state != m_areZoomsVisible){
		m_areZoomsVisible = state;
		UpdateComponentsPosition();
	}
}


void CConsoleBase::SetZoomToFitVisible(bool state)
{
	if (state != m_isZoomToFitVisible){
		m_isZoomToFitVisible = state;
		UpdateComponentsPosition();
	}
}


void CConsoleBase::SetPolylineButtonsVisible(bool state)
{
	if (state != m_arePolylineButtonsVisible){
		m_arePolylineButtonsVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetUserModeButtonsVisible(bool state)
{
	if (state != m_areUserModeButtonsVisible){
		m_areUserModeButtonsVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetStatusButtonVisible(bool state)
{
	if (state != m_isStatusButtonVisible){
		m_isStatusButtonVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetScrollbarsButtonVisible(bool state)
{
	if (state != m_isScrollbarsButtonVisible){
		m_isScrollbarsButtonVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetGridButtonVisible(bool state)
{
	if (state != m_isGridButtonVisible){
		m_isGridButtonVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetRulerButtonVisible(bool state)
{
	if (state != m_isRulerButtonVisible){
		m_isRulerButtonVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetMmButtonVisible(bool state)
{
	if (state != m_isMmButtonVisible){
		m_isMmButtonVisible = state;
		UpdateComponentsPosition();
		UpdateCommands();
	}
}


void CConsoleBase::SetPixelPositionVisible(bool state)
{
	if (state != m_isPixelPositionVisible){
		m_isPixelPositionVisible = state;
	}
}


void CConsoleBase::SetPixelValueVisible(bool state)
{
	if (state != m_isPixelValueVisible){
		m_isPixelValueVisible = state;
	}
}


void CConsoleBase::SetMmPositionVisible(bool state)
{
	if (state != m_isMmPositionVisible){
		m_isMmPositionVisible = state;
	}
}


void CConsoleBase::UpdateView()
{
	CViewport& view = GetViewRef();
	if (m_isZoomToFit){
		m_storedTransform = view.GetTransform();
		UpdateComponentsPosition();
		view.UpdateFitTransform();
	}

	view.Update();
}


// private methods

void CConsoleBase::Init()
{
	m_isButtonsPanelVisible = true;
	m_isButtonsPanelVertical = false;
	m_areScollbarsVisible = true;
	m_areZoomsVisible = true;
	m_isZoomToFitVisible = true;
	m_arePolylineButtonsVisible = true;
	m_areUserModeButtonsVisible = false;
	m_isStatusVisible = true;
	m_isStatusButtonVisible = true;
	m_isScrollbarsButtonVisible = true;
	m_isGridButtonVisible = true;
	m_isRulerButtonVisible = true;
	m_isRulerVisible = false;
	m_isMmButtonVisible = true;
	m_isUndoButtonVisible = true;

	m_isPixelPositionVisible = true;
	m_isPixelValueVisible = true;
	m_isMmPositionVisible = false;

	m_isBkActive = false;
	m_isZoomToFit = false;
	m_fitMode = FM_BOTH;
}


bool CConsoleBase::IsZoomToFit() const
{
	return m_isZoomToFit;
}


CConsoleBase::FitMode CConsoleBase::GetFitMode() const
{
	return m_fitMode;
}


bool CConsoleBase::IsGridVisible() const
{
	const CViewport& view = GetView();
	return view.IsGridVisible();
}


bool CConsoleBase::IsRulerVisible() const
{
	return m_isRulerVisible;
}


bool CConsoleBase::IsGridInMm() const
{
	const CViewport& view = GetView();
	return view.IsGridInMm();
}


// gui visibility

bool CConsoleBase::IsButtonsPanelVisible() const
{
	return m_isButtonsPanelVisible;
}


bool CConsoleBase::IsButtonsPanelVertical() const
{
	return m_isButtonsPanelVertical;
}


bool CConsoleBase::AreScrollbarsVisible() const
{
	return m_areScollbarsVisible;
}


bool CConsoleBase::IsStatusVisible() const
{
	return m_isStatusVisible;
}


// buttons visibility

bool CConsoleBase::AreZoomsVisible() const
{
	return m_areZoomsVisible;
}


bool CConsoleBase::IsZoomToFitVisible() const
{
	return m_isZoomToFitVisible;
}


bool CConsoleBase::ArePolylineButtonsVisible() const
{
	return m_arePolylineButtonsVisible;
}


bool CConsoleBase::AreUserModeButtonsVisible() const
{
	return m_areUserModeButtonsVisible;
}


bool CConsoleBase::IsStatusButtonVisible() const
{
	return m_isStatusButtonVisible;
}


bool CConsoleBase::IsScrollbarsButtonVisible() const
{
	return m_isScrollbarsButtonVisible;
}


bool CConsoleBase::IsGridButtonVisible() const
{
	return m_isGridButtonVisible;
}


bool CConsoleBase::IsRulerButtonVisible() const
{
	return m_isRulerButtonVisible;
}

bool CConsoleBase::IsMmButtonVisible() const
{
	return m_isMmButtonVisible;
}


bool CConsoleBase::IsPixelPositionVisible() const
{
	return m_isPixelPositionVisible;
}


bool CConsoleBase::IsPixelValueVisible() const
{
	return m_isPixelValueVisible;
}


bool CConsoleBase::IsMmPositionVisible() const
{
	return m_isMmPositionVisible;
}


bool CConsoleBase::IsBackgroundActive() const
{
	return m_isBkActive;
}


} // namespace iview


