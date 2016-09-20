#ifndef iqtgui_CSplashScreen_included
#define iqtgui_CSplashScreen_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QSplashScreen>
#else
#include <QtGui/QSplashScreen>
#endif

// ACF includes
#include <iattr/TAttribute.h>
#include <ibase/IApplicationInfo.h>
#include <iqtgui/TDesignerGuiCompBase.h>
#include <ilog/IMessageConsumer.h>

#include <GeneratedFiles/iqtgui/ui_CSplashScreenGuiComp.h>


namespace iqtgui
{


/**
	Splash screen component.
	This component allows to create splash screen with additionally information about version, copyright an so on.
*/
class CSplashScreenGuiComp: 
			public TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen>,
			virtual public ilog::IMessageConsumer
{
	Q_OBJECT
public:
	typedef TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen> BaseClass;

	I_BEGIN_COMPONENT(CSplashScreenGuiComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Version management for application", false, "Application");
		I_ASSIGN(m_imagePathAttrPtr, "ImagePath", "Path of image shown as splash screen", true, "Splash.jpg");
		I_ASSIGN(m_showProductNameAttrPtr, "ShowProductName", "If true, product name will be shown, disable it, if it was painted on background image", true, true);
		I_ASSIGN(m_showApplicationNameAttrPtr, "ShowApplicationName", "If true, application name will be shown, disable it, if it was painted on background image", true, true);
		I_ASSIGN(m_showApplicationSubnameAttrPtr, "ShowApplicationSubName", "If true, application subname will be shown, disable it, if it was painted on background image", true, false);
		I_ASSIGN(m_showProductTypeAttrPtr, "ShowProductType", "Additional product description shown over version info", true, false);
		I_ASSIGN(m_showLegalCopyrightAttrPtr, "ShowLegalCopyright", "If enabled legal copyrights will be shown", true, true);
		I_ASSIGN_MULTI_1(m_versionIdsAttrPtr, "VersionIds", "List of version ID's for version information should be displayed", false, 0);
		I_ASSIGN_MULTI_1(m_versionNamesAttrPtr, "VersionNames", "List of version names will be used as version description", false, "Version");
	I_END_COMPONENT;
	
	CSplashScreenGuiComp();

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
		int messageCategory = -1,
		int messageId = -1,
		const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

Q_SIGNALS:
	void EmitAddMessage(const MessagePtr& messagePtr);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnAddMessage(const MessagePtr& messagePtr);

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(QString, m_imagePathAttrPtr);
	I_ATTR(bool, m_showProductNameAttrPtr);
	I_ATTR(bool, m_showApplicationNameAttrPtr);
	I_ATTR(bool, m_showApplicationSubnameAttrPtr);
	I_ATTR(bool, m_showProductTypeAttrPtr);
	I_ATTR(bool, m_showLegalCopyrightAttrPtr);
	I_MULTIATTR(int, m_versionIdsAttrPtr);
	I_MULTIATTR(QString, m_versionNamesAttrPtr);

	int m_mainVersionId;
};


} // namespace iqtgui


#endif // iqtgui_CSplashScreen_included


