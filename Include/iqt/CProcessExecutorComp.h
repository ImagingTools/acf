#ifndef iqt_CProcessExecutorComp_included
#define iqt_CProcessExecutorComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QMutex>


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
	virtual void SetEnvironment(const isys::IProcessEnvironment& processEnvironment);
	virtual int ExecuteProcess(const QString& executablePath, const QStringList& processArguments);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnError(QProcess::ProcessError error);
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	QProcess m_applicationProcess;
	QMutex m_mutex;

	bool m_isFailed;
};


} // namespace iqt


#endif // !iqt_CProcessExecutorComp_included


