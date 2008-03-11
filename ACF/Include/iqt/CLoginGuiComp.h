#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt/TDesignerGuiComponentBase.h"

#include "iqt/GeneratedFiles/ui_CLoginGuiComp.h"


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


