#include "idoc/CCompositeDocumentTemplateComp.h"


namespace idoc
{


// reimplemented (idoc::IDocumentTemplate)

IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetDocumentTypeIds() const
{
	Ids retVal;

	for (		IdToTemplateMap::const_iterator iter = m_idToTemplateMap.begin();
				iter != m_idToTemplateMap.end();
				++iter){
		const std::string& documentTypeId = iter->first;
		if (!documentTypeId.empty()){
			retVal.push_back(iter->first);
		}
	}

	return retVal;
}


IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetViewTypeIds(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetViewTypeIds(documentTypeId);
	}
	else{
		return Ids();
	}
}


IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetDocumentTypeIdsForFile(const istd::CString& filePath) const
{
	Ids retVal;

	int slavesCount = m_slaveTemplatesCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IDocumentTemplate* slavePtr = m_slaveTemplatesCompPtr[i];
		if (slavePtr != NULL){
			Ids slaveIds = slavePtr->GetDocumentTypeIdsForFile(filePath);

			retVal.insert(retVal.end(), slaveIds.begin(), slaveIds.end());
		}
	}

	return retVal;
}


iser::IFileLoader* CCompositeDocumentTemplateComp::GetFileLoader(const std::string& documentTypeId, bool forSaving) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetFileLoader(documentTypeId, forSaving);
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CCompositeDocumentTemplateComp::CreateDocument(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->CreateDocument(documentTypeId);
	}
	else{
		return NULL;
	}
}


istd::IPolymorphic* CCompositeDocumentTemplateComp::CreateView(
			const std::string& documentTypeId,
			istd::IChangeable* documentPtr,
			const std::string& viewTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->CreateView(documentTypeId, documentPtr, viewTypeId);
	}
	else{
		return NULL;
	}
}


imod::IUndoManager* CCompositeDocumentTemplateComp::CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->CreateUndoManager(documentTypeId, documentPtr);
	}
	else{
		return NULL;
	}
}


istd::CStringList CCompositeDocumentTemplateComp::GetFileFilters(const std::string* documentTypeIdPtr) const
{
	istd::CStringList retVal;

	if (documentTypeIdPtr != NULL){
		IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(*documentTypeIdPtr);
		if (iter != m_idToTemplateMap.end()){
			I_ASSERT(iter->second != NULL);

			return iter->second->GetFileFilters(documentTypeIdPtr);
		}
		else{
			return retVal;
		}
	}

	int slavesCount = m_slaveTemplatesCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IDocumentTemplate* slavePtr = m_slaveTemplatesCompPtr[i];
		if (slavePtr != NULL){
			istd::CStringList filters = slavePtr->GetFileFilters(NULL);

			retVal.insert(retVal.end(), filters.begin(), filters.end());
		}
	}

	return retVal;
}


istd::CStringList CCompositeDocumentTemplateComp::GetFileExtensions(const std::string* documentTypeIdPtr) const
{
	istd::CStringList retVal;

	if (documentTypeIdPtr != NULL){
		IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(*documentTypeIdPtr);
		if (iter != m_idToTemplateMap.end()){
			I_ASSERT(iter->second != NULL);

			return iter->second->GetFileExtensions(documentTypeIdPtr);
		}
		else{
			return retVal;
		}
	}

	int slavesCount = m_slaveTemplatesCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IDocumentTemplate* slavePtr = m_slaveTemplatesCompPtr[i];
		if (slavePtr != NULL){
			istd::CStringList filters = slavePtr->GetFileExtensions(NULL);

			retVal.insert(retVal.end(), filters.begin(), filters.end());
		}
	}

	return retVal;
}


istd::CString CCompositeDocumentTemplateComp::GetDefaultDirectory(const istd::CString& sugestedDir, const std::string* documentTypeIdPtr) const
{
	if (documentTypeIdPtr != NULL){
		IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(*documentTypeIdPtr);
		if (iter != m_idToTemplateMap.end()){
			I_ASSERT(iter->second != NULL);

			return iter->second->GetDefaultDirectory(sugestedDir, documentTypeIdPtr);
		}
		else{
			return sugestedDir;
		}
	}

	int slavesCount = m_slaveTemplatesCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IDocumentTemplate* slavePtr = m_slaveTemplatesCompPtr[i];
		if (slavePtr != NULL){
			istd::CString retVal = slavePtr->GetDefaultDirectory(sugestedDir);
			if (retVal.IsEmpty()){
				return retVal;
			}
		}
	}

	return "";
}


// reimplemented (icomp::IComponent)

void CCompositeDocumentTemplateComp::OnComponentCreated()
{
	int slavesCount = m_slaveTemplatesCompPtr.GetCount();

	if (slavesCount > 0){
		m_idToTemplateMap[""] = m_slaveTemplatesCompPtr[0];
	}

	for (int i = 0; i < slavesCount; ++i){
		const IDocumentTemplate* slavePtr = m_slaveTemplatesCompPtr[i];
		if (slavePtr != NULL){
			Ids ids = slavePtr->GetDocumentTypeIds();
			for (Ids::const_iterator idIter = ids.begin(); idIter != ids.end(); ++idIter){
				const std::string& documentTypeId = *idIter;
				m_idToTemplateMap[documentTypeId] = slavePtr;
			}
		}
	}
}


void CCompositeDocumentTemplateComp::OnComponentDestroyed()
{
	m_idToTemplateMap.clear();
}


} // namespace idoc


