#ifndef iqt_CDockWidgetGuiComp_included
#define iqt_CDockWidgetGuiComp_included


#include <QDockWidget>

#include "iqt/IMainWindowComponent.h"
#include "iqt/TGuiComponentBase.h"



namespace iqt
{


class CDockWidgetGuiComp: public iqt::TGuiComponentBase<QDockWidget>, public iqt::IMainWindowComponent
{
	Q_OBJECT
public:
	typedef iqt::TGuiComponentBase<QDockWidget> BaseClass;

	I_BEGIN_COMPONENT(CDockWidgetGuiComp)
		I_REGISTER_INTERFACE(iqt::IMainWindowComponent)
		I_ASSIGN(m_slaveGuiCompPtr, "SlaveGui", "Slave GUI for this dock widget", true, "SlaveGui")
		I_ASSIGN(m_dockAreaAttrPtr, "DockArea", "Specify the dock area for this widget", false, 0)
		I_ASSIGN(m_dockFeaturesAttrPtr, "DockFeatures", "Specify the dock features for this widget", false, 0)
		I_ASSIGN(m_dockTitleAttrPtr, "DockTitle", "Specify the dock title ", false, "DockTitle")
	I_END_COMPONENT
	
	// reimplemented (iqt::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

protected:
	I_REF(iqt::IGuiObject, m_slaveGuiCompPtr);
	I_ATTR(int, m_dockAreaAttrPtr);
	I_ATTR(int, m_dockFeaturesAttrPtr);
	I_ATTR(istd::CString, m_dockTitleAttrPtr);
};


} // namespace iqt


#endif // !iqt_CDockWidgetGuiComp_included

