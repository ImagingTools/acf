#include "iqt2d/CViewProviderGuiComp.h"


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


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CViewProviderGuiComp::GetCalibration() const
{
	iview::CConsoleGui* consolePtr = GetQtWidget();
	if (consolePtr != NULL){
		const iview::CViewport& view = consolePtr->GetView();

		return view.GetCalibration();
	}

	return NULL;
}


// protected methods

void CViewProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	iview::CConsoleGui* consolePtr = GetQtWidget();
	Q_ASSERT(consolePtr != NULL);

	consolePtr->SetShapeStatusInfo(m_shapeStatusInfoCompPtr.GetPtr());

	// Setup view and its calibration:
	iview::CViewport& view = consolePtr->GetViewRef();
	view.SetViewDraggable();

	if (m_fitModeAttrPtr.IsValid()){
		iview::CConsoleBase::FitMode fitMode = iview::CConsoleBase::FitMode(*m_fitModeAttrPtr);

		consolePtr->SetFitMode(fitMode);
	}

	if (m_zoomToFitEnabledAttrPtr.IsValid()){
		consolePtr->SetZoomToFit(*m_zoomToFitEnabledAttrPtr);
	}

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

	view.ConnectCalibrationShape(&m_calibrationShape);

	if (m_backgroundModeAttrPtr.IsValid()){
		switch (*m_backgroundModeAttrPtr){
			case BM_COLOR_SCHEMA:
				if (!m_colorSchemaCompPtr.IsValid()){
					iview::CColorSchema* newColorSchemaPtr = new iview::CColorSchema;
					newColorSchemaPtr->Assign(consolePtr->GetViewRef().GetColorSchema());

					QBrush backgroundBrush(qRgb(192, 192, 192));
					newColorSchemaPtr->SetBrush(iview::IColorSchema::SB_BACKGROUND, backgroundBrush);

					consolePtr->GetViewRef().SetDefaultColorSchema(newColorSchemaPtr, true);
				}
				else{
					consolePtr->GetViewRef().SetDefaultColorSchema(m_colorSchemaCompPtr.GetPtr(), false);
				}
				break;
		}
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CViewProviderGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
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


