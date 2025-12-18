#ifndef iqtgui_CDockWidgetGuiComp_included
#define iqtgui_CDockWidgetGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDockWidget>
#else
#include <QtGui/QDockWidget>
#endif

// ACF includes
#include <iqtgui/IMainWindowComponent.h>
#include <iqtgui/TGuiComponentBase.h>


namespace iqtgui
{


/**
	Wrapper for docking windows for main window.
*/
class CDockWidgetGuiComp:
			public TGuiComponentBase<QDockWidget>,
			virtual public IMainWindowComponent
{
	Q_OBJECT
public:
	typedef TGuiComponentBase<QDockWidget> BaseClass;

	I_BEGIN_COMPONENT(CDockWidgetGuiComp);
		I_REGISTER_INTERFACE(IMainWindowComponent);
		I_ASSIGN(m_slaveGuiCompPtr, "SlaveGui", "Slave GUI for this dock widget", true, "SlaveGui");
		I_ASSIGN(m_dockAreaAttrPtr, "DockArea", "Specify the dock area for this widget\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_dockFeaturesAttrPtr, "DockFeatures", "Specify the dock features for this widget\nIt is combination of options:\n0 - permanent\n1 - closable\n 2 - moveable\n 4 - floatable\n 8 - vertical title", true, WCF_CLOSABLE | WCF_MOVEABLE | WCF_FLOATABLE);
		I_ASSIGN(m_allowedDockAreasAttrPtr, "AllowedDockAreas", "Specify the allowed dock areas\nIt is combination of options:\n 1 - left\n 2 - right\n 4 - top\n 8 - bottom", false, 0);
		I_ASSIGN(m_dockTitleAttrPtr, "DockTitle", "Specify the dock title ", false, "DockTitle");
	I_END_COMPONENT;

	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow) override;
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow) override;
	virtual QString GetTitle() const override;
	virtual int GetFlags() const override;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;
	virtual void OnGuiRetranslate() override;

private:
	I_REF(iqtgui::IGuiObject, m_slaveGuiCompPtr);
	I_ATTR(int, m_dockAreaAttrPtr);
	I_ATTR(int, m_dockFeaturesAttrPtr);
	I_ATTR(int, m_allowedDockAreasAttrPtr);
	I_TEXTATTR(m_dockTitleAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CDockWidgetGuiComp_included


