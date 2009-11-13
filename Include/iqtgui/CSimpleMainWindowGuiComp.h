#ifndef iqtgui_CMainWindowComp_included
#define iqtgui_CMainWindowComp_included


// Qt includes
#include <QMainWindow>

#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"


namespace iqtgui
{


class CSimpleMainWindowGuiComp: public iqtgui::TRestorableGuiWrap<iqtgui::TGuiComponentBase<QMainWindow> >
{
public:
	typedef iqtgui::TRestorableGuiWrap<iqtgui::TGuiComponentBase<QMainWindow> > BaseClass;

	I_BEGIN_COMPONENT(CSimpleMainWindowGuiComp);
		I_ASSIGN(m_workspaceCompPtr, "Workspace", "Document workspace", true, "Workspace");
		I_ASSIGN(m_isNestingEnabledAttrPtr, "IsNestingEnabled", "If true, docking nesting is enabled,\nmore than one docking widget is allowed on the same size", true, false);
		I_ASSIGN_MULTI_0(m_mainWindowComponentsPtr, "MainWindowComponents", "Additional GUI components", false);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of icons using in the main window", false, 16);
	I_END_COMPONENT;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

protected:
	void SetupMainWindowComponents(QMainWindow& mainWindow);

	virtual void AddMainComponent(iqtgui::IMainWindowComponent* componentPtr);
	virtual void RemoveMainComponent(iqtgui::IMainWindowComponent* componentPtr);

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_workspaceCompPtr);
	I_ATTR(bool, m_isNestingEnabledAttrPtr);
	I_MULTIREF(iqtgui::IMainWindowComponent, m_mainWindowComponentsPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CMainWindowComp_included


