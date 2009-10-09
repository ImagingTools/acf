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
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Applciation info object", true, "ApplicationInfo");
		I_ASSIGN(m_logoPathAttrPtr, "LogoPath", "Path of bitmap logo", false, ":/Icons/AcfLogoBig");
		I_ASSIGN(m_descriptionTextAttrPtr, "DescriptionText", "Optional description text", false, "(C) Copyright 2008 CompanyName\n\nAll rights reserved")
	I_END_COMPONENT;
	
protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

protected:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(istd::CString, m_logoPathAttrPtr);
	I_ATTR(istd::CString, m_descriptionTextAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CAboutWidgetGuiComp_included


