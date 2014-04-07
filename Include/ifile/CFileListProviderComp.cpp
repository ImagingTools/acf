#include "ifile/CFileListProviderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileIconProvider>
#else
#include <QtGui/QFileIconProvider>
#endif

// ACF includes
#include "istd/TChangeNotifier.h"


namespace ifile
{


// reimplemented (ifile::IFileListProvider)

const QFileInfoList& CFileListProviderComp::GetFileList() const
{
	return m_fileList;
}


// public static methods

bool CFileListProviderComp::CreateFileList(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth,
			const QStringList& nameFilters,
			QDir::SortFlags sortSpec,
			QFileInfoList& fileList,
			istd::ILogger* loggerPtr)
{
	QFileInfoList directories;
	if (!CreateDirectoryList(root, minRecursionDepth, maxRecursionDepth, directories, loggerPtr)){
		return false;
	}

	for (		QFileInfoList::const_iterator dirIter = directories.begin();
				dirIter != directories.end();
				++dirIter){
		QString dirPath = dirIter->absoluteFilePath();

		QDir dir(dirPath);
		dir.setFilter(QDir::Files);

		QStringList files = dir.entryList(nameFilters, QDir::Files, sortSpec);

		for (		QStringList::const_iterator fileIter = files.begin();
					fileIter != files.end();
					++fileIter){
			const QString& fileName = *fileIter;

			QString filePath = dir.absoluteFilePath(fileName);

			fileList.push_back(QFileInfo(filePath));
		}
	}

	return true;
}



bool CFileListProviderComp::CreateDirectoryList(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth,
			QFileInfoList& directoryList,
			istd::ILogger* loggerPtr)
{
	Q_ASSERT(minRecursionDepth >= 0);

	QString rootPath = root.absolutePath();

	if (rootPath.isEmpty()){
		return false;
	}

	QFileInfo fileInfo(rootPath);
	if (!fileInfo.isDir()){
		if (loggerPtr != NULL){
			loggerPtr->SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						0,
						QString(tr("Specified path '%1' is not a directory")).arg(root.absolutePath()),
						"File List Provder",
						0);
		}

		return false;
	}

	EnumerateDirectory(root, minRecursionDepth, maxRecursionDepth, directoryList);

	return true;
}


// protected static methods

void CFileListProviderComp::EnumerateDirectory(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth,
			QFileInfoList& directories)
{
	if (minRecursionDepth <= 0){
		QString rootPath = root.absolutePath();

		directories.push_back(QFileInfo(rootPath));
	}

	if (maxRecursionDepth == 0){
		return;
	}

	QStringList entries = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	for (		QStringList::const_iterator iter = entries.begin();
				iter != entries.end();
				++iter){
		const QString& subDirName = *iter;

		QDir subDir = root;
		subDir.setPath(root.absoluteFilePath(subDirName));

		EnumerateDirectory(subDir, minRecursionDepth - 1, maxRecursionDepth - 1, directories);
	}
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CFileListProviderComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	istd::CChangeNotifier notifier(this);

	m_fileList.clear();

	if (m_dirParamCompPtr.IsValid()){
		QStringList filters;

		m_directoryWatcher.removePaths(m_directoryWatcher.directories());

		QFileInfo watchingPathInfo(m_dirParamCompPtr->GetPath());
		if (watchingPathInfo.exists()){
			m_directoryWatcher.addPath(m_dirParamCompPtr->GetPath());		
		}
		else{
			SendWarningMessage(0, QString(tr("Specified path '%1' is not exist")).arg(m_dirParamCompPtr->GetPath()));		
		}

		if (m_fileTypeInfoCompPtr.IsValid()){
			QStringList extensions;
			if (m_fileTypeInfoCompPtr->GetFileExtensions(extensions)){
				for (		QStringList::const_iterator extIter = extensions.begin();
							extIter != extensions.end();
							++extIter){
					const QString& extension = *extIter;

					filters << "*." + extension;
				}
			}
		}
		else{
			int filtersCount = m_filtersAttrPtr.GetCount();
			for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
				filters << m_filtersAttrPtr[filterIndex];
			}
		}

		CreateFileList(m_dirParamCompPtr->GetPath(),
					*m_minRecurDepthAttrPtr,
					*m_maxRecurDepthAttrPtr,
					filters,
					QDir::Name | QDir::IgnoreCase,
					m_fileList);
	}
}


// reimplemented (icomp::CComponentBase)

void CFileListProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_directoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryContentChanged(const QString&)));

	if (m_dirParamModelCompPtr.IsValid()){
		m_dirParamModelCompPtr->AttachObserver(this);
	}
	else{
		OnUpdate(0, NULL);
	}
}


void CFileListProviderComp::OnComponentDestroyed()
{
	imod::CSingleModelObserverBase::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private slots

void CFileListProviderComp::OnDirectoryContentChanged(const QString& /*directoryPath*/)
{
	OnUpdate(0, NULL);
}


} // namespace ifile


