#ifndef iqt_IDockManager_included
#define iqt_IDockManager_included


#include "iqt/iqt.h"


#include "istd/IPolymorphic.h"


class QDockWidget;


namespace iqt
{


/**
	A common interface for docking window manager.
*/
class IDockManager: virtual public istd::IPolymorphic
{
public:
	virtual	void AddDockWidget(int flags, QDockWidget* widget) = 0;
	virtual void RemoveDockWidget(QDockWidget* widget) = 0;
};


} // namespace iqt


#endif // iqt_IDockManager_included

