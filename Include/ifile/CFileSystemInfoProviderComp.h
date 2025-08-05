#pragma once


// Qt includes
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileSystemInfoProvider.h>
#include <ibase/IRuntimeStatusProvider.h>
#include <imod/CMultiModelDispatcherBase.h>


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
		I_ASSIGN(m_autoUpdatePeriodAttr, "AutomaticUpdatePeriod", "Automatic updates period in seconds", false, 10);
		I_ASSIGN(m_runtimeStatusCompPtr, "RuntimeStatus", "Application's runtime status", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusModelCompPtr, m_runtimeStatusCompPtr, false);
	I_END_COMPONENT;

	CFileSystemInfoProviderComp();

	// reimplemented (ifile::IFileSystemInfoProvider)
	virtual const iprm::IOptionsList& GetDriveList() const override;
	virtual const istd::CSystem::FileDriveInfo* GetFileDriveInfo(int driveIndex = -1) const override;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (QThread)
	virtual void run() override;

protected:
	typedef std::vector<istd::CSystem::FileDriveDescriptor> DriveInfos;

Q_SIGNALS:
	void EmitUpdate(const DriveInfos& driveInfos);

private Q_SLOTS:
	void OnUpdate(const DriveInfos& driveInfos);

private:
	DriveInfos CalculateDriveInfos() const;

private:
	DriveInfos m_driveInfos;

	double m_sleepInterval;
	bool m_threadTerminationRequested;

	mutable QRecursiveMutex m_lock;
	mutable QMutex m_pollingMutex;
	mutable QWaitCondition m_pollingWait;

	I_ATTR(double, m_autoUpdatePeriodAttr);

	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusCompPtr);
	I_REF(imod::IModel, m_runtimeStatusModelCompPtr);
};


} // namespace ifile


