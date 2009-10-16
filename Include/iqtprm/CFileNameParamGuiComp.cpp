#include "iqtprm/CFileNameParamGuiComp.h"


// Qt includes
#include <QFileDialog>
#include <QFileInfo>

#include "istd/TChangeNotifier.h"


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
	if (objectPtr != NULL){
		istd::CString directory = iqt::GetCString(DirEdit->text());
		if (directory != objectPtr->GetPath()){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetPath(directory);
		}
	}
}


void CFileNameParamGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		DirEdit->setText(iqt::GetQString(objectPtr->GetPath()));
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


// protected slots

void CFileNameParamGuiComp::on_BrowseButton_clicked()
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int pathType = objectPtr->GetPathType();

		if (pathType == iprm::IFileNameParam::PT_DIRECTORY){
			QString dir = QFileDialog::getExistingDirectory(GetQtWidget(), tr("Select directory"), DirEdit->text());
			if (!dir.isEmpty()){
				DirEdit->setText(dir);
			}
		}
		else if (pathType == iprm::IFileNameParam::PT_FILE){
			QString dir = QFileDialog::getOpenFileName(GetQtWidget(), tr("Select file"), QFileInfo(DirEdit->text()).dir().absolutePath());
			if (!dir.isEmpty()){
				DirEdit->setText(dir);
			}
		}
	}
}


void CFileNameParamGuiComp::on_DirEdit_textChanged(const QString& /*text*/)
{
	UpdateModel();
}


} // namespace iqtprm


