#include "iqtdb/CTableModelCompBase.h"

#include "iqt/iqt.h"

#include "istd/TChangeNotifier.h"


namespace iqtdb
{


CTableModelCompBase::CTableModelCompBase()
:	BaseClass(),
	m_databaseConnectorIfPtr(this, "DatabaseConnector"),
	m_tableNameAttr("", this, "TableName"),
	m_schemaNameAttr("", this, "SchemaName"),
	m_updateIntervallAttr(1, this, "UpdateIntervall")
{
	m_tableModelPtr = NULL;
}


// reimplemented (acf::ComponentInterface)

bool CTableModelCompBase::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		if (!m_databaseConnectorIfPtr->IsDatabaseConnected()){
			if (!m_databaseConnectorIfPtr->ConnectToDatabase()){
				QString error = QSqlDatabase().lastError().text();

				qDebug(error);

				return false;
			}
		}

		m_tableModelPtr = new QSqlRelationalTableModel(this);
		m_schemaName = iqt::GetQString(m_schemaNameAttr.value());
		m_tableName = iqt::GetQString(m_tableNameAttr.value());

		QString tableName = m_tableName;
		if (!m_schemaName.isEmpty()){
			tableName = m_schemaName + "." + tableName;
		}

		m_tableModelPtr->setTable(tableName);

		connect(&m_checkModelTimer, SIGNAL(timeout()), this, SLOT(RefreshModel()));
		m_checkModelTimer.start(m_updateIntervallAttr.value() * 1000); 

		m_checkModelTimer.singleShot(500, this, SLOT(RefreshModel())); 

		return true;
	}

	return false;
}


// protected methods

QString CTableModelCompBase::CalculateFullTableName(const QString& tableName) const
{
	QString fullTableName = tableName;

	if (tableName.isEmpty()){
		fullTableName = m_tableName;
	}

	if (!m_schemaName.isEmpty()){
		fullTableName = m_schemaName + "." + fullTableName;
	}

	return fullTableName;
}


// private slots

void CTableModelCompBase::RefreshModel()
{
	if (m_tableModelPtr != NULL){
		m_tableModelPtr->select();

		if (IsModelChanged()){
			UpdateModel();
		}
	}
}


} // namespace iqtdb
