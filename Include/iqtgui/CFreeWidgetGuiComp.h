#ifndef iqtgui_CFreeWidgetGuiComp_included
#define iqtgui_CFreeWidgetGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif


// ACF includes
#include <icomp/CComponentBase.h>

#include <iqtgui/IGuiObject.h>


namespace iqtgui
{


class CFreeWidgetGuiComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFreeWidgetGuiComp);
		I_ASSIGN(m_slaveGuiCompPtr, "SlaveGui", "Slave GUI for this window", true, "SlaveGui");
		I_ASSIGN(m_titleAttrPtr, "Title", "Specify the window title ", true, "Free Window");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	I_REF(iqtgui::IGuiObject, m_slaveGuiCompPtr);
	I_TEXTATTR(m_titleAttrPtr);

	istd::TDelPtr<QMainWindow> m_mainWindowPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CFreeWidgetGuiComp_included

