#include <iqt2d/CViewProviderGuiComp.h>


// Qt include
#include <QtGui/QGuiApplication>


namespace iqt2d
{


// public methods

// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CViewProviderGuiComp::GetCommands() const
{
	if (IsGuiCreated()){
		iview::CConsoleGui* consolePtr = GetQtWidget();
	
		return consolePtr->GetCommands();
	}

	return NULL;
}


// reimplemented (ibase::ICommandsDisplayer)

void CViewProviderGuiComp::ShowCommands(const ibase::ICommandsProvider* commandsProvider)
{
	if (iview::CConsoleGui* consolePtr = GetQtWidget()){
		auto editModeButtonsPtr = dynamic_cast<const iview::IEditModeButtons*>(commandsProvider);

		consolePtr->SetExtraEditModeButtons(const_cast<iview::IEditModeButtons*>(editModeButtonsPtr));
	}
}


// reimplemented (iqt2d::IViewProvider)

int CViewProviderGuiComp::GetViewId() const
{
	Q_ASSERT(m_viewIdAttrPtr.IsValid());

	return *m_viewIdAttrPtr;
}


iview::IShapeView* CViewProviderGuiComp::GetView() const
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	if (consolePtr != NULL){
		return &consolePtr->GetViewRef();
	}

	return NULL;
}


const i2d::ICalibration2d* CViewProviderGuiComp::GetCalibration() const
{
	if (IsGuiCreated()) {
		iview::CConsoleGui* consolePtr = GetQtWidget();
		Q_ASSERT(consolePtr != NULL);

		iview::CViewport& view = consolePtr->GetViewRef();

		return view.GetCalibration();
	}

	return nullptr;
}


// protected methods

void CViewProviderGuiComp::SetupBackground()
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	Q_ASSERT(consolePtr != NULL);

	if (m_backgroundModeAttrPtr.IsValid()){
		switch (*m_backgroundModeAttrPtr){
		case BM_COLOR_SCHEMA:
			if (!m_colorSchemaCompPtr.IsValid()){
				iview::CColorSchema* newColorSchemaPtr = new iview::CColorSchema;
				newColorSchemaPtr->Assign(consolePtr->GetViewRef().GetColorSchema());

				QBrush backgroundBrush(QGuiApplication::palette().color(QPalette::Window));
				newColorSchemaPtr->SetBrush(iview::IColorSchema::SB_BACKGROUND, backgroundBrush);

				consolePtr->GetViewRef().SetDefaultColorSchema(newColorSchemaPtr, true);
			}
			else {
				consolePtr->GetViewRef().SetDefaultColorSchema(m_colorSchemaCompPtr.GetPtr(), false);
			}
			break;
		case BM_CHECKERBOARD: {
			QPixmap backgroundPixmap(32, 32);

			QPainter p(&backgroundPixmap);

			QColor color1 = QGuiApplication::palette().color(QPalette::Window);
			QColor color2 = QGuiApplication::palette().color(QPalette::Midlight);
			p.fillRect(0, 0, 16, 16, QBrush(color1));
			p.fillRect(0, 16, 16, 16, QBrush(color2));
			p.fillRect(16, 0, 16, 16, QBrush(color2));
			p.fillRect(16, 16, 16, 16, QBrush(color1));
			iview::CColorSchema* newColorSchemaPtr = new iview::CColorSchema;
			newColorSchemaPtr->Assign(consolePtr->GetViewRef().GetColorSchema());
			newColorSchemaPtr->SetBrush(iview::IColorSchema::SB_BACKGROUND, QBrush(backgroundPixmap));
			consolePtr->GetViewRef().SetDefaultColorSchema(newColorSchemaPtr, true);
			}
			break;
		}
	}
}


// reimplemented (CGuiComponentBase)

void CViewProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	Q_ASSERT(consolePtr != NULL);

	consolePtr->SetUpdateBlocked(true, iview::CConsoleBase::BF_COMMANDS);

	consolePtr->SetShapeStatusInfo(m_shapeStatusInfoCompPtr.GetPtr());

	// Setup view and its calibration:
	iview::CViewport& view = consolePtr->GetViewRef();

	view.SetShowInfoText(*m_infoTextEnabledAttrPtr);

	if (m_screenTransformationProvider.IsValid()) {
		view.SetExternalScreenTransform(m_screenTransformationProvider->GetTransformation());
	}

	// Delegate the style sheet of the parent window to the viewport:
	view.setStyleSheet(consolePtr->styleSheet());

	view.SetViewDraggable();

	if (m_fitModeAttrPtr.IsValid()){
		iview::CConsoleBase::FitMode fitMode = iview::CConsoleBase::FitMode(*m_fitModeAttrPtr);

		consolePtr->SetFitMode(fitMode);
	}

	if (m_zoomToFitEnabledAttrPtr.IsValid()){
		consolePtr->SetZoomToFit(*m_zoomToFitEnabledAttrPtr);
	}

	consolePtr->SetFullScreenAllowed(*m_fullScreenModeSupportedAttrPtr);

	// shape edit commands
	bool useShapeEditCommands = false;
	if (m_useShapeEditCommandsAttrPtr.IsValid()){
		useShapeEditCommands = *m_useShapeEditCommandsAttrPtr;
	}
	consolePtr->SetPolylineButtonsVisible(useShapeEditCommands);

	// grid commands
	bool useGridCommands = false;
	if (m_useGridCommandsAttrPtr.IsValid()){
		useGridCommands = *m_useGridCommandsAttrPtr;
	}
	consolePtr->SetGridButtonVisible(useGridCommands);
	consolePtr->SetMmButtonVisible(useGridCommands);

	// scrollbar commands
	bool useScrollBarCommands = false;
	if (m_useScollBarCommandsAttrPtr.IsValid()){
		useScrollBarCommands = *m_useScollBarCommandsAttrPtr;
	}
	consolePtr->SetScrollbarsButtonVisible(useScrollBarCommands);

	if (m_calibrationShapeCompPtr.IsValid()){
		view.ConnectCalibrationShape(m_calibrationShapeCompPtr.GetPtr());
	}

	if (m_distanceMeasureShapeCompPtr.IsValid()){
		view.ConnectToolShape(m_distanceMeasureShapeCompPtr.GetPtr());
		consolePtr->SetDistanceMeasureButtonVisible(true);
	}
	else{
		consolePtr->SetDistanceMeasureButtonVisible(false);
	}

	if (m_pointMeasureShapeCompPtr.IsValid()){
		view.ConnectToolShape(m_pointMeasureShapeCompPtr.GetPtr());
		consolePtr->SetPointMeasureButtonVisible(true);
	}
	else {
		consolePtr->SetPointMeasureButtonVisible(false);
	}

	SetupBackground();
	
	consolePtr->SetUpdateBlocked(false);
}


void CViewProviderGuiComp::OnGuiDestroyed()
{
	if (m_viewActivationHandlerCompPtr.IsValid()) {
		const int handlerCount = m_viewActivationHandlerCompPtr.GetCount();
		for (int i = 0; i < handlerCount; i++) {
			m_viewActivationHandlerCompPtr[i]->OnViewDeactivated(this);
		}
	}

	// Quit full screen mode on exit:
	iview::CConsoleGui* consolePtr = GetQtWidget();
	if (consolePtr != NULL){
		consolePtr->SetExtraEditModeButtons(nullptr);

		iview::CViewport& view = consolePtr->GetViewRef();
		const_cast<iview::IViewLayer&>(view.GetCalibrationLayer()).DisconnectAllShapes();
		const_cast<iview::IViewLayer&>(view.GetToolsLayer()).DisconnectAllShapes();

		if (consolePtr->IsFullScreenMode()){
			consolePtr->SetFullScreenMode(false);
		}
	}

	BaseClass::OnGuiDestroyed();
}


void CViewProviderGuiComp::OnGuiRetranslate()
{
	static const istd::IChangeable::ChangeSet commandsChangeSet(ibase::ICommandsProvider::CF_COMMANDS);
	istd::CChangeNotifier commandsNotifier(this, &commandsChangeSet);
	Q_UNUSED(commandsNotifier);

	iview::CConsoleGui* consolePtr = GetQtWidget();
	if (consolePtr != NULL){
		consolePtr->RetranslateGui();
	}
}


void CViewProviderGuiComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	if (m_viewActivationHandlerCompPtr.IsValid()){
		const int handlerCount = m_viewActivationHandlerCompPtr.GetCount();
		for (int i = 0; i < handlerCount; i++) {
			m_viewActivationHandlerCompPtr[i]->OnViewActivated(this);
		}
	}
}


void CViewProviderGuiComp::OnGuiHidden()
{
	BaseClass::OnGuiHidden();

	if (m_viewActivationHandlerCompPtr.IsValid()){
		const int handlerCount = m_viewActivationHandlerCompPtr.GetCount();
		for (int i = 0; i < handlerCount; i++) {
			m_viewActivationHandlerCompPtr[i]->OnViewDeactivated(this);
		}
	}
}


void CViewProviderGuiComp::OnGuiDesignChanged()
{
	BaseClass::OnGuiDesignChanged();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	Q_ASSERT(consolePtr != nullptr);

	consolePtr->UpdateDesign();

	SetupBackground();

	consolePtr->GetViewRef().InvalidateBackground();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CViewProviderGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (IsGuiCreated() && m_calibrationProviderCompPtr.IsValid()){
		const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();

		iview::CConsoleGui* consolePtr = GetQtWidget();
		Q_ASSERT(consolePtr != NULL);

		iview::CViewport& view = consolePtr->GetViewRef();

		view.SetDisplayCalibration(calibrationPtr);
	}
}


// reimplemented (icomp::CComponentBase)

void CViewProviderGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationProviderModelCompPtr.IsValid()){
		BaseClass2::RegisterModel(m_calibrationProviderModelCompPtr.GetPtr());
	}
}


void CViewProviderGuiComp::OnComponentDestroyed()
{
	BaseClass2::UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt2d


