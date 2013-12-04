#ifndef iqtgui_CStatusBarWidgetGuiComp_included
#define iqtgui_CStatusBarWidgetGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QStatusBar>
#else
#include <QtGui/QStatusBar>
#endif

// ACF includes
#include "icomp/CComponentBase.h"

#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Component for inserting of a widget into a status bar of the application.
*/
class CStatusBarWidgetComp:
			public icomp::CComponentBase,
			virtual public iqtgui::IMainWindowComponent
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStatusBarWidgetComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN_MULTI_0(m_statusBarWidgetsCompPtr, "StatusBarWidgets", "Widgets to place into the status bar of the application", true);
		I_ASSIGN(m_associatedDocumentTypeId, "AssociatedDocumentTypeId", "ID of the associated document type ID", false, "Default");
		I_ASSIGN(m_isSizeGripEnabledAttrPtr, "EnableSizeGrip", "if enabled, the size grip will be created", true, true);
		I_ASSIGN(m_suppressWidgetFrameAttrPtr, "SuppressWidgetFrame", "if enabled, the frame around status bar widgets will be hidden", true, false);
	I_END_COMPONENT;

	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow);
	virtual QByteArray GetAssociatedDocumentTypeId() const;
	virtual QString GetTitle() const;

private:
	I_MULTIREF(iqtgui::IGuiObject, m_statusBarWidgetsCompPtr);
	I_ATTR(QByteArray, m_associatedDocumentTypeId);
	I_ATTR(bool, m_isSizeGripEnabledAttrPtr);
	I_ATTR(bool, m_suppressWidgetFrameAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CStatusBarWidgetGuiComp_included


