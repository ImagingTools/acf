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
		if (m_extenderGuiCompPtr->CreateGui(ParamsFrame)){
			QWidget* widgetPtr = m_extenderGuiCompPtr->GetWidget();
			I_ASSERT(widgetPtr != NULL);	// GUI was created correctly

			QSizePolicy policy = widgetPtr->sizePolicy();

			if ((policy.verticalPolicy() & QSizePolicy::ExpandFlag) == 0){
				QLayout* layoutPtr = ExternFrame->layout();
				if (layoutPtr != NULL){
					QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
					layoutPtr->addItem(spacerPtr);
				}
			}
		}
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


