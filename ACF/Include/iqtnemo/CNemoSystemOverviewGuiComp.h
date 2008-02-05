#ifndef iqtnemo_CNemoSystemOverviewGuiComp_h_included
#define iqtnemo_CNemoSystemOverviewGuiComp_h_included


#include "iqtnemo.h"


#include "QtAcf/QtGuiTemplateBasedComponent.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "QtCompLib/QtIconProviderInterface.h"

#include "generatedfiles/ui_CNemoSystemOverviewGuiComp.h"


namespace iqtnemo
{


class CNemoSystemOverviewGuiComp: public acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSystemOverviewGuiComp> 
{
public:
	typedef acf::QtGuiTemplateBasedComponent<QWidget, Ui::CNemoSystemOverviewGuiComp> BaseClass;

	CNemoSystemOverviewGuiComp();
	virtual ~CNemoSystemOverviewGuiComp();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void initializeGui();
	virtual void destroyGui();

protected:
	acf::ComponentDependency<acf::QtGuiInterface> m_sensorListGuiIfPtr;
	acf::ComponentDependency<acf::QtGuiInterface> m_systemOverviewGuiIfPtr;
	acf::ComponentDependency<acf::QtGuiInterface> m_sensorDataGuiIfPtr;
//	acf::ComponentDependency<acf::ObserverInterface> m_sensorDataObserverIfPtr;
//	acf::ComponentDependency<acf::ObserverInterface> m_systemOverviewObserverIfPtr;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoSystemOverviewGuiComp_h_included

