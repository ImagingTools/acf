#include <ifilegui/CFileSystemExplorerGuiComp.h>


// Qt includes
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#else
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>


namespace ifilegui
{


CFileSystemExplorerGuiComp::CFileSystemExplorerGuiComp()
:	m_filterEditPtr(NULL)
{
}


// protected methods

QStringList CFileSystemExplorerGuiComp::GetDefaultFilters() const
{
	QStringList retVal;

	if (m_filterInfoCompPtr.IsValid()){
		QStringList extensions;
		if (m_filterInfoCompPtr->GetFileExtensions(extensions, NULL, ifile::IFileTypeInfo::QF_LOAD | ifile::IFileTypeInfo::QF_FILE)){
			for (		QStringList::const_iterator iter = extensions.begin();
						iter != extensions.end();
						++iter){
				retVal += "*." + *iter;
			}
		}
	}

	return retVal;
}


void CFileSystemExplorerGuiComp::InvalidateFileSystemModel(const QString& currentFilePath)
{
	QString currentPath = QFileInfo(currentFilePath).absolutePath();

	QString saveRootPath = m_fileSystemModel.rootPath();

	m_fileSystemModel.setRootPath(currentPath);

	m_fileSystemModel.setRootPath(saveRootPath);
}


void CFileSystemExplorerGuiComp::UpdateFileRoot()
{
	if (IsGuiCreated()){
		QString currentRootFilePath;

		QModelIndex rootIndex;

		if (m_rootPathParamCompPtr.IsValid()){
			currentRootFilePath = istd::CSystem::GetEnrolledPath(m_rootPathParamCompPtr->GetPath());
		}

		if (!currentRootFilePath.isEmpty() && QFile::exists(currentRootFilePath)){
			rootIndex = m_fileSystemModel.setRootPath(currentRootFilePath);
		}
		else {
			rootIndex = m_fileSystemModel.setRootPath(m_fileSystemModel.myComputer().toString());
		}

		FileTree->setRootIndex(rootIndex);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CFileSystemExplorerGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString currentFilePath = istd::CSystem::GetEnrolledPath(objectPtr->GetPath());

		QDir::Filters filters = objectPtr->GetPathType() == ifile::IFileNameParam::PT_DIRECTORY?
					(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Drives | QDir::AllDirs):
					(QDir::Drives | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);

		if (m_fileSystemModel.filter() != filters){
			m_fileSystemModel.setFilter(filters);
		}

		InvalidateFileSystemModel(currentFilePath);

		QModelIndex index = m_fileSystemModel.index(currentFilePath);
		if (index.isValid()){
			QItemSelectionModel* selectionModelPtr = FileTree->selectionModel();
			if (selectionModelPtr != NULL){
				UpdateBlocker blocker(this);

				selectionModelPtr->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

				FileTree->scrollTo(index);
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFileSystemExplorerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (!m_useSystemDecoratedIconsAttrPtr.IsValid() || !*m_useSystemDecoratedIconsAttrPtr){
		m_fileSystemModel.setIconProvider(&m_fileIconProvider);
	}

	FileTree->header()->hide();

	FileTree->setSelectionMode(QAbstractItemView::SingleSelection);

	FileTree->setModel(&m_fileSystemModel);

	UpdateFileRoot();

	QItemSelectionModel* selectionModelPtr = FileTree->selectionModel();
	if (selectionModelPtr != NULL){
		connect(selectionModelPtr,
					SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
					this,
					SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
	}

	QStringList fileFilters = GetDefaultFilters();

	m_fileSystemModel.setNameFilters(fileFilters);
	m_fileSystemModel.setNameFilterDisables(false);

	FilterFrame->setVisible(*m_showUserFilterAttrPtr);
	QLayout* filterLayoutPtr = FilterFrame->layout();
	if (filterLayoutPtr != NULL){
		m_filterEditPtr = new iwidgets::CExtLineEdit(tr("Enter text to filter file items"), 2, FilterFrame);

		// add "clear" button:
		QToolButton* clearButton = new QToolButton(m_filterEditPtr);
		clearButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		clearButton->setIcon(QIcon(":/Icons/Clear"));
		connect(clearButton, SIGNAL(clicked()), m_filterEditPtr, SLOT(clear()));

		m_filterEditPtr->AddWidget(clearButton, Qt::AlignRight);

		filterLayoutPtr->addWidget(m_filterEditPtr);

		connect(m_filterEditPtr, SIGNAL(editingFinished()), this, SLOT(OnFilterChanged()));
	}

	FileTree->setDragDropMode(QTreeView::DragDrop);
	FileTree->setDragEnabled(true);

	QHeaderView* headerPtr = FileTree->header();
	if (headerPtr != NULL){
		if (*m_showFileTypeAttrPtr){
			headerPtr->setSectionHidden(2, false);
		}
		else{
			headerPtr->setSectionHidden(2, true);
		}

		headerPtr->setSectionHidden(1, true);

		if (*m_showFileModificationTimeAttrPtr){
			headerPtr->setSectionHidden(3, false);
		}
		else{
			headerPtr->setSectionHidden(3, true);
		}
	}

	if (m_allowOpenFileAttrPtr.IsValid() && *m_allowOpenFileAttrPtr){
		connect(FileTree, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDoubleClicked(const QModelIndex&)));
	}

	if (m_rootPathParamModelCompPtr.IsValid()){
		RegisterModel(m_rootPathParamModelCompPtr.GetPtr(), 0);
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CFileSystemExplorerGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	UpdateFileRoot();
}


// private slots

void CFileSystemExplorerGuiComp::OnFilterChanged()
{
	QString filterText = m_filterEditPtr->GetText();

	QStringList fileFilters = GetDefaultFilters();

	if (!filterText.isEmpty()){
		fileFilters << filterText;
	}

	m_fileSystemModel.setNameFilters(fileFilters);
}


void CFileSystemExplorerGuiComp::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&/* deselected*/)
{
	if (IsUpdateBlocked() ){
		return;
	}

	UpdateBlocker blocker(this);

	if (!selected.indexes().isEmpty()){
		ifile::IFileNameParam* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			QModelIndex selectedIndex = selected.indexes().at(0);

			QString currentFilePath = m_fileSystemModel.filePath(selectedIndex);
			QFileInfo fileInfo = m_fileSystemModel.fileInfo(selectedIndex);

			bool isFile = fileInfo.isFile();
			bool isDir = fileInfo.isDir();

			int selectedFileType = ifile::IFileNameParam::PT_UNKNOWN;

			if (isFile){
				selectedFileType = ifile::IFileNameParam::PT_FILE;
			}

			if (isDir){
				selectedFileType = ifile::IFileNameParam::PT_DIRECTORY;
			}

			int supportedPathType = objectPtr->GetPathType();
			if ((supportedPathType == ifile::IFileNameParam::PT_UNKNOWN) || (supportedPathType == selectedFileType)){
				objectPtr->SetPath(currentFilePath);
			}
			else{
				objectPtr->SetPath(QString());
			}
		}
	}
}


void CFileSystemExplorerGuiComp::OnDoubleClicked(const QModelIndex& modelIndex)
{
	QFileInfo fileInfo = m_fileSystemModel.fileInfo(modelIndex);
	if (fileInfo.isDir()){
		return;
	}

	QDesktopServices::openUrl(fileInfo.canonicalFilePath());
}


// public methods of the embedded class FileIconProvider

// reimplemented (QFileIconProvider)

QIcon CFileSystemExplorerGuiComp::FileIconProvider::icon(const QFileInfo& fileInfo) const
{
	if (fileInfo.isDir()){
		return BaseClass::icon(Folder);
	}

	return BaseClass::icon(fileInfo);
}


} // namespace ifilegui


