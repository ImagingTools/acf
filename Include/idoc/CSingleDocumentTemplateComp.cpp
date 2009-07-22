#include "idoc/CSingleDocumentTemplateComp.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

iser::IFileLoader* CSingleDocumentTemplateComp::GetFileLoader(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return m_fileLoaderCompPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CSingleDocumentTemplateComp::CreateDocument(const std::string& documentTypeId) const
{
	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateComp::CreateView(
			const std::string& documentTypeId,
			istd::IChangeable* documentPtr,
			const std::string& viewTypeId) const
{
	I_ASSERT(documentPtr != NULL);

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(documentPtr);

	if (		(modelPtr != NULL) &&
				m_viewCompFact.IsValid() &&
				IsDocumentTypeSupported(documentTypeId) &&
				IsViewTypeSupported(viewTypeId)){
		istd::TDelPtr<icomp::IComponent> componentPtr(m_viewCompFact.CreateComponent());

		imod::IObserver* observerPtr = m_viewCompFact.ExtractInterface(componentPtr.GetPtr());
		istd::IPolymorphic* viewPtr = ExtractViewInterface(componentPtr.GetPtr());

		if (		(viewPtr != NULL) &&
					(observerPtr != NULL) &&
					modelPtr->AttachObserver(observerPtr)){
			componentPtr.PopPtr();

			return viewPtr;
		}
	}

	return NULL;
}


// reimplemented (icomp::CComponentBase)

void CSingleDocumentTemplateComp::OnComponentCreated()
{
	I_ASSERT(m_documentTypeIdAttrPtr.IsValid());
	SetDocumentTypeId((*m_documentTypeIdAttrPtr).ToString());

	I_ASSERT(m_defaultDirectoryAttrPtr.IsValid());
	SetDefaultDirectory(*m_defaultDirectoryAttrPtr);

	int featureFlags = 0;
	if (m_isNewSupportedAttrPtr.IsValid() && m_isNewSupportedAttrPtr->GetValue()){
		featureFlags |= idoc::IDocumentTemplate::New;
	}

	if (m_isEditSupportedAttrPtr.IsValid() && m_isEditSupportedAttrPtr->GetValue()){
		featureFlags |= idoc::IDocumentTemplate::Edit;
	}

	SetSupportedFeatures(featureFlags);
}


// protected methods

istd::IPolymorphic* CSingleDocumentTemplateComp::ExtractViewInterface(icomp::IComponent* componentPtr) const
{
	return m_viewCompFact.ExtractInterface(componentPtr);
}


} // namespace idoc

