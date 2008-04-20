#include "idoc/CSingleDocumentTemplateComp.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

bool CSingleDocumentTemplateComp::LoadDocumentFromFile(const istd::CString& filePath, imod::IModel& result) const
{
	istd::IChangeable* changeableDocPtr = dynamic_cast<istd::IChangeable*>(&result);

	if (m_fileLoaderCompPtr.IsValid() && (changeableDocPtr != NULL)){
		int state = m_fileLoaderCompPtr->LoadFromFile(*changeableDocPtr, filePath);
		return (state == iser::IFileLoader::StateOk);
	}

	return false;
}


bool CSingleDocumentTemplateComp::SaveDocumentToFile(const imod::IModel& document, const istd::CString& filePath) const
{
	const istd::IChangeable* changeableDocPtr = dynamic_cast<const istd::IChangeable*>(&document);

	if (m_fileLoaderCompPtr.IsValid() && (changeableDocPtr != NULL)){
		int state = m_fileLoaderCompPtr->SaveToFile(*changeableDocPtr, filePath);
		return (state == iser::IFileLoader::StateOk);
	}

	return false;
}


imod::IModel* CSingleDocumentTemplateComp::CreateDocument(const std::string& documentTypeId) const
{
	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateComp::CreateView(imod::IModel* documentPtr, const std::string& viewTypeId) const
{
	I_ASSERT(documentPtr != NULL);

	if (m_viewCompFact.IsValid() && IsViewTypeSupported(viewTypeId)){
		istd::TDelPtr<imod::IObserver> viewPtr(m_viewCompFact.CreateInstance());
		if (viewPtr.IsValid() && documentPtr->AttachObserver(viewPtr.GetPtr())){
			return viewPtr.PopPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::CComponentBase)

void CSingleDocumentTemplateComp::OnComponentCreated()
{
	I_ASSERT(m_documentTypeIdAttrPtr.IsValid());
	SetDocumentTypeId((*m_documentTypeIdAttrPtr).ToString());

	istd::CStringList fileFilters;
	if (m_fileFiltersAttrPtr.IsValid()){
		for (int index = 0; index < m_fileFiltersAttrPtr.GetCount(); index++){
			fileFilters.push_back(m_fileFiltersAttrPtr[index]);		
		}
	}
	SetFileFilters(fileFilters);

	I_ASSERT(m_defaultDirectoryAttrPtr.IsValid());
	SetDefaultDirectory(*m_defaultDirectoryAttrPtr);

	istd::CStringList fileExtensions;
	if (m_fileExtensionsAttrPtr.IsValid()){
		for (int index = 0; index < m_fileExtensionsAttrPtr.GetCount(); index++){
			fileExtensions.push_back(m_fileExtensionsAttrPtr[index]);		
		}
	}
	SetFileExtensions(fileExtensions);
}


} // namespace idoc

