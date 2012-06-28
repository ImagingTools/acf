#ifndef iqt2d_CViewProviderGuiComp_included
#define iqt2d_CViewProviderGuiComp_included


// ACF includes
#include "ibase/ICommandsProvider.h"

#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TGuiComponentBase.h"

#include "iview/CConsoleGui.h"
#include "iview/CAffiniteCalibrationShape.h"

#include "iqt2d/IViewProvider.h"


namespace iqt2d
{


class CViewProviderGuiComp: 
			public	iqtgui::TGuiComponentBase<iview::CConsoleGui>,
			virtual public ibase::ICommandsProvider,
			virtual public IViewProvider,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef iqtgui::TGuiComponentBase<iview::CConsoleGui> BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CViewProviderGuiComp);	
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(IViewProvider);
		I_ASSIGN(m_shapeStatusInfoCompPtr, "ShapeStatusInfo", "Shape status info consumer", false, "ShapeStatusInfo");
		I_ASSIGN(m_viewIdAttrPtr, "ViewId", "ID allowing identifying this view", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
		I_ASSIGN(m_zoomToFitEnabledAttrPtr, "ZoomToFitEnabled", "If true, the shapes will be fit to the view according to the defined fitting mode", false, false);
		I_ASSIGN(m_useShapeEditCommandsAttrPtr, "UseShapeEditCommands", "If true, the commands for shape editing are available", false, false);
		I_ASSIGN(m_useGridCommandsAttrPtr, "UseGridCommands", "If true, the commands for grid mangement are available", false, false);
		I_ASSIGN(m_useScollBarCommandsAttrPtr, "UseScrollBarCommands", "If true, the commands for scroll bar management are available", false, false);
		I_ASSIGN(m_fitModeAttrPtr, "FitMode", "Select fitting mode for the view. 0 - No fitting\n1 - Fit contents to view\n2 - Horizontal fit\n3 - Vertical fit\n4 - Both axes sclaled separately\n5 - Scale both axes proportional to display smallest AOI, which fully covers display", false, 0);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provider of 2D-calibration for the entire view", false, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::IViewProvider)
	virtual int GetViewId() const;
	virtual iview::IShapeView* GetView() const;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CMultiModelDispatcherBase)
	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iview::IShapeStatusInfo, m_shapeStatusInfoCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_ATTR(int, m_viewIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(bool, m_useShapeEditCommandsAttrPtr);
	I_ATTR(bool, m_useGridCommandsAttrPtr);
	I_ATTR(bool, m_useScollBarCommandsAttrPtr);
	I_ATTR(bool, m_useStatusBarCommandsAttrPtr);
	I_ATTR(bool, m_zoomToFitEnabledAttrPtr);
	I_ATTR(int, m_fitModeAttrPtr);

	iview::CAffiniteCalibrationShape m_calibrationShape;
};


} // namespace iqt2d


#endif // !iqt2d_CViewProviderGuiComp_included


