#include "iview/CConsoleBase.h"


// Qt includes
#include <QtCore/QString>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

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
			iview::CScreenTransform viewTransform = view.GetTransform();
			if (m_storedTransform != viewTransform){
				m_storedTransform = viewTransform;

				UpdateComponentsPosition();

				view.UpdateFitTransform();
			}

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


void CConsoleBase::UpdateView()
{
	CViewport& view = GetViewRef();
	if (m_isZoomToFit){
		iview::CScreenTransform viewTransform = view.GetTransform();
		if (m_storedTransform != viewTransform){
			m_storedTransform = viewTransform;

			UpdateComponentsPosition();

			view.UpdateFitTransform();
		}
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
	m_isScrollbarsButtonVisible = true;
	m_isGridButtonVisible = true;
	m_isRulerButtonVisible = true;
	m_isRulerVisible = false;
	m_isMmButtonVisible = true;
	m_isBkActive = false;
	m_isZoomToFit = false;
	m_fitMode = FM_BOTH;
	m_isFullScreenAllowed = true;
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


bool CConsoleBase::IsBackgroundActive() const
{
	return m_isBkActive;
}


bool CConsoleBase::IsFullScreenAllowed() const
{
	return m_isFullScreenAllowed;
}


void CConsoleBase::SetFullScreenAllowed(bool allow /*= true*/)
{
	m_isFullScreenAllowed = allow;
}


} // namespace iview


