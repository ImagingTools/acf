#ifndef ifile_CFileSystemInfoProviderComp_included
#define ifile_CFileSystemInfoProviderComp_included


// Qt includes
#include <QThread>
#include <QMutex>

// ACF includes
#include "ilog/TLoggerCompWrap.h"
#include "ifile/IFileSystemInfoProvider.h"
#include "ibase/IRuntimeStatusProvider.h"
#include "imod/CMultiModelDispatcherBase.h"


namespace ifile
{


class CFileSystemInfoProviderComp:
			protected QThread,
			public ilog::CLoggerComponentBase, 
			virtual public ifile::IFileSystemInfoProvider,
			virtual public iprm::IOptionsList,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum ModelId
	{
		MI_RUNTIME_STATUS = 0
	};

	I_BEGIN_COMPONENT(CFileSystemInfoProviderComp);
		I_REGISTER_INTERFACE(ifile::IFileSystemInfoProvider);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_ASSIGN(m_autoUpdatePeriodAttr, "AutomaticUpdatePeriod", "Automatic updates period, s", false, 10);
		I_ASSIGN(m_runtimeStatusCompPtr, "RuntimeStatus", "Application's runtime status", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusModelCompPtr, m_runtimeStatusCompPtr, false);
	I_END_COMPONENT;

	CFileSystemInfoProviderComp();

	// reimplemented (ifile::IFileSystemInfoProvider)
	virtual const iprm::IOptionsList& GetDriveList() const;
	virtual const istd::CSystem::FileDriveInfo* GetFileDriveInfo(int driveIndex = -1) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (QThread)
	virtual void run();

protected:
	struct DriveInfo
	{
		istd::CSystem::FileDriveInfo info;
		QString name;
		QByteArray id;

		bool operator == (const DriveInfo& driveInfo) const
		{
			return (info == driveInfo.info) && (name == driveInfo.name) && (id == driveInfo.id);
		}
	};

	typedef std::vector<DriveInfo> DriveInfos;

Q_SIGNALS:
	void EmitUpdate(const DriveInfos& driveInfos);

private Q_SLOTS:
	void OnUpdate(const DriveInfos& driveInfos);

private:
	DriveInfos CalculateDriveInfos() const;

private:
	DriveInfos m_driveInfos;

	int m_sleepInterval;
	bool m_threadTerminationRequested;

	mutable QMutex m_lock;

	I_ATTR(int, m_autoUpdatePeriodAttr);

	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusCompPtr);
	I_REF(imod::IModel, m_runtimeStatusModelCompPtr);
};


} // namespace ifile


#endif // !ifile_CFileSystemInfoProviderComp_included


