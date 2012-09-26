#include "iqtgui/CFileListProviderGuiComp.h"


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "i2d/CAffine2d.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CFileListProviderGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	ibase::IFileListProvider* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		FileList->clear();

		QStringList fileList = objectPtr->GetFileList();

		for (int fileIndex = 0; fileIndex < fileList.count(); fileIndex++){
			QTreeWidgetItem* fileItemPtr = new QTreeWidgetItem();

			fileItemPtr->setText(0, fileList[fileIndex]);

			FileList->addTopLevelItem(fileItemPtr);
		}	
	}
}


} // namespace iqtgui


