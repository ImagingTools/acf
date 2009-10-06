#include "iqtmm/CVideoPlayerGuiComp.h"


namespace iqtmm
{


// public methods

// reimplemented (icomp::IComponent)

void CVideoPlayerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_urlParamModelCompPtr.IsValid()){
		m_urlParamModelCompPtr->AttachObserver(this);
	}
}


void CVideoPlayerGuiComp::OnComponentDestroyed()
{
	if (m_urlParamModelCompPtr.IsValid()){
		m_urlParamModelCompPtr->DetachObserver(this);
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CVideoPlayerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_urlParamsGuiCompPtr.IsValid() && m_urlParamModelCompPtr.IsValid()){
		m_urlParamsGuiCompPtr->CreateGui(UrlParamFrame);

		UrlParamFrame->setVisible(true);
	}
	else{
		UrlParamFrame->setVisible(false);
	}

	if (m_controllerBarGuiCompPtr.IsValid()){
		m_controllerBarGuiCompPtr->CreateGui(ControllerBarFrame);

		ControllerBarFrame->setVisible(true);
	}
	else{
		ControllerBarFrame->setVisible(false);
	}

	if (m_videoGuiCompPtr.IsValid()){
		m_videoGuiCompPtr->CreateGui(FrameView);
	}
}


void CVideoPlayerGuiComp::OnGuiDestroyed()
{
	if (m_mediaControllerCompPtr.IsValid()){
		m_mediaControllerCompPtr->CloseMedium();
	}

	if (m_controllerBarGuiCompPtr.IsValid() && m_controllerBarGuiCompPtr->IsGuiCreated()){
		m_controllerBarGuiCompPtr->DestroyGui();
	}

	if (m_videoGuiCompPtr.IsValid() && m_videoGuiCompPtr->IsGuiCreated()){
		m_videoGuiCompPtr->DestroyGui();
	}

	if (m_urlParamsGuiCompPtr.IsValid() && m_urlParamsGuiCompPtr->IsGuiCreated()){
		m_urlParamsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CVideoPlayerGuiComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (m_mediaControllerCompPtr.IsValid() && m_urlParamCompPtr.IsValid()){
		m_mediaControllerCompPtr->OpenMediumUrl(m_urlParamCompPtr->GetPath(), false);
	}
}


} // namespace iqtmm


