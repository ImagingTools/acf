#ifndef iqt_CTabContainerGuiComp_included
#define iqt_CTabContainerGuiComp_included


#include "iqt.h"


#include <QTabWidget>

#include "QtAcf/QtGuiComponent.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "QtCompLib/QtIconProviderInterface.h"


namespace iqt
{


class CTabContainerGuiComp: public acf::QtGuiComponent<QTabWidget> 
{
public:
	typedef acf::QtGuiComponent<QTabWidget> BaseClass;

	CTabContainerGuiComp();
	virtual ~CTabContainerGuiComp();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected:
	acf::MultipleComponentDependency<iqt::IGuiObject> m_slaveGuisCompIfPtr;
	acf::ComponentDependency<acf::QtIconProviderInterface> m_iconsProviderCompIfPtr;
	acf::ComponentDependency<iqt::IGuiObject> m_cornerGuiCompIfPtr;
	acf::StringListAttribute m_tabNamesAttr;
	acf::IntAttribute m_iconSizeAttr;
	acf::BoolAttribute m_useTriangularTabsAttr;
	acf::IntAttribute m_tabOrientationAttr;
};


} // namespace iqt


#endif // !iqt_CTabContainerGuiComp_included

