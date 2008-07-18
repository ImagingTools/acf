#include "iqt2d/CSceneConnectorGuiComp.h"


namespace iqt2d
{


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CSceneConnectorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_sceneGuiCompPtr.IsValid()){
		m_sceneGuiCompPtr->CreateGui(ImageViewFrame);
	}

	if (m_extenderGuiCompPtr.IsValid()){
		m_extenderGuiCompPtr->CreateGui(ParamsFrame);
	}

	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->AddItemsToScene(m_sceneProviderCompPtr.GetPtr(), ISceneExtender::SF_DIRECT);
	}
}


void CSceneConnectorGuiComp::OnGuiDestroyed()
{
	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->RemoveItemsFromScene(m_sceneProviderCompPtr.GetPtr());
	}

	if (m_extenderGuiCompPtr.IsValid()){
		m_extenderGuiCompPtr->DestroyGui();
	}

	if (m_sceneGuiCompPtr.IsValid() && m_sceneGuiCompPtr->IsGuiCreated()){
		m_sceneGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqt2d


