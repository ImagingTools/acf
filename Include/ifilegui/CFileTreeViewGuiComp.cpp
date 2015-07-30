#include "ifilegui/CFileTreeViewGuiComp.h"


#define PERFORMANCE_TEST


namespace ifilegui
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CFileTreeViewGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_filterMatcher.setCaseSensitivity(Qt::CaseInsensitive);
	m_filterMatcher.setPatternSyntax(QRegExp::Wildcard);

	m_fileModelUpdateAllowed = true;

	m_fileTypeInfoCompPtr.EnsureInitialized();

	RegisterModel(&m_currentFile);
}


void CFileTreeViewGuiComp::OnComponentDestroyed()
{
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (CGuiComponentBase)

void CFileTreeViewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	FileList->setModel(&m_itemModel);

	QItemSelectionModel* selectionModelPtr = FileList->selectionModel();
	if (selectionModelPtr != NULL){
		connect(selectionModelPtr,
			SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
			this,
			SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
	}

	connect(&m_filterTimer, SIGNAL(timeout()), this, SLOT(on_Refresh_clicked()));

	m_internalThreadPtr = new InternalThread(this);
	connect(m_internalThreadPtr, SIGNAL(finished()), this, SLOT(OnTreeModelUpdated()), Qt::QueuedConnection);

	Stack->setCurrentIndex(0);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CFileTreeViewGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	RebuildTreeModel();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CFileTreeViewGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	UpdateCurrentSelection();
}


// private slots

void CFileTreeViewGuiComp::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&/* deselected*/)
{
	UpdateBlocker updateBlocker(this);

	if (!selected.indexes().isEmpty()){
		QModelIndex selectedIndex = selected.indexes().at(0);

		QString currentFilePath = m_itemModel.data(selectedIndex, DR_PATH).toString();
		QFileInfo fileInfo(currentFilePath);

		bool isFile = fileInfo.isFile();
		bool isDir = fileInfo.isDir();

		int selectedFileType = ifile::IFileNameParam::PT_UNKNOWN;

		if (isFile){
			selectedFileType = ifile::IFileNameParam::PT_FILE;
		}

		if (isDir){
			selectedFileType = ifile::IFileNameParam::PT_DIRECTORY;
		}

		int supportedPathType = m_currentFile.GetPathType();
		if ((supportedPathType == ifile::IFileNameParam::PT_UNKNOWN) || (supportedPathType == selectedFileType)){
			m_fileModelUpdateAllowed = false;

			m_currentFile.SetPath(currentFilePath);

			m_fileModelUpdateAllowed = true;

			return;
		}
	}

	m_currentFile.SetPath("");
}


void CFileTreeViewGuiComp::on_Refresh_clicked()
{
	// update tree via QThread
	RebuildTreeModel();
}


void CFileTreeViewGuiComp::UpdateChildItems(QStandardItem* itemPtr)
{
	if (itemPtr == NULL){
		return;
	}

	if (itemPtr->data(DR_ISDIR).toBool()){
		itemPtr->setIcon(m_iconProvider.icon(QFileIconProvider::Folder));
	} else {
		QString filePath = itemPtr->data(DR_PATH).toString();
		const QString& fileExtension = QFileInfo(filePath).suffix();
		if (m_extToIconMap.contains(fileExtension)){
			itemPtr->setIcon(m_extToIconMap[fileExtension]);
		} else {
			QIcon icon(m_iconProvider.icon(filePath));
			m_extToIconMap[fileExtension] = icon;
			itemPtr->setIcon(icon);
		}

	}

	if (itemPtr->hasChildren()){
		int rows = itemPtr->rowCount();
		for (int r = 0; r < rows; r++){
			QStandardItem* childItemPtr = itemPtr->child(r);
			UpdateChildItems(childItemPtr);
		}
	}
}


void CFileTreeViewGuiComp::OnTreeModelUpdated()
{
	if (!IsGuiCreated()){
		return;
	}

	// update item icons etc.
	int rows = m_itemModel.rowCount();
	for (int r = 0; r < rows; r++){
		QStandardItem* itemPtr = m_itemModel.item(r);
		UpdateChildItems(itemPtr);
	}

	QString infoText = QString("Files: %1  Dirs: %2").arg(m_filesCount).arg(m_dirsCount);

#ifdef PERFORMANCE_TEST
	infoText += QString("  Time: %1 ms").arg(m_performanceTimer.nsecsElapsed() / 1000000.0);
#endif

	InfoLabel->setText(infoText);

	FileList->setUpdatesEnabled(true);
	FileList->expandAll();
#if QT_VERSION >= 0x050000
	FileList->header()->setSectionResizeMode(QHeaderView::Stretch);
#else
	FileList->header()->setResizeMode(QHeaderView::Stretch);
#endif

	FilterText->setFocus();

	Stack->widget(0)->setEnabled(true);
	FileList->setEnabled(true);
	Refresh->setEnabled(true);
	Stack->setCurrentIndex(0);

	// update selection
	UpdateCurrentSelection();
}


void CFileTreeViewGuiComp::on_FilterText_textChanged(QString filterText)
{
	m_userFilter = filterText.trimmed();

	m_filterTimer.start(500);
}


// private members

void CFileTreeViewGuiComp::RebuildTreeModel()
{
	m_filterTimer.stop();

#ifdef PERFORMANCE_TEST
	m_performanceTimer.start();
#endif

	Stack->setCurrentIndex(1);
	Stack->widget(0)->setEnabled(false);
	FileList->setEnabled(false);
	Refresh->setEnabled(false);

	qApp->processEvents();

	FileList->setUpdatesEnabled(false);

	m_itemModel.clear();
	m_filesCount = m_dirsCount = 0;

	// this will start DoTreeModelUpdate() in the separate thread.
	m_internalThreadPtr->start();

	// after update is finished, OnTreeModelUpdated() will be invoked.
}


void CFileTreeViewGuiComp::DoTreeModelUpdate()
{
	QMutexLocker lock(&m_lock);

	ifile::IFileNameParam* rootDirPtr = GetObjectPtr();
	if (rootDirPtr == NULL){
		return;
	}

	m_itemModel.setColumnCount(1);
	m_itemModel.setHorizontalHeaderItem(0, new QStandardItem(tr("File")));

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

	m_filterMatcher.setPattern(m_userFilter);

	CreateDirectoryList(rootDirPtr->GetPath(),
		filters,
		QDir::Name | QDir::IgnoreCase | QDir::Reversed,
		NULL);
}


void CFileTreeViewGuiComp::UpdateCurrentSelection()
{
	if (!m_fileModelUpdateAllowed){
		return;
	}

	if (m_lock.tryLock(100)){
		UpdateBlocker updateBlocker(this);

		QString currentPath = m_currentFile.GetPath();

		QModelIndexList indexes = m_itemModel.match(m_itemModel.index(0,0), DR_PATH, currentPath, 1,
			Qt::MatchFixedString | Qt::MatchRecursive | Qt::MatchWrap);
		if (!indexes.isEmpty()){
			QItemSelectionModel* selectionModelPtr = FileList->selectionModel();
			if (selectionModelPtr != NULL){
				selectionModelPtr->setCurrentIndex(indexes.first(), QItemSelectionModel::ClearAndSelect);
			}
		}

		m_lock.unlock();
	}
}


bool CFileTreeViewGuiComp::CreateFileList(
	const QDir& root,
	const QStringList& nameFilters,
	QDir::SortFlags sortSpec,
	QStandardItem* parentItemPtr)
{
	QDir dir(root.absolutePath());
	dir.setFilter(QDir::Files);

	QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files, sortSpec);

	for (QFileInfoList::const_iterator fileIter = files.begin(); fileIter != files.end(); ++fileIter){
		const QFileInfo& fileInfo = *fileIter;
		const QString& fileName = fileInfo.completeBaseName();

		// check if can be filtered
		if (!m_userFilter.isEmpty()){
			if (m_filterMatcher.indexIn(fileName) < 0)
				continue;
		}

		const QString& filePath = fileInfo.absoluteFilePath();

		QStandardItem* fileItemPtr = new QStandardItem(fileName);
		fileItemPtr->setEditable(false);
		fileItemPtr->setData(filePath, DR_PATH);
		fileItemPtr->setData(false, DR_ISDIR);

		if (parentItemPtr != NULL)
			parentItemPtr->appendRow(fileItemPtr);
		else
			m_itemModel.appendRow(fileItemPtr);

		m_filesCount++;
	}

	return true;
}


bool CFileTreeViewGuiComp::CreateDirectoryList(
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


void CFileTreeViewGuiComp::EnumerateDirectory(
	const QDir& root,
	const QStringList& nameFilters,
	QDir::SortFlags sortSpec,
	QStandardItem* parentItemPtr)
{
	QString rootPath = root.absolutePath();

	QStringList entries = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	for (QStringList::const_iterator iter = entries.begin(); iter != entries.end();	++iter){
		const QString& subDirName = *iter;

		QDir subDir = root;
		subDir.setPath(root.absoluteFilePath(subDirName));

		QStandardItem* dirItemPtr = new QStandardItem(subDirName);
		dirItemPtr->setEditable(false);
		dirItemPtr->setData(subDir.absolutePath(), DR_PATH);
		dirItemPtr->setData(true, DR_ISDIR);

		EnumerateDirectory(subDir, nameFilters, sortSpec, dirItemPtr);

		CreateFileList(subDir, nameFilters, sortSpec, dirItemPtr);

		// remove if empty
		if (!dirItemPtr->hasChildren() && *m_noEmptyDirsAttrPtr){
			delete dirItemPtr;

			continue;
		}
		else{
			if (parentItemPtr != NULL)
				parentItemPtr->appendRow(dirItemPtr);
			else{
				m_itemModel.appendRow(dirItemPtr);
			}

			m_dirsCount++;
		}
	}
}


} // namespace ifilegui


