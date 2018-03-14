#ifndef iqtgui_CGuiApplicationComp_included
#define iqtgui_CGuiApplicationComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QSystemTrayIcon>
#else
#include <QtGui/QSystemTrayIcon>
#endif

// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <iprm/IEnableableParam.h>
#include <ilog/IMessageConsumer.h>
#include <ibase/ICommandsProvider.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/IGuiApplication.h>
#include <iqtgui/CApplicationCompBase.h>


namespace iqtgui
{


/**
	Standard component for a Qt based GUI application.
	You have several attributes to control the appearance and decoration of the application window,
	you can define the splash screen and set some information about your application.

	Some notes for using style sheets:
	To make using of different style sheets according to the operation system possible, this component sets the property 'OperatingSystem' to the application's main widget.
	Then you can you following syntax in your style sheet to to manage your content depend on currently running operation system:
	\code
	QWidget[OperatingSystem=OSX] QLabel{
	font-size: 13pt;
	}

	QWidget[OperatingSystem=Windows] QLabel{
	font-size: 8pt;
	}
	\endcode
	Supported operation systems are: iOS, Unix, OSX and Windows. \sa istd::CSystem::GetOperationSystemName
*/
class CGuiApplicationComp:
			public QObject,
			public CApplicationCompBase,
			protected imod::TSingleModelObserverBase<iprm::IEnableableParam>,
			virtual public iqtgui::IGuiApplication
{
	Q_OBJECT
public:
	typedef CApplicationCompBase BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::IEnableableParam> BaseClass2;

	I_BEGIN_COMPONENT(CGuiApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_REGISTER_SUBELEMENT(TrayMessages);
		I_REGISTER_SUBELEMENT_INTERFACE(TrayMessages, ilog::IMessageConsumer, ExtractTrayMessages);
		I_ASSIGN(m_mainGuiCompPtr, "MainGui", "Gui object shown as main window", false, "MainGui");
		I_ASSIGN(m_allowApplicationCloseCompPtr, "AllowClose", "Controls closing of the application", false, "AllowClose");
		I_ASSIGN_TO(m_allowApplicationCloseModelCompPtr, m_allowApplicationCloseCompPtr, false);
		I_ASSIGN(m_trayIconCommandsCompPtr, "TrayIconsCommands", "Provider of tray icons commands. The commands are shown in the menu of the system tray", false, "TrayIconsCommands");
		I_ASSIGN(m_frameSpaceSizeAttrPtr, "FrameSpaceSize", "Number of pixels will be added on the all window sides", false, 9);
		I_ASSIGN(m_uiStartModeAttrPtr, "UiStartMode", "UI mode by application start up.\n0 - normal\n1 - full screen\n2 - minimized\n3 - maximized", false, 0);
		I_ASSIGN(m_useMenuIconsOnMacAttrPtr, "UseMenuIconsOnMac", "If enabled, the menu icons will be used in the application's menu bar on Mac OS", true, false);
		I_ASSIGN(m_useTrayIconAttrPtr, "UseTrayIcon", "If enabled, the tray icon for the application will be used", true, false);
	I_END_COMPONENT;

	CGuiApplicationComp();

	// reimplemented (ibase::IGuiApplication)
	virtual const iqtgui::IGuiObject* GetApplicationGui() const;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;

protected:
	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

protected:
	template <class InterfaceType>
	static InterfaceType* ExtractTrayMessages(CGuiApplicationComp& component)
	{
		return &component.m_trayMessages;
	}

	class TrayMessages: virtual public ilog::IMessageConsumer
	{
	public:
		explicit TrayMessages(CGuiApplicationComp& parent);

		// reimplemented (ilog::IMessageConsumer)
		virtual bool IsMessageSupported(
					int messageCategory = -1,
					int messageId = -1,
					const istd::IInformationProvider* messagePtr = NULL) const;
		virtual void AddMessage(const MessagePtr& messagePtr);

	private:
		CGuiApplicationComp& m_parent;
	};

private:
	void UpdateMainWidgetDecorations();
	void ShowWindow();

private Q_SLOTS:
	void OnQuit();

protected:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_REF(iprm::IEnableableParam, m_allowApplicationCloseCompPtr);
	I_REF(imod::IModel, m_allowApplicationCloseModelCompPtr);
	I_REF(ibase::ICommandsProvider, m_trayIconCommandsCompPtr);
	I_ATTR(int, m_frameSpaceSizeAttrPtr);
	I_ATTR(int, m_uiStartModeAttrPtr);
	I_ATTR(bool, m_useMenuIconsOnMacAttrPtr);
	I_ATTR(bool, m_useTrayIconAttrPtr);

	/**
		\note The main widget must be deleted manually.
		For the main widget the deffered deleting in CGuiComponentBase::DestroyGui doesn't work, 
		because it is called after leaving the main loop.
	*/
	istd::TDelPtr<QWidget> m_mainWidgetPtr;
	Qt::WindowFlags m_defaultWidgetFlags;
	QRect m_lastWidgetGeometry;

	TrayMessages m_trayMessages;
	
	istd::TDelPtr<QSystemTrayIcon> m_trayIconPtr;
};


} // namespace iqtgui


#endif // iqtgui_CGuiApplicationComp_included

