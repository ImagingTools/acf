#ifndef OLD_ACF_SUPPORT


#include "icomp/export.h"

#include "iqt/CLoginGuiComp.h"


I_EXPORT_PACKAGE("Qt", "Standard Qt package", "Qt Standard");
I_EXPORT_COMPONENT(iqt::CLoginGuiComp, "LoginGui", "Gui used to user login", "User Login Qt");
	

#else OLD_ACF_SUPPORT
#ifndef CQtPackage_included
#define CQtPackage_included

#include "Comp/ComponentLibrary.h"


class CQtPackage: public acf::ComponentLibrary
{
public:
	typedef acf::ComponentLibrary BaseClass;

	CQtPackage();
};


#endif // !CQtPackage_included
#endif // OLD_ACF_SUPPORT


