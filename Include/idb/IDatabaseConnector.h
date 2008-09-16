#ifndef idb_IDatabaseConnector_included
#define idb_IDatabaseConnector_included


#include "istd/IPolymorphic.h"


namespace idb
{


/**
	\ingroup idb

	IDatabaseConnector interface provides conrol of a database connection.
*/
class IDatabaseConnector: virtual public istd::IPolymorphic
{
public:
	virtual bool IsDatabaseConnected() const = 0;
	virtual bool ConnectToDatabase() = 0;
	virtual bool DisconnectFromDatabase() = 0;
};


} // namespace idb


#endif // !idb_IDatabaseConnector_included
