#ifndef INemoDatabaseAccessor_h_included
#define INemoDatabaseAccessor_h_included


#include "inemo/inemo.h"

#include "istd/IPolymorphic.h"

#include "Base/WideString.h"


namespace inemo
{


class INemoSystemModel;

/**
	\ingroup inemo

	INemoDatabaseAccessor is an low level interface to access NEMO database.	
*/
class INemoDatabaseAccessor: virtual public istd::IPolymorphic
{
public:
	virtual bool IsDatabaseConnected() const = 0;
	virtual bool ConnectToDatabase() = 0;
	virtual bool DisconnectFromDatabase() = 0;
	virtual int GetSensorCount() const = 0;
	virtual acf::WideString GetSensorName(int sensorIndex) const = 0;
	virtual int GetSensorId(int sensorIndex) const = 0;
	virtual int GetSensor(int sensorIndex) const = 0;
};


} // namespace inemo


#endif // !INemoDatabaseAccessor_h_included
