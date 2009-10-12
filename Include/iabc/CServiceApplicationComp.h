#ifndef iabc_CServiceApplicationComp_included
#define iabc_CServiceApplicationComp_included


// Qt includes
#include <QSystemTrayIcon>
#include "QtService.h"


// ACF includes
#include "iabc/iabc.h"


#include "ibase/IApplication.h"
#include "ibase/TLoggerCompWrap.h"

#include "icomp/TAttribute.h"
#include "icomp/CComponentBase.h"


namespace iabc
{


/**
	Qt based component for service-based application.
*/
class CServiceApplicationComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			public ibase::IApplication
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CServiceApplicationComp)
		I_REGISTER_INTERFACE(ibase::IApplication)
		I_ASSIGN(m_applicationCompPtr, "ApplicationInstance", "Service application object", true, "Application");
		I_ASSIGN(m_serviceDescriptionAttrPtr, "SeriviceDescription", "Service description", true, "This services provides...");
		I_ASSIGN(m_serviceNameAttrPtr, "ServiceName", "The name of the service", true, "MyService");
		I_ASSIGN(m_manualStartupAttrPtr, "ManualStartup", "If enabled, the service is registered with manual start up", false, false);
	I_END_COMPONENT

	QStringList GetApplicationArguments(int argc, char** argv) const;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourceObject, QEvent* eventPtr);

protected Q_SLOTS:
	void OnTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
	class CService: public QtServiceBase
	{
	public:
		typedef QtServiceBase BaseClass;

		CService(CServiceApplicationComp& parent,
					ibase::IApplication& application,
					int serviceArgc,
					char** serviceArgv,
					const QString &name);

	protected:
		// reimplemented (QtServiceBase)
		virtual void start();
		virtual void stop();
		virtual void pause();
   		virtual void resume();
		virtual void createApplication(int &argc, char **argv);
		virtual int executeApplication();

	private:
		QVector<char*> GetApplicationArguments() const;

	private:
		CServiceApplicationComp& m_parent;
		ibase::IApplication& m_application;
		QStringList m_applicationArguments;
	};

private:
	I_REF(ibase::IApplication, m_applicationCompPtr);
	I_ATTR(istd::CString, m_serviceDescriptionAttrPtr);
	I_ATTR(istd::CString, m_serviceNameAttrPtr);
	I_ATTR(bool, m_manualStartupAttrPtr);

	istd::TDelPtr<CService> m_servicePtr;
};


} // namespace iabc


#endif // iabc_CServiceApplicationComp_included

