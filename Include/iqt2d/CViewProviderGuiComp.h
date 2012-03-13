#ifndef iqt2d_CViewProviderGuiComp_included
#define iqt2d_CViewProviderGuiComp_included


// ACF includes
#include "ibase/TCommandsProviderCompWrap.h"

#include "iqtgui/TGuiComponentBase.h"

#include "iview/CConsoleGui.h"

#include "iqt2d/IViewProvider.h"


namespace iqt2d
{


class CViewProviderGuiComp: 
			public	ibase::TCommandsProviderCompWrap<
						iqtgui::TGuiComponentBase<iview::CConsoleGui> >,
			virtual public IViewProvider
{
public:
	typedef ibase::TCommandsProviderCompWrap<
				iqtgui::TGuiComponentBase<iview::CConsoleGui> > BaseClass;

	I_BEGIN_COMPONENT(CViewProviderGuiComp);	
		I_REGISTER_INTERFACE(IViewProvider);
		I_ASSIGN(m_viewIdAttrPtr, "ViewId", "ID allowing identifying this view", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
		I_ASSIGN(m_fitOnStartAttrPtr, "FitToViewOnStart", "If true, the shapes will be fit to the view on start", false, true);
		I_ASSIGN(m_useShapeEditCommandsAttrPtr, "UseShapeEditCommands", "If true, the commands for shape editing are available", false, false);
		I_ASSIGN(m_useGridCommandsAttrPtr, "UseGridCommands", "If true, the commands for grid mangement are available", false, false);
		I_ASSIGN(m_useScollBarCommandsAttrPtr, "UseScrollBarCommands", "If true, the commands for scroll bar management are available", false, false);
		I_ASSIGN(m_useStatusBarCommandsAttrPtr, "UseStatusBarCommands", "If true, the commands for status bar management are available", false, false);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::IViewProvider)
	virtual int GetViewId() const;
	virtual iview::IShapeView* GetView() const;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_ATTR(int, m_viewIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(bool, m_useShapeEditCommandsAttrPtr);
	I_ATTR(bool, m_useGridCommandsAttrPtr);
	I_ATTR(bool, m_useScollBarCommandsAttrPtr);
	I_ATTR(bool, m_useStatusBarCommandsAttrPtr);
	I_ATTR(bool, m_fitOnStartAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CViewProviderGuiComp_included


