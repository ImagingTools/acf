#include "iqt/CFileAcquisitionParamsGuiComp.h"


// Qt includes
#include <QFileDialog>

#include "istd/TChangeNotifier.h"


namespace iqt
{


// reimplemented (imod::IModelEditor)

void CFileAcquisitionParamsGuiComp::UpdateModel() const
{
	icam::IFileBitmapAcquisitionParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::CString directory = iqt::GetCString(DirEdit->text());
		if (directory != objectPtr->GetDirectory()){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetDirectory(directory);
		}
	}
}


void CFileAcquisitionParamsGuiComp::UpdateEditor()
{
	icam::IFileBitmapAcquisitionParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		DirEdit->setText(iqt::GetQString(objectPtr->GetDirectory()));
	}
}


// protected slots

void CFileAcquisitionParamsGuiComp::on_BrowseButton_clicked()
{
	DirEdit->setText(QFileDialog::getExistingDirectory(GetQtWidget(), tr("Select image directory"), DirEdit->text()));
}


void CFileAcquisitionParamsGuiComp::on_DirEdit_textChanged(const QString& /*text*/)
{
	UpdateModel();
}


} // namespace iqt


