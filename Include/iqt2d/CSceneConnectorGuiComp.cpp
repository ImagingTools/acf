#include "iqt2d/CSceneConnectorGuiComp.h"


namespace iqt2d
{


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CSceneConnectorGuiComp::GetCommands() const
{
	return &m_commands;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

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
		m_extenderCompPtr->AddItemsToScene(m_sceneProviderCompPtr.GetPtr(), IViewExtender::SF_DIRECT);

		iview::IShapeView* viewPtr = m_sceneProviderCompPtr->GetView();
		if (viewPtr != NULL){
			viewPtr->Update();
		}
	}
}


void CSceneConnectorGuiComp::OnGuiDestroyed()
{
	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->RemoveItemsFromScene(m_sceneProviderCompPtr.GetPtr());
		iview::IShapeView* viewPtr = m_sceneProviderCompPtr->GetView();
		if (viewPtr != NULL){
			viewPtr->Update();
		}
	}

	if (m_extenderGuiCompPtr.IsValid()){
		m_extenderGuiCompPtr->DestroyGui();
	}

	if (m_sceneGuiCompPtr.IsValid() && m_sceneGuiCompPtr->IsGuiCreated()){
		m_sceneGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::CComponentBase)

void CSceneConnectorGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_sceneCommandsCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandPtr = m_sceneCommandsCompPtr->GetCommands();
		if (commandPtr != NULL){
			m_commands.JoinLinkFrom(commandPtr);
		}
	}

	if (m_extenderCommandsCompPtr.IsValid()){
		const ibase::IHierarchicalCommand* commandPtr = m_extenderCommandsCompPtr->GetCommands();
		if (commandPtr != NULL){
			m_commands.JoinLinkFrom(commandPtr);
		}
	}
}


void CSceneConnectorGuiComp::OnComponentDestroyed()
{
	m_commands.ResetChilds();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt2d


