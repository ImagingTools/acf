#ifndef iqtdb_CTableModelCompBase_h_included
#define iqtdb_CTableModelCompBase_h_included


#include "iqtdb.h"


#include <set>


#include <QObject>
#include <QtSql>


#include "Comp/Component.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"


#include "idb/IDatabaseConnector.h"

#include "inemo/INemoSensors.h"
#include "inemo/CNemoSensor.h"


namespace iqtdb
{


/**
	Basic implementation of an table based model wrapper.

	You can use this class as a basic implementation of your model based wrapper for a database table.
	You will need to reimplement to IsModelChanged() and SynchronizeModelWithTable() 
	for enable the update delegation to the connected observers.
*/
class CTableModelCompBase:	public QObject,
							public acf::Component,
							public acf::Model
{
	Q_OBJECT
public:
	typedef acf::Component BaseClass;

	CTableModelCompBase();

	// reimplemented (acf::ComponentInterface)
	virtual bool onInitialize(acf::ComponentManagerInterface* managerPtr);

protected:
	/**
		Abstract method for checking the model changes.
	*/
	virtual bool IsModelChanged() const = 0;

	/**
		Abstract method to create the model data from the table content.
	*/
	virtual void SynchronizeModelWithTable() = 0;

private slots:
	void RefreshModel();

protected:
	QSqlRelationalTableModel* m_tableModelPtr;

private:
	QTimer m_checkModelTimer;

	acf::ComponentDependency<idb::IDatabaseConnector> m_databaseConnectorIfPtr;
	acf::StringAttribute m_tableNameAttr;
	acf::IntAttribute m_updateIntervallAttr;
};


} // namespace iqtdb


#endif // !iqtdb_CTableModelCompBase_h_included

