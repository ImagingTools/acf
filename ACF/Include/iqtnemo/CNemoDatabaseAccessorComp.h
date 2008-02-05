#ifndef iqtnemo_CNemoDatabaseAccessorComp_h_included
#define iqtnemo_CNemoDatabaseAccessorComp_h_included


#include "iqtnemo.h"


#include <QObject>
#include <QtSql>


#include "Comp/Component.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"


#include "inemo/INemoDatabaseAccessor.h"
#include "inemo/INemoDatabaseObserver.h"


namespace iqtnemo
{


class CNemoDatabaseAccessorComp:	public QObject,
									public acf::Component, 
									public inemo::INemoDatabaseAccessor										
{
public:
	typedef acf::Component BaseClass;

	CNemoDatabaseAccessorComp();

	// reimplemented (acf::INemoDatabaseAccessor)
	virtual bool IsDatabaseConnected() const;
	virtual bool ConnectToDatabase();
	virtual bool DisconnectFromDatabase();
	virtual int GetSensorCount() const;
	virtual acf::WideString GetSensorName(int sensorIndex) const;
	virtual int GetSensorId(int sensorIndex) const;
	virtual int GetSensor(int sensorIndex) const;

	// reimplemented (acf::ComponentInterface)
	virtual bool onInitialize(acf::ComponentManagerInterface* managerPtr);

private:
	QSqlTableModel* m_tableModelPtr;

	acf::StringAttribute m_databaseNameAttr;
	acf::StringAttribute m_hostNameAttr;
	acf::StringAttribute m_userNameAttr;
	acf::StringAttribute m_passwordAttr;
};


} // namespace iqtnemo


#endif // !iqtnemo_CNemoDatabaseAccessorComp_h_included

