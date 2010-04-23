#ifndef iqt_CExternalAppComp_included
#define iqt_CExternalAppComp_included


// Qt includes
#include <QObject>
#include <QProcess>


// ACF includes
#include "iprm/IFileNameParam.h"

#include "ibase/IApplication.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Component for execution of an external application.
*/
class CExternalAppComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ibase::IApplication
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalAppComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_applicationPathCompPtr, "ApplicationPath", "Path to the application's binary", true, "ApplicationPath");
	I_END_COMPONENT;

	CExternalAppComp();

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

protected Q_SLOTS:
	void OnError(QProcess::ProcessError error);
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	I_REF(iprm::IFileNameParam, m_applicationPathCompPtr);

	QProcess m_applicationProcess;
};


} // namespace iqt


#endif // !iqt_CExternalAppComp_included


