#include "ifile/CFileListProviderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtGui/QFileIconProvider>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "ifile/CFileList.h"


namespace ifile
{


// reimplemented (ifile::IFileListProvider)

QStringList CFileListProviderComp::GetFileList() const
{
	return m_fileList;
}


// reimplemented (ibase::IQtItemModelProvider)

const QAbstractItemModel* CFileListProviderComp::GetItemModel() const
{
	return &m_itemModel;
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CFileListProviderComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	istd::CChangeNotifier notifier(this);

	m_fileList.clear();
	m_itemModel.clear();

	m_itemModel.setColumnCount(2);
	m_itemModel.setHorizontalHeaderItem(0, new QStandardItem(tr("File")));
	m_itemModel.setHorizontalHeaderItem(1, new QStandardItem(tr("Modification time")));

	if (m_dirParamCompPtr.IsValid()){
		QStringList filters;

		m_directoryWatcher.removePaths(m_directoryWatcher.directories());
		m_directoryWatcher.addPath(m_dirParamCompPtr->GetPath());

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

		CFileList fileList;
		fileList.Create(
					m_dirParamCompPtr->GetPath(),
					*m_minRecurDepthAttrPtr,
					*m_maxRecurDepthAttrPtr,
					filters);

		m_fileList = fileList;

		m_itemModel.setRowCount(fileList.count());
		QFileIconProvider fileIconProvider;
		for (int fileIndex = 0; fileIndex < fileList.count(); fileIndex++){
			QFileInfo fileInfo(fileList[fileIndex]);
			QIcon fileIcon = fileIconProvider.icon(fileInfo);
		
			QStandardItem* fileItemPtr = new QStandardItem(fileList[fileIndex]);
			fileItemPtr->setEditable(false);
			fileItemPtr->setIcon(fileIcon);

			QStandardItem* fileTimeItemPtr = new QStandardItem(fileInfo.lastModified().toString());
			fileTimeItemPtr->setEditable(false);

			m_itemModel.setItem(fileIndex, 0, fileItemPtr);
			m_itemModel.setItem(fileIndex, 1, fileTimeItemPtr);
		}
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


