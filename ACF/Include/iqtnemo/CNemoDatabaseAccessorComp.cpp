#include "QtAcf/QtAcf.h"

#include "iqtnemo/CNemoDatabaseAccessorComp.h"


namespace iqtnemo
{


CNemoDatabaseAccessorComp::CNemoDatabaseAccessorComp()
	:BaseClass(),
	m_databaseNameAttr("postgres", this, "DatabaseName"),
	m_hostNameAttr("localhost", this, "DatabaseHost"),
	m_userNameAttr("NEMO", this, "User"),
	m_passwordAttr("", this, "Password")
{
	registerInterface<inemo::INemoDatabaseAccessor>(this);

	m_tableModelPtr = NULL;
}


// reimplemented (acf::INemoDatabaseAccessor)

void CNemoDatabaseAccessorComp::RegisterConsumer(acf::ModelInterface* consumerPtr)
{
	m_modelConsumers.insert(consumerPtr);
}


bool CNemoDatabaseAccessorComp::IsDatabaseConnected() const
{
	return m_tableModelPtr != NULL;
}


bool CNemoDatabaseAccessorComp::ConnectToDatabase()
{
	return true;
}


bool CNemoDatabaseAccessorComp::DisconnectFromDatabase()
{
	return true;
}


int CNemoDatabaseAccessorComp::GetSensorCount() const
{
	if (m_tableModelPtr != NULL){
		return m_tableModelPtr->rowCount();
	}

	return 0;
}


acf::WideString CNemoDatabaseAccessorComp::GetSensorName(int sensorIndex) const
{
	if (sensorIndex >= 0 && sensorIndex < GetSensorCount()){
		QSqlRecord record = m_tableModelPtr->record(sensorIndex);
		if (!record.isNull("name")){
			QString sensorName = record.value("name").toString();

			return acf::acfString(sensorName);
		}
	}
		
	return acf::WideString();
}


int CNemoDatabaseAccessorComp::GetSensorId(int sensorIndex) const
{
	return 0;
}


int CNemoDatabaseAccessorComp::GetSensor(int sensorIndex) const
{
	return 0;
}


// reimplemented (acf::ComponentInterface)

bool CNemoDatabaseAccessorComp::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

		db.setHostName(acf::qtString(m_hostNameAttr.value()));
		db.setDatabaseName(acf::qtString(m_databaseNameAttr.value()));
		db.setUserName(acf::qtString(m_userNameAttr.value()));
		db.setPassword(acf::qtString(m_passwordAttr.value()));
		
		if (db.open()){
			m_tableModelPtr = new QSqlTableModel(this, db);
			m_tableModelPtr->setTable("nemo.sensors");
	
			connect(&m_checkModelTimer, SIGNAL(timeout()), this, SLOT(RefreshModel()));
			m_checkModelTimer.start(1000); 
		}
		else{
			QString error = db.lastError().text();

			qDebug(error);
		}

		return true;
	}

	return false;
}



// private slots

void CNemoDatabaseAccessorComp::RefreshModel()
{
	if (m_tableModelPtr != NULL){
		if (m_tableModelPtr->select()){
			for (ModelConsumers::iterator index = m_modelConsumers.begin(); index != m_modelConsumers.end(); index++){
				if (*index != NULL){
					(*index)->notifyUpdate();
				}
			}
		}
	}
}


} // namespace iqtnemo
