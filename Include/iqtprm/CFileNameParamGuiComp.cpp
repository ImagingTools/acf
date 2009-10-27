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

	BaseClass::OnGuiCreated();
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
		QString filePath = iqt::GetQString(objectPtr->GetPath());
		
		SetPathToEditor(filePath);

		int pathType = objectPtr->GetPathType();

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
		switch(objectPtr->GetPathType()){
			case iprm::IFileNameParam::PT_DIRECTORY:
				m_directoryModel.setFilter(QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);
				break;
			case iprm::IFileNameParam::PT_FILE:
				m_directoryModel.setFilter(QDir::Files);
				break;
		}

		iqt::CSignalBlocker block(DirEdit);

		if (objectPtr->GetPathType() != iprm::IFileNameParam::PT_URL){
			DirEdit->setModel(&m_directoryModel);
		}

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


void CFileNameParamGuiComp::on_DirEdit_currentIndexChanged(const QString&/* text*/)
{
	QString filePath = m_directoryModel.filePath(DirEdit->view()->currentIndex());

	OnPathEdited(filePath);
}


// private methods

void CFileNameParamGuiComp::SetPathToEditor(const QString& path) const
{
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

	iqt::CSignalBlocker blocker(DirEdit, true);

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

			DirEdit->setRootModelIndex(m_directoryModel.index(directory));
		}
	}
}


void CFileNameParamGuiComp::OnPathEdited(const QString& path) const
{
	SetPathToEditor(path);

	UpdateModel();
}


} // namespace iqtprm


