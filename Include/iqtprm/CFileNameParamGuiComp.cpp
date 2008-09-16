#include "iqtprm/CFileNameParamGuiComp.h"


// Qt includes
#include <QFileDialog>

#include "istd/TChangeNotifier.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CFileNameParamGuiComp::UpdateModel() const
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::CString directory = iqt::GetCString(DirEdit->text());
		if (directory != objectPtr->GetDirectory()){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetDirectory(directory);
		}
	}
}


void CFileNameParamGuiComp::UpdateEditor()
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		DirEdit->setText(iqt::GetQString(objectPtr->GetDirectory()));
	}
}


// protected slots

void CFileNameParamGuiComp::on_BrowseButton_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(GetQtWidget(), tr("Select image directory"), DirEdit->text());
	if (!dir.isEmpty()){
		DirEdit->setText(dir);
	}
}


void CFileNameParamGuiComp::on_DirEdit_textChanged(const QString& /*text*/)
{
	UpdateModel();
}


} // namespace iqtprm


