#include "QtAcf/QtAcf.h"

#include "iqtdb/CDatabaseConnectorComp.h"


namespace iqtdb
{


CDatabaseConnectorComp::CDatabaseConnectorComp()
	:BaseClass(),
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
		m_database = QSqlDatabase::addDatabase(acf::qtString(m_databaseDriverNameAttr.value()));

		m_database.setHostName(acf::qtString(m_hostNameAttr.value()));
		m_database.setDatabaseName(acf::qtString(m_databaseNameAttr.value()));
		m_database.setUserName(acf::qtString(m_userNameAttr.value()));
		m_database.setPassword(acf::qtString(m_passwordAttr.value()));

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
