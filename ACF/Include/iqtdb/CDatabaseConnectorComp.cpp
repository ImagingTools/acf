#include "iqtdb/CDatabaseConnectorComp.h"

#include "iqt/iqt.h"


namespace iqtdb
{


CDatabaseConnectorComp::CDatabaseConnectorComp()
:	BaseClass(),
	m_databaseDriverNameAttr("ODBC", this, "DatabaseDriver"),
	m_databaseNameAttr("", this, "DatabaseName"),
	m_hostNameAttr("localhost", this, "DatabaseHost"),
	m_userNameAttr("", this, "User"),
	m_passwordAttr("", this, "Password")
{
	registerInterface<idb::IDatabaseConnector>(this);
}


// reimplemented (acf::IDatabaseConnector)

bool CDatabaseConnectorComp::IsDatabaseConnected() const
{
	return m_database.isOpen();
}


bool CDatabaseConnectorComp::ConnectToDatabase()
{
	return m_database.open();
}


bool CDatabaseConnectorComp::DisconnectFromDatabase()
{
	m_database.close();

	return !m_database.isOpen();
}


// reimplemented (acf::ComponentInterface)

bool CDatabaseConnectorComp::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		m_database = QSqlDatabase::addDatabase(iqt::GetQString(m_databaseDriverNameAttr.value()));

		m_database.setHostName(iqt::GetQString(m_hostNameAttr.value()));
		m_database.setDatabaseName(iqt::GetQString(m_databaseNameAttr.value()));
		m_database.setUserName(iqt::GetQString(m_userNameAttr.value()));
		m_database.setPassword(iqt::GetQString(m_passwordAttr.value()));

		return true;
	}

	return false;
}


void CDatabaseConnectorComp::onDeinitialize(acf::ComponentManagerInterface* managerPtr)
{
	DisconnectFromDatabase();

	BaseClass::onDeinitialize(managerPtr);
}


} // namespace iqtdb
