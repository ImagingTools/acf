#include <iqt2d/CSceneConnectorGuiComp.h>


// ACF includes
#include <iview/IShapeView.h>


namespace iqt2d
{


const istd::IChangeable::ChangeSet s_commandsChangeSet(ibase::ICommandsProvider::CF_COMMANDS);


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
			Q_ASSERT(widgetPtr != NULL);	// GUI was created correctly

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
	else{
		ExternFrame->setVisible(false);
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



// reimplemented (TRestorableGuiWrap)

void CSceneConnectorGuiComp::OnRestoreSettings(const QSettings& settings)
{
	Q_ASSERT(IsGuiCreated());

	// preserve overriding of splitter orientation:
	Qt::Orientation splitterOrientation = splitterPtr->orientation();

	QByteArray splitterState = settings.value(GetSettingsKey()).toByteArray();

	splitterPtr->restoreState(splitterState);

	splitterPtr->setOrientation(splitterOrientation);
}


void CSceneConnectorGuiComp::OnSaveSettings(QSettings& settings) const
{
	Q_ASSERT(IsGuiCreated());

	QByteArray splitterState = splitterPtr->saveState();

	settings.setValue(GetSettingsKey(), splitterState);
}


QString CSceneConnectorGuiComp::GetSettingsKey() const
{
	QString settingsKey = "Splitter";
	if (m_settingsKeyAttrPtr.IsValid()){
		settingsKey = *m_settingsKeyAttrPtr + QString("/") + settingsKey;
	}

	return settingsKey;
}


// reimplemented (icomp::CComponentBase)

void CSceneConnectorGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeNotifier commandsNotifier(this, &s_commandsChangeSet);
	Q_UNUSED(commandsNotifier);

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
	istd::CChangeNotifier commandsNotifier(this, &s_commandsChangeSet);
	Q_UNUSED(commandsNotifier);

	m_commands.ResetChilds();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt2d


