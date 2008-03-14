#ifndef iqt_IActionProvider_included
#define iqt_IActionProvider_included


#include "iqt/iqt.h"


#include "istd/IPolymorphic.h"


class QAction;


namespace iqt
{


class IActionProvider: virtual public istd::IPolymorphic
{
public:
	virtual int GetActionCount() const = 0;

	virtual QAction* GetAction(int actionIndex) const = 0;
};


} // namespace iqt


#endif // !iqt_IActionProvider_included
