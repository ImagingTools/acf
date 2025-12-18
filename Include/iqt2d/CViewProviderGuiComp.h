#pragma once


// ACF includes
#include <ibase/ICommandsProvider.h>
#include <ibase/ICommandsDisplayer.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqt2d/IViewProvider.h>
#include <iqt2d/IViewActivationHandler.h>
#include <iview/IColorSchema.h>
#include <iview/CConsoleGui.h>
#include <iview/IScreenTransformationProvider.h>
#include <iview/IEditModeButtons.h>


namespace iqt2d
{



class CViewProviderGuiCompAttr: public iqtgui::TGuiComponentBase<iview::CConsoleGui>
{
public:
	typedef iqtgui::TGuiComponentBase<iview::CConsoleGui> BaseClass;

	/**
		Background type for the 2D-view.
	*/
	enum BackgroundMode
	{
		/**
			Standard window background.
		*/
		BM_NORMAL,

		/**
			Color from the given color scheme.
		*/
		BM_COLOR_SCHEMA,

		/**
			Checkerboard pattern.
		*/
		BM_CHECKERBOARD,

		/**
			Dot grid.
		*/
		BM_DOT_GRID
	};

	I_BEGIN_COMPONENT(CViewProviderGuiCompAttr);
		I_ASSIGN(m_viewIdAttrPtr, "ViewId", "ID allowing identifying this view", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
		I_ASSIGN(m_infoTextEnabledAttrPtr, "InfoTextEnabled", "Draws InfoText as overlay", true, false);
		I_ASSIGN(m_drawBorderAttrPtr, "DrawBorder", "Draws border", true, true);
		I_ASSIGN(m_zoomToFitEnabledAttrPtr, "ZoomToFitEnabled", "If true, the shapes will be fit to the view according to the defined fitting mode", false, false);
		I_ASSIGN(m_useShapeEditCommandsAttrPtr, "UseShapeEditCommands", "If true, the commands for shape editing are available", false, false);
		I_ASSIGN(m_useGridCommandsAttrPtr, "UseGridCommands", "If true, the commands for grid management are available", false, false);
		I_ASSIGN(m_useScollBarCommandsAttrPtr, "UseScrollBarCommands", "If true, the commands for scroll bar management are available", false, false);
		I_ASSIGN(m_fitModeAttrPtr, "FitMode", "Select fitting mode for the view. 0 - No fitting\n1 - Fit contents to view\n2 - Horizontal fit\n3 - Vertical fit\n4 - Both axes scaled separately\n5 - Scale both axes proportional to display smallest AOI, which fully covers display", false, 0);
		I_ASSIGN(m_backgroundModeAttrPtr, "BackgroundMode", "Mode of background drawing:\n 0 - standard window background\n 1 - color used from color scheme \n 2 - checkerboard\n 3 - dot grid\n", true, BM_NORMAL);
		I_ASSIGN(m_fullScreenModeSupportedAttrPtr, "FullScreenModeSupported", "If enabled, the view will be switched into full screen mode on mouse double click", true, true);
	I_END_COMPONENT;

protected:
	I_ATTR(int, m_viewIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(bool, m_infoTextEnabledAttrPtr);
	I_ATTR(bool, m_useShapeEditCommandsAttrPtr);
	I_ATTR(bool, m_useGridCommandsAttrPtr);
	I_ATTR(bool, m_useScollBarCommandsAttrPtr);
	I_ATTR(bool, m_useStatusBarCommandsAttrPtr);
	I_ATTR(bool, m_zoomToFitEnabledAttrPtr);
	I_ATTR(int, m_fitModeAttrPtr);
	I_ATTR(int, m_backgroundModeAttrPtr);
	I_ATTR(bool, m_fullScreenModeSupportedAttrPtr);
	I_ATTR(bool, m_drawBorderAttrPtr);

};


/**
	Standard 2D-View provider.
	It allows setting of display calibration and supports \c i2d::ICalibrationProvider to get it.
*/
class CViewProviderGuiComp: 
			public CViewProviderGuiCompAttr,
			virtual public ibase::ICommandsProvider,
			virtual public IViewProvider,
			virtual public i2d::ICalibrationProvider,
			virtual public ibase::ICommandsDisplayer,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef CViewProviderGuiCompAttr BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CViewProviderGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(ibase::ICommandsDisplayer);
		I_REGISTER_INTERFACE(IViewProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_shapeStatusInfoCompPtr, "ShapeStatusInfo", "Shape status info consumer", false, "ShapeStatusInfo");
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provider of 2D-calibration for the entire view", false, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN(m_colorSchemaCompPtr, "ColorSchema", "Color schema used for console", false, "ColorSchema");
		I_ASSIGN(m_calibrationShapeCompPtr, "CalibrationShape", "Calibration shape displaying calibration grid, if not choosen default affine calibration shape will be used", false, "CalibrationShape");
		I_ASSIGN(m_distanceMeasureShapeCompPtr, "DistanceMeasureShape", "Distance measurement shape", false, "DistanceMeasureShape");
		I_ASSIGN(m_pointMeasureShapeCompPtr, "PointMeasureShape", "Point measurement shape", false, "PointMeasureShape");
		I_ASSIGN_MULTI_0(m_viewActivationHandlerCompPtr, "ViewActivationHandler", "Handler for observation the visual status of the view provider", false);
		I_ASSIGN(m_screenTransformationProvider, "ScreenTransformationProvider", "Gets object which contains current screen transformation", false, "ScreenTransformationProvider");
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

	// reimplemented (ibase::ICommandsDisplayer)
	virtual void ShowCommands(const ibase::ICommandsProvider* commandsProvider) override;

	// reimplemented (iqt2d::IViewProvider)
	virtual int GetViewId() const override;
	virtual iview::IShapeView* GetView() const override;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const override;

protected:
	virtual void SetupBackground();

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;
	virtual void OnGuiRetranslate() override;
	virtual void OnGuiShown() override;
	virtual void OnGuiHidden() override;
	virtual void OnGuiDesignChanged() override;

	// reimplemented (imod::CMultiModelDispatcherBase)
	void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(iview::IShapeStatusInfo, m_shapeStatusInfoCompPtr);
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_REF(iview::IColorSchema, m_colorSchemaCompPtr);
	I_REF(iview::IShape, m_calibrationShapeCompPtr);
	I_REF(iview::IShape, m_distanceMeasureShapeCompPtr);
	I_REF(iview::IShape, m_pointMeasureShapeCompPtr);
	I_MULTIREF(IViewActivationHandler, m_viewActivationHandlerCompPtr);
	I_REF(iview::IScreenTransformationProvider, m_screenTransformationProvider);
};


} // namespace iqt2d


