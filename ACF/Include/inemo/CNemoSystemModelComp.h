#ifndef CNemoSystemModelComp_h_included
#define CNemoSystemModelComp_h_included


#include "inemo/inemo.h"


#include "Model/HierarchicalModelTemplate.h"
#include "Model/MultiModelObserver.h"

#include "Comp/Component.h"
#include "Comp/ComponentDependency.h"
#include "Comp/Attribute.h"

#include "Base/FactorisableContainerTemplate.h"
#include "Base/ObjectFactoryInterface.h"
#include "Base/FactoryTemplate.h"

#include "inemo/INemoSystemModel.h"
#include "inemo/INemoSensor.h"


namespace inemo
{


class INemoDatabaseAccessor;


/**
	\ingroup inemo

	Standard implementation of NEMO data model.
*/
class CNemoSystemModelComp: public acf::Component,
							public acf::HierarchicalModelTemplate<inemo::INemoSystemModel>,
							protected acf::FactorisableContainerTemplate<inemo::INemoSensor>,
							protected acf::ObjectFactoryInterface<inemo::INemoSensor>
{
public:
	typedef acf::HierarchicalModelTemplate<inemo::INemoSystemModel> BaseClass;
	typedef acf::FactorisableContainerTemplate<inemo::INemoSensor> BaseClass2;
	typedef acf::Component BaseClass3;

	CNemoSystemModelComp();

	// reimplemented (inemo::INemoSystemModel)
	virtual int GetSensorCount() const;
	virtual inemo::INemoSensor& GetSensor(int sensorIndex) const;

	// reimplemented (acf::ObserverInterface)
	virtual void update(acf::ModelInterface* object);

	// reimplemented (acf::ObjectFactoryInterface)
	virtual inemo::INemoSensor* createObject();

	// reimplemented (acf::ComponentInterface)
	virtual bool onInitialize(acf::ComponentManagerInterface* managerPtr);

protected:
	void GetFromDatabase();

private:
	acf::ComponentDependency<inemo::INemoDatabaseAccessor> m_databaseAccessorIfPtr;
	acf::FactoryTemplate<inemo::INemoSensor> m_sensorFactory;
};


} // namespace inemo


#endif // !CNemoSystemModelComp_h_included