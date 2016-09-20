#include <iqtprm/CFileNameParamGuiComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLineEdit>
#else
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>
#include <iqt/CSignalBlocker.h>
#include <iwidgets/CExtLineEdit.h>
#include <ifilegui/CFileDialogLoaderComp.h>


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CFileNameParamGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	UpdateBlocker updateBlocker(const_cast<CFileNameParamGuiComp*>(this));

	QString currentPath = GetPathFromEditor();
	if (currentPath != objectPtr->GetPath()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetPath(currentPath);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CFileNameParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		SetPathToEditor(objectPtr->GetPath());
	}
}


void CFileNameParamGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		QString newPath = objectPtr->GetPath();
		if (newPath != GetPathFromEditor()){
			SetPathToEditor(newPath);
		}

		if (IsLabelNeeded()){
			DirectoryLabel->setVisible(pathType == ifile::IFileNameParam::PT_DIRECTORY);
			PathLabel->setVisible(pathType == ifile::IFileNameParam::PT_FILE);
			UrlLabel->setVisible(pathType == ifile::IFileNameParam::PT_URL);
		}

		BrowseButton->setVisible(
			*m_showBrowseButtonAttrPtr && 
			((pathType == ifile::IFileNameParam::PT_DIRECTORY) || (pathType == ifile::IFileNameParam::PT_FILE)));

		if (*m_readOnlyAttrPtr){
			BrowseButton->setVisible(false);
		}
	}
	else{
		DirectoryLabel->setVisible(false);
		PathLabel->setVisible(false);
		UrlLabel->setVisible(false);
		BrowseButton->setVisible(false);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFileNameParamGuiComp::OnGuiCreated()
{
	DirectoryLabel->setVisible(false);
	PathLabel->setVisible(false);
	UrlLabel->setVisible(false);
	BrowseButton->setVisible(*m_showBrowseButtonAttrPtr);

	QString startHint = tr("<Enter path>");
	if (m_startHintAttrPtr.IsValid()){
		startHint = *m_startHintAttrPtr;
	}

	if (m_labelWidthAttrPtr.IsValid()){
		int width = *m_labelWidthAttrPtr;
		if (width > 0){
			DirectoryLabel->setFixedWidth(width);
			PathLabel->setFixedWidth(width);
			UrlLabel->setFixedWidth(width);
		}
	}

	iwidgets::CExtLineEdit* lineEdit = new iwidgets::CExtLineEdit(startHint, 2, DirEdit);

	// add "cd up" button:
	if (!*m_readOnlyAttrPtr && *m_showBrowseButtonAttrPtr){
		QToolButton* cdUpButton = new QToolButton(lineEdit);
		cdUpButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		cdUpButton->setIcon(QIcon(":/Icons/UpDir"));
		connect(cdUpButton, SIGNAL(clicked()), this, SLOT(OnDirectoryUp()));

		lineEdit->AddWidget(cdUpButton, Qt::AlignRight);
	}

	lineEdit->setReadOnly(*m_readOnlyAttrPtr);
	BrowseButton->setHidden(*m_readOnlyAttrPtr);

	DirEdit->setLineEdit(lineEdit);
	DirEdit->setCompleter(NULL);

	if (*m_readOnlyAttrPtr || !*m_filesComboAttrPtr){
		DirEdit->setStyleSheet(
			"QComboBox::drop-down {border: none; background-color: transparent;} "
			"QComboBox::down-arrow {image: none;}"
			);
	}

	BaseClass::OnGuiCreated();
}


void CFileNameParamGuiComp::OnGuiRetranslate()
{
	if (m_pathLabelAttrPtr.IsValid()){
		DirectoryLabel->setText(*m_pathLabelAttrPtr);
		PathLabel->setText(*m_pathLabelAttrPtr);
		UrlLabel->setText(*m_pathLabelAttrPtr);
	}
}


// protected slots

void CFileNameParamGuiComp::on_BrowseButton_clicked()
{
	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		if (pathType == ifile::IFileNameParam::PT_DIRECTORY){
			QString pathToSearch = GetPathFromEditor();
			if (!QFile::exists(pathToSearch) && m_defaultDirPtr.IsValid()){
				pathToSearch = m_defaultDirPtr->GetPath();
			}

			QString filePath = QFileDialog::getExistingDirectory(
				GetQtWidget(),
				tr("Select directory"),
				pathToSearch);
			if (!filePath.isEmpty()){
				OnPathEdited(filePath);
			}
		}
		else if (pathType == ifile::IFileNameParam::PT_FILE){
			QStringList filterList;

			if (m_fileTypeInfoPtr.IsValid()){
				QStringList allExt;
				ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(
							*m_fileTypeInfoPtr,
							NULL,
							0,
							allExt,
							filterList);

				if (allExt.size() > 1){
					filterList += tr("All supported files (%1)").arg("*." + allExt.join(" *."));
				}
			}

			QString pathToSearch = GetPathFromEditor();
			if (!pathToSearch.isEmpty()){
				pathToSearch = QFileInfo(pathToSearch).absoluteFilePath();
			}
			if (!QFile::exists(pathToSearch) && m_defaultDirPtr.IsValid()){
				pathToSearch = m_defaultDirPtr->GetPath();
			}

			QString filePath = QFileDialog::getOpenFileName(
						GetQtWidget(),
						tr("Select file"),
						pathToSearch,
						filterList.join("\n"));
			if (!filePath.isEmpty()){
				OnPathEdited(filePath);
			}
		}
	}
}


void CFileNameParamGuiComp::on_DirEdit_editTextChanged(const QString& text)
{
	OnPathEdited(text);
}


void CFileNameParamGuiComp::OnDirectoryUp()
{
	QString currentDirectory = GetPathFromEditor();

	QFileInfo fileInfo(currentDirectory);
	QString directoryPath = currentDirectory;
	if (!fileInfo.isDir()){
		directoryPath = fileInfo.absolutePath();
	}

	QDir directory(directoryPath);

	directory.cdUp();

	QString newPath = directory.absolutePath();

	OnPathEdited(newPath);
}


// private methods

void CFileNameParamGuiComp::SetPathToEditor(const QString& path) const
{
	ifile::IFileNameParam* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	Q_ASSERT(DirEdit->isEditable());

	iqt::CSignalBlocker blocker(DirEdit);

	int cursorPosition = DirEdit->lineEdit()->cursorPosition();

	QString normalizedPath;
	iwidgets::CExtLineEdit* lineEdit = dynamic_cast<iwidgets::CExtLineEdit*>(DirEdit->lineEdit());
	Q_ASSERT(lineEdit != NULL);

	int pathType = objectPtr->GetPathType();
	if (pathType == ifile::IFileNameParam::PT_URL){
		normalizedPath = path;
	}
	else{
		DirEdit->clear();

		normalizedPath = QDir::fromNativeSeparators(path);

		MakeSelectionHint(normalizedPath);

		QIcon fileIcon = GetFileIcon(normalizedPath);

		lineEdit->SetIcon(fileIcon);
	}

	DirEdit->setEditText(normalizedPath);
	lineEdit->setCursorPosition(lineEdit->isReadOnly() ? normalizedPath.length() : cursorPosition);
}


void CFileNameParamGuiComp::MakeSelectionHint(const QString& text) const
{
	if (*m_readOnlyAttrPtr || !*m_filesComboAttrPtr){
		return;
	}

	ifile::IFileNameParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		if (objectPtr->GetPathType() == ifile::IFileNameParam::PT_URL){
			return;
		}

		QString filePath = text;

		istd::TDelPtr<QFileInfo> validFileInfoPtr;

		while(!filePath.isEmpty()){
			validFileInfoPtr.SetPtr(new QFileInfo(filePath));
			if (validFileInfoPtr->exists()){
				break;
			}

			validFileInfoPtr.Reset();
			filePath = filePath.left(filePath.length() - 1);
		}

		if (validFileInfoPtr.IsValid()){
			QString directory;
			if (!validFileInfoPtr->isDir()){
				directory = validFileInfoPtr->absoluteDir().absolutePath();
			}
			else{
				directory = validFileInfoPtr->filePath();
			}

			QDir::Filters filters = QDir::AllEntries;

			if (objectPtr->GetPathType() == ifile::IFileNameParam::PT_DIRECTORY){
				filters = QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot;
			}

			if (objectPtr->GetPathType() == ifile::IFileNameParam::PT_FILE){
				filters = QDir::Files;
			}

			QDir dir(directory);
			QFileInfoList subDirs = dir.entryInfoList(filters);

			for (int dirIndex = 0; dirIndex < subDirs.count(); dirIndex++){
				QString filePath = subDirs[dirIndex].absoluteFilePath();

				DirEdit->addItem(QDir::toNativeSeparators(filePath));
			}
		}
	}
}


QIcon CFileNameParamGuiComp::GetFileIcon(const QString& filePath) const
{
	if (filePath.isEmpty()){
		return QIcon();
	}

	QIcon fileIcon = QIcon(":/Icons/Warning");

	QModelIndex index = m_directoryModel.index(istd::CSystem::GetEnrolledPath(filePath));
	if (index.isValid()){
		fileIcon = m_directoryModel.fileIcon(index);
	}

	return fileIcon;
}


void CFileNameParamGuiComp::OnPathEdited(const QString& path) const
{
	SetPathToEditor(path);

	UpdateModel();
}


QString CFileNameParamGuiComp::GetPathFromEditor() const
{
	iwidgets::CExtLineEdit* lineEdit = dynamic_cast<iwidgets::CExtLineEdit*>(DirEdit->lineEdit());
	if (lineEdit != NULL){
		QString path = DirEdit->currentText();
		if (path == lineEdit->GetStartupText()){
			return QString();
		}

		return path;
	}

	return QString();
}


bool CFileNameParamGuiComp::IsLabelNeeded() const
{
	return (m_pathLabelAttrPtr.IsValid() && !(*m_pathLabelAttrPtr).isEmpty());
}


} // namespace iqtprm


