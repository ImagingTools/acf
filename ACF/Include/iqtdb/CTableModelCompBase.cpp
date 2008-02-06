#include "QtAcf/QtAcf.h"

#include "iqtdb/CTableModelCompBase.h"


namespace iqtdb
{


CTableModelCompBase::CTableModelCompBase()
	:BaseClass(),
	m_databaseConnectorIfPtr(this, "DatabaseConnector"),
	m_tableNameAttr("", this, "TableName"),
	m_updateIntervallAttr(1, this, "UpdateIntervall")
{
	registerInterface<acf::ModelInterface>(this);

	m_tableModelPtr = NULL;
}


// reimplemented (acf::ComponentInterface)

bool CTableModelCompBase::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		if (!m_databaseConnectorIfPtr->IsDatabaseConnected()){
			if (m_databaseConnectorIfPtr->ConnectToDatabase()){
				m_tableModelPtr = new QSqlRelationalTableModel(this);
				m_tableModelPtr->setTable(acf::qtString(m_tableNameAttr.value()));
		
				connect(&m_checkModelTimer, SIGNAL(timeout()), this, SLOT(RefreshModel()));
				m_checkModelTimer.start(m_updateIntervallAttr.value() * 1000); 
			}
		}
		else{
			QString error = QSqlDatabase().lastError().text();

			qDebug(error);
		}

		return true;
	}

	return false;
}


// private slots

void CTableModelCompBase::RefreshModel()
{
	if (m_tableModelPtr != NULL){
		if (m_tableModelPtr->select()){
			if (IsModelChanged()){
				SynchronizeModelWithTable();

				notifyUpdate();
			}
		}
	}
}


} // namespace iqtdb
