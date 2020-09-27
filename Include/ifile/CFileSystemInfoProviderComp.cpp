#include <ifile/CFileSystemInfoProviderComp.h>


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QMetaType>


namespace ifile
{


// public methods

CFileSystemInfoProviderComp::CFileSystemInfoProviderComp()
	:m_sleepInterval(0.0),
	m_threadTerminationRequested(false)
{
	qRegisterMetaType<DriveInfos>("DriveInfos");
}


// reimplemented (ifile::IFileSystemInfoProvider)

const iprm::IOptionsList& CFileSystemInfoProviderComp::GetDriveList() const
{
	return *this;
}


const istd::CSystem::FileDriveInfo* CFileSystemInfoProviderComp::GetFileDriveInfo(int driveIndex) const
{
	QMutexLocker locker(&m_lock);

	Q_ASSERT((driveIndex >= 0) && (driveIndex < int(m_driveInfos.size())));

	return &m_driveInfos[driveIndex].info;
}


// reimplemented (iprm::IOptionsList)

int CFileSystemInfoProviderComp::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CFileSystemInfoProviderComp::GetOptionsCount() const
{
	QMutexLocker locker(&m_lock);

	return int(m_driveInfos.size());
}


QString CFileSystemInfoProviderComp::GetOptionName(int index) const
{
	QMutexLocker locker(&m_lock);

	Q_ASSERT((index >= 0) && (index < int(m_driveInfos.size())));

	return m_driveInfos[index].name;
}


QString CFileSystemInfoProviderComp::GetOptionDescription(int index) const
{
	QMutexLocker locker(&m_lock);

	Q_ASSERT((index >= 0) && (index < int(m_driveInfos.size())));

	return m_driveInfos[index].name;
}


QByteArray CFileSystemInfoProviderComp::GetOptionId(int index) const
{
	QMutexLocker locker(&m_lock);

	Q_ASSERT((index >= 0) && (index < int(m_driveInfos.size())));

	return m_driveInfos[index].id;
}


bool CFileSystemInfoProviderComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileSystemInfoProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_runtimeStatusCompPtr.IsValid() && m_runtimeStatusModelCompPtr.IsValid()){
		RegisterModel(m_runtimeStatusModelCompPtr.GetPtr(), MI_RUNTIME_STATUS);
	}

	if (m_autoUpdatePeriodAttr.IsValid()){
		m_sleepInterval = *m_autoUpdatePeriodAttr;

		if (m_sleepInterval > 0){
			connect(this, SIGNAL(EmitUpdate(const DriveInfos&)), this, SLOT(OnUpdate(const DriveInfos&)), Qt::QueuedConnection);

			start();
		}
	}
}


void CFileSystemInfoProviderComp::OnComponentDestroyed()
{
	m_threadTerminationRequested = true;

	m_pollingWait.wakeAll();

	wait();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CFileSystemInfoProviderComp::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (modelId == MI_RUNTIME_STATUS){
		if (m_runtimeStatusCompPtr->GetRuntimeStatus() == ibase::IRuntimeStatusProvider::RS_SHUTDOWN){
			m_threadTerminationRequested = true;

			m_pollingWait.wakeAll();

			wait();
		}
	}
}


// reimplemented (QThread)

void CFileSystemInfoProviderComp::run()
{
	QMutexLocker lock(&m_pollingMutex);

	while (!m_threadTerminationRequested){
		DriveInfos driveInfos = CalculateDriveInfos();

		if (driveInfos != m_driveInfos){
			Q_EMIT EmitUpdate(driveInfos);
		}

		m_pollingWait.wait(&m_pollingMutex, m_sleepInterval * 1000);
	}
}


// private slots

void CFileSystemInfoProviderComp::OnUpdate(const DriveInfos& driveInfos)
{
	istd::CChangeNotifier changeNotifier(this);

	{
		QMutexLocker locker(&m_lock);

		m_driveInfos = driveInfos;
	}
}


// private methods

CFileSystemInfoProviderComp::DriveInfos CFileSystemInfoProviderComp::CalculateDriveInfos() const
{
	CFileSystemInfoProviderComp::DriveInfos driveInfos;

	QFileInfoList drives = QDir::drives();
	for (int i = 0; i < drives.count(); ++i){
		istd::CSystem::FileDriveDescriptor driveInfo;

		QString drivePath = drives[i].absolutePath();

		driveInfo.info = istd::CSystem::GetFileDriveInfo(drivePath);
		
		driveInfo.name = QDir::toNativeSeparators(drivePath);

		driveInfos.push_back(driveInfo);
	}

	return driveInfos;
}


} // namespace ifile


