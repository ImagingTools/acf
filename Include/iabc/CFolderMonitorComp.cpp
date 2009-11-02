#include "iabc/CFolderMonitorComp.h"


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "isys/CSectionBlocker.h"

#include "iqt/CTimer.h"


namespace iabc
{


CFolderMonitorComp::CFolderMonitorComp()
	:m_notificationFrequency(10),
	m_poolingFrequency(60),
	m_finishThread(false)
{
}

	
// reimplemented (imod::CSingleModelObserverBase)

void CFolderMonitorComp::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	SetFolderPath(iqt::GetQString(m_fileNameParamCompPtr->GetPath()));
}


// reimplemented (ibase::IFolderMonitor)

istd::CStringList CFolderMonitorComp::GetChangedFileItems(int changeFlags) const
{
	istd::CStringList changedFilesList;
	if ((changeFlags & FilesAdded) != 0){
		istd::CStringList addedFiles = iqt::GetCStringList(m_folderChanges.addedFiles);
		changedFilesList.insert(changedFilesList.end(), addedFiles.begin(), addedFiles.end()); 
	}

	if ((changeFlags & FilesRemoved) != 0){
		istd::CStringList removedFiles = iqt::GetCStringList(m_folderChanges.removedFiles);
		changedFilesList.insert(changedFilesList.end(), removedFiles.begin(), removedFiles.end()); 
	}

	if ((changeFlags & FilesModified) != 0){
		istd::CStringList modifiedFiles = iqt::GetCStringList(m_folderChanges.modifiedFiles);
		changedFilesList.insert(changedFilesList.end(), modifiedFiles.begin(), modifiedFiles.end()); 
	}

	if ((changeFlags & FilesAttributeChanged) != 0){
		istd::CStringList attributeChangedFiles = iqt::GetCStringList(m_folderChanges.attributeChangedFiles);
		changedFilesList.insert(changedFilesList.end(), attributeChangedFiles.begin(), attributeChangedFiles.end()); 
	}

	return changedFilesList;
}


// reimplemented (icomp::IComponent)

void CFolderMonitorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_notificationFrequencyAttrPtr.IsValid()){
		m_notificationFrequency = *m_notificationFrequencyAttrPtr;
	}

	if (m_poolingFrequencyAttrPtr.IsValid()){
		m_poolingFrequency = *m_poolingFrequencyAttrPtr;
	}

	if (m_fileFilterExpressionsAttrPtr.IsValid()){
		for (int filterIndex = 0; filterIndex < m_fileFilterExpressionsAttrPtr.GetCount(); filterIndex++){
			m_fileFilterExpressions.push_back(iqt::GetQString(m_fileFilterExpressionsAttrPtr[filterIndex]));
		}
	}

	if (m_fileNameParamCompPtr.IsValid()){
		imod::IModel* fileNameModelPtr = dynamic_cast<imod::IModel*>(m_fileNameParamCompPtr.GetPtr());
		if (fileNameModelPtr != NULL){
			fileNameModelPtr->AttachObserver(this);
		}
	}

	connect(&m_fileSystemWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChanged(const QString&)));
	connect(this, SIGNAL(FolderChanged(int)), this, SLOT(OnFolderChanged(int)), Qt::QueuedConnection);

	// start 
	BaseClass2::start();
}


void CFolderMonitorComp::OnComponentDestroyed()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass2::isRunning()){
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}

	if (BaseClass2::isRunning()){
		BaseClass2::terminate();
	}

	if (m_fileNameParamCompPtr.IsValid()){
		imod::IModel* fileNameModelPtr = dynamic_cast<imod::IModel*>(m_fileNameParamCompPtr.GetPtr());
		if (fileNameModelPtr != NULL){
			fileNameModelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimplemented (QThread)

void CFolderMonitorComp::run()
{
	iqt::CTimer fullUpdateTimer;
	iqt::CTimer directoryChangesUpdateTimer;
	
	while (!m_finishThread){
		QDir folderDir(m_currentFolderPath);

		bool needFullDirectoryUpdate = (fullUpdateTimer.GetElapsed() >= m_poolingFrequency);
		bool needDirectoryChangesUpdate = (directoryChangesUpdateTimer.GetElapsed() >= m_notificationFrequency);
		bool needUpdate = ((needDirectoryChangesUpdate && !m_directoryChangesConfirmed)  || needFullDirectoryUpdate);
		if (!needUpdate){
			msleep(50);

			continue;
		}

		QFileInfoList currentFiles = folderDir.entryInfoList(m_fileFilterExpressions, QDir::AllEntries);

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		// check for changes:
		for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
			QFileInfo fileInfo = m_directoryFiles[fileIndex];
			if (!fileInfo.exists()){
				removedFiles.push_back(fileInfo.absoluteFilePath());
				SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
			}
		}

		for (int currentFileIndex = 0; currentFileIndex < int(currentFiles.count()); currentFileIndex++){
			const QFileInfo& currentFileInfo = currentFiles[currentFileIndex];
			bool newFile = true;
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
				const QFileInfo& fileInfo = m_directoryFiles[fileIndex];
				if (currentFileInfo == fileInfo){
					newFile = false;
					break;
				}
			}

			if (newFile){
				addedFiles.push_back(currentFileInfo.absoluteFilePath());
				SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was added"));
			}
		}

		m_directoryFiles = currentFiles;

		isys::CSectionBlocker block(&m_lock);

		m_folderChanges.addedFiles = addedFiles;
		m_folderChanges.removedFiles = removedFiles;
		m_folderChanges.modifiedFiles = modifiedFiles;
		m_folderChanges.attributeChangedFiles = attributeChangedFiles;

		int changeFlags = CF_MODEL;

		if (!addedFiles.isEmpty()){
			changeFlags |= FilesAdded;
		}

		if (!removedFiles.isEmpty()){
			changeFlags |= FilesRemoved;
		}

		if (!modifiedFiles.isEmpty()){
			changeFlags |= FilesModified;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeFlags |= FilesAttributeChanged;
		}

		Q_EMIT FolderChanged(changeFlags);

		m_directoryChangesConfirmed = true;
		fullUpdateTimer.Start();
		directoryChangesUpdateTimer.Start();
	}
}


// private slots

void CFolderMonitorComp::OnDirectoryChanged(const QString&/* directoryPath*/)
{
	m_directoryChangesConfirmed = false;
}


void CFolderMonitorComp::OnFolderChanged(int changeFlags)
{
	isys::CSectionBlocker block(&m_lock);

	istd::CChangeNotifier changePtr(this, changeFlags, &m_folderChanges);

	changePtr.Reset();
}


// private methods

void CFolderMonitorComp::SetFolderPath(const QString& folderPath)
{	
	isys::CSectionBlocker block(&m_lock);

	if (folderPath != m_currentFolderPath){
		if (!m_currentFolderPath.isEmpty()){
			m_fileSystemWatcher.removePath(m_currentFolderPath);
		}

		m_currentFolderPath = folderPath;
		QDir folderDir(m_currentFolderPath);
	
		m_directoryFiles = folderDir.entryInfoList(QDir::Dirs | QDir::Files);
		m_fileSystemWatcher.addPath(m_currentFolderPath);
	}
}


} // namespace iabc


