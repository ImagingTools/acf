#include <iqtgui/CImagePropertyGuiComp.h>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImagePropertyGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	PropertyTree->clear();

	iimg::IBitmap* bitmapPtr = GetObservedObject();
	if (bitmapPtr != NULL && IsGuiCreated()){
		QTreeWidgetItem* widthItemPtr = new QTreeWidgetItem();
		widthItemPtr->setText(0, tr("Width"));
		widthItemPtr->setText(1, QString("%1").arg(bitmapPtr->GetImageSize().GetX()));

		QTreeWidgetItem* heightItemPtr = new QTreeWidgetItem();
		heightItemPtr->setText(0, tr("Height"));
		heightItemPtr->setText(1, QString("%1").arg(bitmapPtr->GetImageSize().GetY()));

		QTreeWidgetItem* depthItemPtr = new QTreeWidgetItem();
		depthItemPtr->setText(0, tr("Depth"));
		depthItemPtr->setText(1, QString("%1 bits").arg(bitmapPtr->GetComponentBitsCount() * bitmapPtr->GetComponentsCount()));

		PropertyTree->addTopLevelItem(widthItemPtr);
		PropertyTree->addTopLevelItem(heightItemPtr);
		PropertyTree->addTopLevelItem(depthItemPtr);
	}
}


} // namespace iqtgui


