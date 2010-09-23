#ifndef iqt_CProcessExecutorComp_included
#define iqt_CProcessExecutorComp_included


// Qt includes
#include <QObject>
#include <QProcess>
#include <QMutex>


// ACF includes
#include "ibase/IProcessExecutor.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Component for execution of an external application.
*/
class CProcessExecutorComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ibase::IProcessExecutor
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProcessExecutorComp);
		I_REGISTER_INTERFACE(ibase::IProcessExecutor);
	I_END_COMPONENT;

	CProcessExecutorComp();

	// reimplemented (ibase::IProcessExecutor)
	virtual void SetEnvironment(const isys::IApplicationEnvironment& processEnvironment);

	virtual int Execute(const istd::CString& executablePath, const istd::CStringList& processArguments);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnError(QProcess::ProcessError error);
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	QProcess m_applicationProcess;
	QMutex m_lock;

	bool m_isFailed;
};


} // namespace iqt


#endif // !iqt_CProcessExecutorComp_included


