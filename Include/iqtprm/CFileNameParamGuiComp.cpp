#include "iqtprm/CFileNameParamGuiComp.h"


// Qt includes
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


// reimplemented (iqtgui::CGuiComponentBase)

void CFileNameParamGuiComp::OnGuiCreated()
{
	DirectoryLabel->setVisible(false);
	PathLabel->setVisible(false);
	UrlLabel->setVisible(false);
	BrowseButton->setVisible(false);

	if (m_pathLabelAttrPtr.IsValid()){
		DirectoryLabel->setText(iqt::GetQString(*m_pathLabelAttrPtr));
		PathLabel->setText(iqt::GetQString(*m_pathLabelAttrPtr));
		UrlLabel->setText(iqt::GetQString(*m_pathLabelAttrPtr));
	}

	m_fileIconPtr.SetPtr(new QLabel(DirEdit->lineEdit()));
	m_fileIconPtr->move(1, 0);
	m_fileIconPtr->hide();

	BaseClass::OnGuiCreated();
}


void CFileNameParamGuiComp::OnGuiDestroyed()
{
	m_fileIconPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (imod::IModelEditor)

void CFileNameParamGuiComp::UpdateModel() const
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CFileNameParamGuiComp*>(this));

		istd::CString currentPath = iqt::GetCString(DirEdit->currentText());
		if (currentPath != objectPtr->GetPath()){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetPath(currentPath);
		}
	}
}


void CFileNameParamGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		QString newPath = iqt::GetQString(objectPtr->GetPath());
		if (newPath != DirEdit->currentText()){
			SetPathToEditor(newPath);
		}

		DirectoryLabel->setVisible(pathType == iprm::IFileNameParam::PT_DIRECTORY);
		PathLabel->setVisible(pathType == iprm::IFileNameParam::PT_FILE);
		UrlLabel->setVisible(pathType == iprm::IFileNameParam::PT_URL);
		BrowseButton->setVisible((pathType == iprm::IFileNameParam::PT_DIRECTORY) || (pathType == iprm::IFileNameParam::PT_FILE));
	}
	else{
		DirectoryLabel->setVisible(false);
		PathLabel->setVisible(false);
		UrlLabel->setVisible(false);
		BrowseButton->setVisible(false);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CFileNameParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		SetPathToEditor(iqt::GetQString(objectPtr->GetPath()));
	}
}


// protected slots

void CFileNameParamGuiComp::on_BrowseButton_clicked()
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		if (pathType == iprm::IFileNameParam::PT_DIRECTORY){
			QString filePath = QFileDialog::getExistingDirectory(GetQtWidget(), tr("Select directory"), DirEdit->currentText());
			if (!filePath.isEmpty()){
				OnPathEdited(filePath);
			}
		}
		else if (pathType == iprm::IFileNameParam::PT_FILE){
			QString filePath = QFileDialog::getOpenFileName(GetQtWidget(), tr("Select file"), QFileInfo(DirEdit->currentText()).dir().absolutePath());
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


// private methods

void CFileNameParamGuiComp::SetPathToEditor(const QString& path) const
{
	I_ASSERT(DirEdit->isEditable());

	iqt::CSignalBlocker blocker(DirEdit, true);

	DirEdit->clear();
			
	QString normalizedPath = QDir::toNativeSeparators(path);

	MakeSelectionHint(normalizedPath);

	static QPalette defaultPalette = qApp->palette();

	QFileInfo fileInfo(path);
	if (!fileInfo.exists() && !path.isEmpty()){
		QPalette invalidPalette = defaultPalette;
		invalidPalette.setColor(QPalette::Text, Qt::red);
	
		DirEdit->lineEdit()->setPalette(invalidPalette);
	}
	else{
		DirEdit->lineEdit()->setPalette(defaultPalette);
	}

	QIcon fileIcon = GetFileIcon(normalizedPath);
	if (!fileIcon.isNull()){
		int lineEditHeight = DirEdit->lineEdit()->height();
		m_fileIconPtr->setPixmap(fileIcon.pixmap(lineEditHeight - 2, lineEditHeight - 2));
		DirEdit->lineEdit()->setTextMargins(lineEditHeight, 0, 0, 0);
		m_fileIconPtr->show();
	}
	else{
		m_fileIconPtr->hide();
		DirEdit->lineEdit()->setTextMargins(0, 0, 0, 0);
	}
	
	DirEdit->setEditText(normalizedPath);
}


void CFileNameParamGuiComp::MakeSelectionHint(const QString& text) const
{
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

			for(int dirIndex = 0; dirIndex < subDirs.count(); dirIndex++){
				QString filePath = subDirs[dirIndex].absoluteFilePath();

				DirEdit->addItem(GetFileIcon(filePath), QDir::toNativeSeparators(filePath));
			}
		}
	}
}


QIcon CFileNameParamGuiComp::GetFileIcon(const QString& filePath) const
{
	QIcon fileIcon;

	QModelIndex index = m_directoryModel.index(filePath);
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


} // namespace iqtprm


