#ifndef CRegistryPreviewComp_included
#define CRegistryPreviewComp_included


#include <QProcess>
#include <QLabel>


#include "icomp/CComponentBase.h"

#include "IRegistryPreview.h"


class CRegistryPreviewComp:	public QProcess,
							public icomp::CComponentBase,
							public IRegistryPreview
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryPreviewComp)
		I_REGISTER_INTERFACE(IRegistryPreview)
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (IRegistryPreview)
	virtual bool StartRegistry(const icomp::IRegistry& registry);
	virtual bool IsRunning() const;
	virtual void AbortRegistry();

protected slots:
	void OnFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	QLabel m_startLabel;
	QString m_tempFileName;
};


#endif // !CRegistryPreviewComp_included