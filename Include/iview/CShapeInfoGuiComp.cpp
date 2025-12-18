#include <iview/CShapeInfoGuiComp.h>


namespace iview
{


// protected methods

void CShapeInfoGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	IShapeStatusInfo* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString infoText = objectPtr->GetInfoText();

		InfoText->setText(infoText);
	}
}


} // namespace iview


