#ifndef icmpstr_CRegistryPreviewComp_included
#define icmpstr_CRegistryPreviewComp_included


// Qt includes
#include <QtCore/QProcess>
#include <QtGui/QLabel>

// ACF includes
#include "icomp/IComponentEnvironmentManager.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"

#include "icmpstr/IRegistryPreview.h"


namespace icmpstr
{


class CRegistryPreviewComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			public IRegistryPreview
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryPreviewComp);
		I_REGISTER_INTERFACE(icmpstr::IRegistryPreview);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Version info used to serialize temporary registry file", false, "VersionInfo");
		I_ASSIGN(m_commandFileNameCompPtr, "CommandFileName", "Parameter storing command file name", true, "CommandFileName");
		I_ASSIGN(m_envManagerCompPtr, "EnvironmentManager", "Registry loader used to retrive package configuration", false, "EnvironmentManager");
	I_END_COMPONENT;

	CRegistryPreviewComp();

	// reimplemented (icmpstr::IRegistryPreview)
	virtual bool StartRegistry(const icomp::IRegistry& registry);
	virtual bool IsRunning() const;
	virtual void AbortRegistry();

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnStateChanged(QProcess::ProcessState state);
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();

private:
	QString m_tempFileName;
	QProcess m_process;

	bool m_isRunning;

	I_REF(iprm::IFileNameParam, m_commandFileNameCompPtr);
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPreviewComp_included


