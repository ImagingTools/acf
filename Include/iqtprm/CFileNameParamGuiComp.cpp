#include "iqtprm/CFileNameParamGuiComp.h"


// Qt includes
#include <QtGui/QFileDialog>
#include <QtCore/QFileInfo>
#include <QtGui/QLineEdit>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"
#include "iqt/CSystem.h"

#include "iqtgui/CExtLineEdit.h"
#include "iqtgui/CFileDialogLoaderComp.h"


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CFileNameParamGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

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

	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		SetPathToEditor(objectPtr->GetPath());
	}
}


void CFileNameParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		QString newPath = objectPtr->GetPath();
		if (newPath != GetPathFromEditor()){
			SetPathToEditor(newPath);
		}

		if (IsLabelNeeded()){
			DirectoryLabel->setVisible(pathType == iprm::IFileNameParam::PT_DIRECTORY);
			PathLabel->setVisible(pathType == iprm::IFileNameParam::PT_FILE);
			UrlLabel->setVisible(pathType == iprm::IFileNameParam::PT_URL);
		}

		BrowseButton->setVisible((pathType == iprm::IFileNameParam::PT_DIRECTORY) || (pathType == iprm::IFileNameParam::PT_FILE));

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
	BrowseButton->setVisible(false);

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

	iqtgui::CExtLineEdit* lineEdit = new iqtgui::CExtLineEdit(startHint, 2, DirEdit);

	// add "cd up" button:
	if (!*m_readOnlyAttrPtr){
		QToolButton* cdUpButton = new QToolButton(lineEdit);
		cdUpButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		cdUpButton->setIcon(QIcon(":/Icons/UpDir.svg"));
		connect(cdUpButton, SIGNAL(clicked()), this, SLOT(OnDirectoryUp()));

		lineEdit->AddWidget(cdUpButton, Qt::AlignRight);
	}

	lineEdit->setReadOnly(*m_readOnlyAttrPtr);
	BrowseButton->setHidden(*m_readOnlyAttrPtr);

	DirEdit->setLineEdit(lineEdit);
	DirEdit->setCompleter(NULL);

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
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		if (pathType == iprm::IFileNameParam::PT_DIRECTORY){
			QString filePath = QFileDialog::getExistingDirectory(GetQtWidget(), tr("Select directory"), GetPathFromEditor());
			if (!filePath.isEmpty()){
				OnPathEdited(filePath);
			}
		}
		else if (pathType == iprm::IFileNameParam::PT_FILE){
			QStringList filterList;

			if (m_fileTypeInfoPtr.IsValid()){
				QString allExt;
				iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(
							*m_fileTypeInfoPtr,
							0,
							allExt,
							filterList);

				if (filterList.size() > 1){
					filterList += tr("All supported files (%1)").arg(allExt);
				}
			}

			QString filePath = QFileDialog::getOpenFileName(
						GetQtWidget(),
						tr("Select file"),
						QFileInfo(GetPathFromEditor()).dir().absolutePath(),
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
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	I_ASSERT(DirEdit->isEditable());
	
	iqt::CSignalBlocker blocker(DirEdit);

	int currsorPosition = DirEdit->lineEdit()->cursorPosition();

	QString normalizedPath;
	iqtgui::CExtLineEdit* lineEdit = dynamic_cast<iqtgui::CExtLineEdit*>(DirEdit->lineEdit());
	I_ASSERT(lineEdit != NULL);

	int pathType = objectPtr->GetPathType();
	if (pathType == iprm::IFileNameParam::PT_URL){
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
	lineEdit->setCursorPosition(currsorPosition);
}


void CFileNameParamGuiComp::MakeSelectionHint(const QString& text) const
{
	if (*m_readOnlyAttrPtr){
		return;
	}

	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		if (objectPtr->GetPathType() == iprm::IFileNameParam::PT_URL){
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

			if (objectPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY){
				filters = QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot;
			}

			if (objectPtr->GetPathType() == iprm::IFileNameParam::PT_FILE){
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

	QIcon fileIcon = QIcon(":/Icons/Warning.svg");

	QModelIndex index = m_directoryModel.index(iqt::CSystem::GetEnrolledPath(filePath));
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
	iqtgui::CExtLineEdit* lineEdit = dynamic_cast<iqtgui::CExtLineEdit*>(DirEdit->lineEdit());
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


