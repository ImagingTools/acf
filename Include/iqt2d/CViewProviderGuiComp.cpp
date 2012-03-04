#include "iqt2d/CViewProviderGuiComp.h"



namespace iqt2d
{


// public methods

CViewProviderGuiComp::CViewProviderGuiComp()
{
}


// reimplemented (iqt2d::IViewProvider)

int CViewProviderGuiComp::GetViewId() const
{
	I_ASSERT(m_viewIdAttrPtr.IsValid());

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


} // namespace iqt2d




