#include "ifile/CFileTreeProviderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtGui/QFileIconProvider>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace ifile
{


// reimplemented (ibase::IQtItemModelProvider)

const QAbstractItemModel* CFileTreeProviderComp::GetItemModel() const
{
	return &m_itemModel;
}


// protected methods

bool CFileTreeProviderComp::CreateFileList(
	const QDir& root,
	const QStringList& nameFilters,
	QDir::SortFlags sortSpec,
	QStandardItem* parentItemPtr)
{
	QDir dir(root.absolutePath());
	dir.setFilter(QDir::Files);

	QStringList files = dir.entryList(nameFilters, QDir::Files, sortSpec);

	for (		QStringList::const_iterator fileIter = files.begin();
		fileIter != files.end();
		++fileIter){
			const QString& fileName = *fileIter;

			QString filePath = dir.absoluteFilePath(fileName);

			QStandardItem* fileItemPtr = new QStandardItem(fileName);
			fileItemPtr->setEditable(false);
			fileItemPtr->setIcon(m_iconProvider.icon(QFileInfo(filePath)));
			fileItemPtr->setData(filePath, DR_PATH);

			if (parentItemPtr != NULL)
				parentItemPtr->appendRow(fileItemPtr);
			else
				m_itemModel.appendRow(fileItemPtr);
	}

	return true;
}


bool CFileTreeProviderComp::CreateDirectoryList(
	const QDir& root,
	const QStringList& nameFilters,
	QDir::SortFlags sortSpec,
	QStandardItem* parentItemPtr)
{
	QString rootPath = root.absolutePath();

	if (rootPath.isEmpty()){
		return false;
	}

	QFileInfo fileInfo(rootPath);
	if (!fileInfo.isDir()){
		return false;
	}

	EnumerateDirectory(root, nameFilters, sortSpec, parentItemPtr);

	CreateFileList(root, nameFilters, sortSpec, parentItemPtr);

	return true;
}


// protected methods

void CFileTreeProviderComp::EnumerateDirectory(
	const QDir& root,
	const QStringList& nameFilters,
	QDir::SortFlags sortSpec,
	QStandardItem* parentItemPtr)
{
	QString rootPath = root.absolutePath();

	QStringList entries = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	for (		QStringList::const_iterator iter = entries.begin();
		iter != entries.end();
		++iter){
			const QString& subDirName = *iter;

			QDir subDir = root;
			subDir.setPath(root.absoluteFilePath(subDirName));

			QStandardItem* dirItemPtr = new QStandardItem(subDirName);
			dirItemPtr->setEditable(false);
			dirItemPtr->setIcon(m_iconProvider.icon(QFileIconProvider::Folder));
			dirItemPtr->setData(subDir.absolutePath(), DR_PATH);

			if (parentItemPtr != NULL)
				parentItemPtr->appendRow(dirItemPtr);
			else{
				m_itemModel.appendRow(dirItemPtr);
			}

			EnumerateDirectory(subDir, nameFilters, sortSpec, dirItemPtr);

			CreateFileList(subDir, nameFilters, sortSpec, dirItemPtr);
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CFileTreeProviderComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_itemModel.clear();

	m_itemModel.setColumnCount(1);
	m_itemModel.setHorizontalHeaderItem(0, new QStandardItem(tr("File")));

	if (m_dirParamCompPtr.IsValid()){
		QStringList filters;

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

		CreateDirectoryList(m_dirParamCompPtr->GetPath(),
			filters,
			QDir::Name | QDir::IgnoreCase,
			NULL);
	}
}


// pseudo-reimplemented (istd::IChangeable)

void CFileTreeProviderComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	OnUpdate(changeFlags, changeParamsPtr);
}


// reimplemented (icomp::CComponentBase)

void CFileTreeProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_dirParamModelCompPtr.IsValid()){
		m_dirParamModelCompPtr->AttachObserver(this);
	}
	else{
		OnUpdate(0, NULL);
	}
}


void CFileTreeProviderComp::OnComponentDestroyed()
{
	imod::CSingleModelObserverBase::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace ifile


