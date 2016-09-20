#include <iqt2d/CCircleParamsGuiComp.h>


namespace iqt2d
{


// protected methods


// protected slots

void CCircleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CCircleParamsGuiComp::OnActionTriggered(QAction* actionPtr)
{
	BaseClass::OnActionTriggered(actionPtr);
}


} // namespace iqt2d


