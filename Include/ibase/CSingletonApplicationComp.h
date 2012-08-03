#ifndef ibase_CSingletonApplicationComp_included
#define ibase_CSingletonApplicationComp_included


// Qt includes
#include <QSharedMemory>

// ACF includes
#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"


namespace ibase
{


/**
	Singleton application component.
	This component wraps a "real" application component implementation and allows only a single execution of the application instance. 
*/
class CSingletonApplicationComp:
			public icomp::CComponentBase, 
			virtual public ibase::IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSingletonApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to identify the application in the system", false, "ApplicationInfo");
		I_ASSIGN(m_slaveApplicationCompPtr, "Application", "Application component to be executed", true, "Application");
	I_END_COMPONENT;

	CSingletonApplicationComp();

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	/**
		Info about running application.
	*/
	struct RunningProcessInfo
	{
		qint64 processId;
	};

	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(ibase::IApplication, m_slaveApplicationCompPtr);

	istd::TDelPtr<QSharedMemory> m_processData;
	bool m_isAlreadyRunning;
};


} // namespace ibase


#endif // ibase_CSingletonApplicationComp_included

