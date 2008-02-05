#ifndef INemoDatabaseObserver_h_included
#define INemoDatabaseObserver_h_included


#include "inemo/inemo.h"

#include "istd/IPolymorphic.h"


namespace inemo
{


class INemoSystemModel;

/**
	\ingroup inemo

	INemoDatabaseObserver watching the changes in the database and updates the registered
	model.
*/
class INemoDatabaseObserver: virtual public istd::IPolymorphic
{
public:
	virtual void OnModelChanged(INemoSystemModel* modelPtr) = 0;
};


} // namespace inemo


#endif // !INemoDatabaseObserver_h_included
