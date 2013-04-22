#include "iloggui/CTextLogGuiComp.h"


namespace iloggui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CTextLogGuiComp::UpdateGui(int /*updateFlags*/)
{
	QString document;

	ilog::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const ilog::IMessageContainer::Messages messages = objectPtr->GetMessages();
		for (int i = 0; i < messages.count(); i++){
			const ilog::IMessageConsumer::MessagePtr messagePtr = messages.at(i);

			QString entry = QString("[%1] <%2> %3 %4")
				.arg(messagePtr->GetInformationTimeStamp().toString())
				.arg(messagePtr->GetInformationSource())
				.arg(messagePtr->GetInformationCategory())
				.arg(messagePtr->GetInformationDescription());

			document += entry;
			document += "\n";
		}
	}	

	LogEditor->setPlainText(document);
}


// reimplemented (CGuiComponentBase)

void CTextLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CTextLogGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


} // namespace iloggui


