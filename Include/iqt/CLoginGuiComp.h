#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt/TDesignerGuiCompBase.h"

#include "iqt/Generated/ui_CLoginGuiComp.h"


namespace iqt
{


class CLoginGuiComp: public TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLoginGuiComp)
	I_END_COMPONENT

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt


#endif // !iqt_CLoginGuiComp_included

