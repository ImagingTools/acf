#ifndef OLD_ACF_SUPPORT
#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt/TDesignerGuiComponentBase.h"

#include "iqt/Generated/ui_CLoginGuiComp.h"


namespace iqt
{


class CLoginGuiComp: public TDesignerGuiComponentBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TDesignerGuiComponentBase<Ui::CLoginGuiComp, QWidget> BaseClass;

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt


#endif // !iqt_CLoginGuiComp_included
#else
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
#endif // OLD_ACF_SUPPORT

