#ifndef iqtgui_CAboutWidgetGuiComp_included
#define iqtgui_CAboutWidgetGuiComp_included


// ACF includes
#include "ibase/IApplicationInfo.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "Generated/ui_CAboutWidgetGuiComp.h"


namespace iqtgui
{


/**
	Standart component for displaying on "On About..." action.
*/
class CAboutWidgetGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CAboutWidgetGuiComp>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CAboutWidgetGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CAboutWidgetGuiComp);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info object", true, "ApplicationInfo");
		I_ASSIGN(m_logoPathAttrPtr, "LogoPath", "Path of bitmap logo", false, "");
		I_ASSIGN(m_showProductNameAttrPtr, "ShowProductName", "If true, product name will be shown, disable it, if it was painted on background image", true, true);
		I_ASSIGN(m_showApplicationNameAttrPtr, "ShowApplicationName", "If true, application name will be shown, disable it, if it was painted on background image", true, true);
		I_ASSIGN(m_showApplicationSubnameAttrPtr, "ShowApplicationSubName", "If true, application subname will be shown, disable it, if it was painted on background image", true, false);
		I_ASSIGN(m_showProductTypeAttrPtr, "ShowProductType", "Additional product description shown over version info", true, false);
		I_ASSIGN(m_showLegalCopyrightAttrPtr, "ShowLegalCopyright", "If enabled legal copyrights will be shown", true, true);
		I_ASSIGN_MULTI_1(m_versionIdsAttrPtr, "VersionIds", "List of version ID's for version infor should be displayed", false, 0);
		I_ASSIGN_MULTI_1(m_versionNamesAttrPtr, "VersionNames", "List of version names will be used as version description", false, "Version");
	I_END_COMPONENT;
	
protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

protected:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(QString, m_logoPathAttrPtr);
	I_ATTR(bool, m_showProductNameAttrPtr);
	I_ATTR(bool, m_showApplicationNameAttrPtr);
	I_ATTR(bool, m_showApplicationSubnameAttrPtr);
	I_ATTR(bool, m_showProductTypeAttrPtr);
	I_ATTR(bool, m_showLegalCopyrightAttrPtr);
	I_MULTIATTR(int, m_versionIdsAttrPtr);
	I_MULTIATTR(QString, m_versionNamesAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CAboutWidgetGuiComp_included


