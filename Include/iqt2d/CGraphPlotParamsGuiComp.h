#pragma once


// ACF includes
#include <i2d/CGraphData2d.h>
#include <iview/CGraphPlotShape.h>
#include <iqt2d/TShapeParamsGuiCompBase.h>


namespace iqt2d
{


/**
	GUI component for editing graph plot parameters.
	This component provides a simple interface for managing graph data,
	including curves, axis labels, and display options.
*/
class CGraphPlotParamsGuiComp: 
			public TShapeParamsGuiCompBase<
						iqtgui::CGuiComponentBase,
						iview::CGraphPlotShape,
						i2d::CGraphData2d>
{
	Q_OBJECT

public:
	typedef TShapeParamsGuiCompBase<
				iqtgui::CGuiComponentBase,
				iview::CGraphPlotShape,
				i2d::CGraphData2d> BaseClass;

	I_BEGIN_COMPONENT(CGraphPlotParamsGuiComp);
	I_END_COMPONENT;

	CGraphPlotParamsGuiComp();

protected:
	// reimplemented (iqt2d::TShapeParamsGuiCompBase)
	virtual iview::CInteractiveShapeBase* CreateShapeInstance() const override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate() override;
};


} // namespace iqt2d
