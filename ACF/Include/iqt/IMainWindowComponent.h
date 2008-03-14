#ifndef iqt_IMainWindowComponent_included
#define iqt_IMainWindowComponent_included


#include "iqt/iqt.h"


#include "istd/IPolymorphic.h"


class QMainWindow;


namespace iqt
{


/**
	A common interface for main window component such as dock widget,tool bar and so on.
*/
class IMainWindowComponent: virtual public istd::IPolymorphic
{
public:
	virtual bool AddToMainWindow(QMainWindow& mainWindow) = 0;
};


} // namespace iqt


#endif // !iqt_IMainWindowComponent_included

