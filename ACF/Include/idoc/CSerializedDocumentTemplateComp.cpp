#include "idoc/CSerializedDocumentTemplateComp.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

bool CSerializedDocumentTemplateComp::LoadDocumentFromFile(const istd::CString& filePath, imod::IModel& result) const
{
	iser::ISerializable* serializableDocPtr = dynamic_cast<iser::ISerializable*>(&result);

	if (m_fileSerializerCompPtr.IsValid() && (serializableDocPtr != NULL)){
		int state = m_fileSerializerCompPtr->LoadFromFile(*serializableDocPtr, filePath);
		return (state == iser::IFileSerializer::StateOk);
	}

	return false;
}


bool CSerializedDocumentTemplateComp::SaveDocumentToFile(const imod::IModel& document, const istd::CString& filePath) const
{
	const iser::ISerializable* serializableDocPtr = dynamic_cast<const iser::ISerializable*>(&document);

	if (m_fileSerializerCompPtr.IsValid() && (serializableDocPtr != NULL)){
		int state = m_fileSerializerCompPtr->SaveToFile(*serializableDocPtr, filePath);
		return (state == iser::IFileSerializer::StateOk);
	}

	return false;
}


imod::IModel* CSerializedDocumentTemplateComp::CreateDocument(const std::string& documentTypeId) const
{
	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSerializedDocumentTemplateComp::CreateView(imod::IModel* documentPtr, const std::string& viewTypeId) const
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

void CSerializedDocumentTemplateComp::OnComponentCreated()
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


