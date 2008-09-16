#ifndef iqtdb_CDatabaseConnectorComp_included
#define iqtdb_CDatabaseConnectorComp_included


#include "iqtdb.h"


#include <set>


#include <QObject>
#include <QtSql>


#include "Comp/Component.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"


#include "idb/IDatabaseConnector.h"

#include "ibase/IMessageContainer.h"


namespace iqtdb
{

/**
	Qt based component implementation of IDatabaseConnector interface using QSqlDatabase class.	
*/
class CDatabaseConnectorComp: public QObject, public acf::Component, virtual public idb::IDatabaseConnector
{
public:
	typedef acf::Component BaseClass;

	CDatabaseConnectorComp();

	// reimplemented (idb::IDatabaseConnector)
	virtual bool IsDatabaseConnected() const;
	virtual bool ConnectToDatabase();
	virtual bool DisconnectFromDatabase();

	// reimplemented (acf::ComponentInterface)
	virtual bool OnInitialize(acf::ComponentManagerInterface* managerPtr);
	virtual void OnDeinitialize(acf::ComponentManagerInterface* managerPtr);

private:
	QSqlDatabase m_database;

	acf::StringAttribute m_databaseDriverNameAttr;
	acf::StringAttribute m_databaseNameAttr;
	acf::StringAttribute m_hostNameAttr;
	acf::StringAttribute m_userNameAttr;
	acf::StringAttribute m_passwordAttr;

	acf::ComponentDependency<ibase::IMessageContainer> m_logCompPtr;
};


} // namespace iqtdb


#endif // !iqtdb_CDatabaseConnectorComp_included

