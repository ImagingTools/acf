#include <iqt2d/CGraphPlotParamsGuiComp.h>


namespace iqt2d
{


CGraphPlotParamsGuiComp::CGraphPlotParamsGuiComp()
{
}


// reimplemented (iqt2d::TShapeParamsGuiCompBase)

iview::CInteractiveShapeBase* CGraphPlotParamsGuiComp::CreateShapeInstance() const
{
	iview::CGraphPlotShape* shapePtr = new iview::CGraphPlotShape();
	return shapePtr;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CGraphPlotParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();
}


} // namespace iqt2d
