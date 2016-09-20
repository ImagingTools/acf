#include <iview/CShapeInfoGuiComp.h>


namespace iview
{


// protected methods

void CShapeInfoGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	IShapeStatusInfo* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
//		i2d::CVector2d logicalPosition = objectPtr->GetLogicalPosition();
//		LogicalPosition->setText(tr("%1, %2").arg(logicalPosition.GetX(), 2, 'f', 2).arg(logicalPosition.GetY(), 2, 'f', 2));

//		i2d::CVector2d pixelPosition = objectPtr->GetPixelPosition();
//		PixelPosition->setText(tr("%1, %2").arg(int(pixelPosition.GetX())).arg(int(pixelPosition.GetY())));

		QString infoText = objectPtr->GetInfoText();
		InfoText->setText(infoText);
	}
}


} // namespace iview


