#ifndef iqtdb_CTableModelCompBase_included
#define iqtdb_CTableModelCompBase_included


#include "iqtdb.h"


#include <set>


#include <QObject>
#include <QtSql>


#include "Comp/Component.h"
#include "Comp/Attribute.h"
#include "Comp/MultipleAttribute.h"
#include "Comp/MultipleComponentDependency.h"

#include "istd/IChangeable.h"

#include "idb/IDatabaseConnector.h"
#include "idb/IDatabaseTableModelProvider.h"

#include "ibase/IMessageContainer.h"


namespace iqtdb
{


/**
	Basic implementation of an table based model wrapper.

	You can use this class as a basic implementation of your model based wrapper for a database table.
	You will need to reimplement to IsModelChanged() and SynchronizeModelWithTable() 
	for enable the update delegation to the connected observers.
*/
class CTableModelCompBase:	public QObject,
							public acf::Component

{
	Q_OBJECT
public:
	typedef acf::Component BaseClass;

	CTableModelCompBase();

	// reimplemented (acf::ComponentInterface)
	virtual bool OnInitialize(acf::ComponentManagerInterface* managerPtr);

protected:
	/**
		Abstract method for update database table with the contents of the model.
	*/
	virtual void UpdateTableFromModel() = 0;

	/**
		Abstract method for update model with the contents of the database table.
	*/
	virtual void UpdateModelFromTable() = 0;

	/**
		Abstract method for checking the model changes.
	*/
	virtual bool IsModelChanged() const = 0;

	QString CalculateFullTableName(const QString& tableName = QString()) const;
	void AddSqlMessage(const QSqlError& error, const QString& source = "Database") const;

private slots:
	void RefreshModel();

protected:
	QSqlRelationalTableModel* m_tableModelPtr;
	QString m_schemaName;
	QString m_tableName;
	acf::ComponentDependency<ibase::IMessageContainer> m_logCompPtr;

private:
	QTimer m_checkModelTimer;

	acf::ComponentDependency<idb::IDatabaseConnector> m_databaseConnectorIfPtr;

	acf::StringAttribute m_tableNameAttr;
	acf::StringAttribute m_schemaNameAttr;
	acf::IntAttribute m_updateIntervallAttr;
};


} // namespace iqtdb


#endif // !iqtdb_CTableModelCompBase_included

