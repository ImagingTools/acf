#include "icmpstr/CRegistryPropEditorComp.h"


// Qt includes
#include "istd/TChangeNotifier.h"


// public methods
	
namespace icmpstr
{


// reimplmented (imod::IModelEditor)

void CRegistryPropEditorComp::UpdateEditor(int /*updateFlags*/)
{
	if (!IsGuiCreated()){
		return;
	}

	const icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	DescriptionEdit->setText(iqt::GetQString(registryPtr->GetDescription()));
	KeywordsEdit->setText(iqt::GetQString(registryPtr->GetKeywords()));
}


void CRegistryPropEditorComp::UpdateModel() const
{
}


// protected slots:

void CRegistryPropEditorComp::on_DescriptionEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString description = iqt::GetCString(DescriptionEdit->text());

		if (description != registryPtr->GetDescription()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetDescription(description);
		}
	}
}


void CRegistryPropEditorComp::on_KeywordsEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString keywords = iqt::GetCString(KeywordsEdit->text());

		if (keywords != registryPtr->GetKeywords()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetKeywords(keywords);
		}
	}
}


} // namespace icmpstr


