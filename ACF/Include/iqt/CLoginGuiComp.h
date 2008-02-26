#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt.h"


#include "QtAcf/QtGuiTemplateBasedComponent.h"
#include "QtAcf/QtAbstractGuiObserverComp.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"

#include "QtCompLib/QtIconProviderInterface.h"

#include "ui_CLoginGuiComp.h"


namespace iqt
{


class CLoginGuiComp: public acf::QtGuiTemplateBasedComponent<QWidget, Ui::CLoginGuiComp>
{
public:
	typedef acf::QtGuiTemplateBasedComponent<QWidget, Ui::CLoginGuiComp> BaseClass;

	CLoginGuiComp();
	virtual ~CLoginGuiComp();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void initializeGui();
};


} // namespace iqt


#endif // !iqt_CLoginGuiComp_included

