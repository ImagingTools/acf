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
	m_finishThread(false),
	m_fileSystemWatcher(this)
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
	isys::CSectionBlocker block(&m_lock);
	
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
}


void CFolderMonitorComp::OnComponentDestroyed()
{
	StopObserverThread();

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
	
	I_ASSERT(!m_currentFolderPath.isEmpty());

	while (!m_finishThread){
		QDir folderDir(m_currentFolderPath);

		bool needFullDirectoryUpdate = (fullUpdateTimer.GetElapsed() >= m_poolingFrequency);
		bool needDirectoryChangesUpdate = (directoryChangesUpdateTimer.GetElapsed() >= m_notificationFrequency);
		bool needUpdate = ((needDirectoryChangesUpdate && !m_directoryChangesConfirmed)  || needFullDirectoryUpdate);
		if (!needUpdate){
			msleep(50);

			continue;
		}

		QDir::Filters itemFilter = QDir::AllEntries;
		if (m_observingItemsAttrPtr.IsValid()){
			itemFilter = QDir::Filters(*m_observingItemsAttrPtr);
		}

		QFileInfoList currentFiles = folderDir.entryInfoList(m_fileFilterExpressions, itemFilter);

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		int observingFlags = OC_ALL;

		if (m_observingChangesAttrPtr.IsValid()){
			observingFlags = *m_observingChangesAttrPtr;
		}

		// check for changes:

		if ((observingFlags & OC_REMOVE) != 0){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
				QFileInfo fileInfo = m_directoryFiles[fileIndex];
				if (!fileInfo.exists()){
					removedFiles.push_back(fileInfo.absoluteFilePath());
					SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
				}
			}
		}

		if (observingFlags != OC_REMOVE){
			for (int currentFileIndex = 0; currentFileIndex < int(currentFiles.count()); currentFileIndex++){
				const QFileInfo& currentFileInfo = currentFiles[currentFileIndex];

				QFileInfoList::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileInfo);
				if (foundFileIter == m_directoryFiles.end()){
					if ((observingFlags & OC_ADD) != 0){
						addedFiles.push_back(currentFileInfo.absoluteFilePath());
						SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was added"));
					}
				}
				else{
					if (foundFileIter->lastModified() != currentFileInfo.lastModified()){
						if ((observingFlags & OC_MODIFIED) != 0){
							modifiedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was modified"));
						}
					}
					if (foundFileIter->permissions() != currentFileInfo.permissions()){
						if ((observingFlags & OC_ATTR_CHANGED) != 0){
							attributeChangedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(currentFileInfo.absoluteFilePath() + " have been changed"));
						}
					}
				}
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
	istd::CChangeNotifier changePtr(this, changeFlags, &m_folderChanges);

	changePtr.Reset();
}


// private methods

void CFolderMonitorComp::SetFolderPath(const QString& folderPath)
{	
	if (folderPath != m_currentFolderPath){
		SendInfoMessage(0, istd::CString("Start observing of: ") + iqt::GetCString(folderPath), "FolderMonitor");

		if (!m_currentFolderPath.isEmpty()){
			m_fileSystemWatcher.removePath(m_currentFolderPath);
		}

		StopObserverThread();

		m_currentFolderPath = folderPath;
		m_directoryFiles.clear();
		m_folderChanges.addedFiles.clear();
		m_folderChanges.attributeChangedFiles.clear();
		m_folderChanges.modifiedFiles.clear();
		m_folderChanges.removedFiles.clear();

		QFileInfo fileInfo(m_currentFolderPath);
		if (fileInfo.exists()){
			QDir folderDir(m_currentFolderPath);	
			
			m_directoryFiles = folderDir.entryInfoList(QDir::Dirs | QDir::Files);
			m_fileSystemWatcher.addPath(m_currentFolderPath);

			StartObserverThread();
		}
		else{
			SendWarningMessage(0, istd::CString("Directory: ") + iqt::GetCString(folderPath) + istd::CString(" not exists. Observing aborted"), "FolderMonitor");
		}
	}
}


void CFolderMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	BaseClass2::start();
}


void CFolderMonitorComp::StopObserverThread()
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
}


} // namespace iabc


