#ifndef iqt2d_CViewProviderGuiComp_included
#define iqt2d_CViewProviderGuiComp_included


// ACF includes
#include "iqtgui/TGuiComponentBase.h"

#include "iview/CConsoleGui.h"

#include "iqt2d/IViewProvider.h"


namespace iqt2d
{


class CViewProviderGuiComp: 
			public iqtgui::TGuiComponentBase<iview::CConsoleGui>,
			virtual public IViewProvider
{
public:
	typedef iqtgui::TGuiComponentBase<iview::CConsoleGui> BaseClass;

	I_BEGIN_COMPONENT(CViewProviderGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(IViewProvider);
		I_ASSIGN(m_viewIdAttrPtr, "ViewId", "ID allowing identifying this view", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
	I_END_COMPONENT;

	CViewProviderGuiComp();

	// reimplemented (iqt2d::IViewProvider)
	virtual int GetViewId() const;
	virtual iview::IShapeView* GetView() const;

private:
	I_ATTR(int, m_viewIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CViewProviderGuiComp_included


