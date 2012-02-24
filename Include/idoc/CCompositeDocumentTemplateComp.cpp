#include "idoc/CCompositeDocumentTemplateComp.h"


namespace idoc
{


// reimplemented (idoc::IDocumentTypesInfo)

bool CCompositeDocumentTemplateComp::IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator foundTemplateIter = m_idToTemplateMap.find(documentTypeId);
	if (foundTemplateIter != m_idToTemplateMap.end()){
		return foundTemplateIter->second->IsFeatureSupported(featureFlags, documentTypeId);
	}

	return false;
}


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


QString CCompositeDocumentTemplateComp::GetDocumentTypeName(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetDocumentTypeName(documentTypeId);
	}
	else{
		return "";
	}
}


iser::IFileTypeInfo* CCompositeDocumentTemplateComp::GetDocumentFileTypeInfo(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetDocumentFileTypeInfo(documentTypeId);
	}
	else{
		return NULL;
	}
}


IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetDocumentTypeIdsForFile(const QString& filePath) const
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


QString CCompositeDocumentTemplateComp::GetDefaultDirectory(const QString& sugestedDir, const std::string* documentTypeIdPtr) const
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
			QString retVal = slavePtr->GetDefaultDirectory(sugestedDir);
			if (retVal.isEmpty()){
				return retVal;
			}
		}
	}

	return "";
}


// reimplemented (idoc::IDocumentTemplate)

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


QString CCompositeDocumentTemplateComp::GetViewTypeName(
			const std::string& documentTypeId,
			const std::string& viewTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetViewTypeName(documentTypeId, viewTypeId);
	}
	else{
		return "";
	}
}


iser::IFileLoader* CCompositeDocumentTemplateComp::GetFileLoader(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->GetFileLoader(documentTypeId);
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


IDocumentStateComparator* CCompositeDocumentTemplateComp::CreateStateComparator(const std::string& documentTypeId) const
{
	IdToTemplateMap::const_iterator iter = m_idToTemplateMap.find(documentTypeId);
	if (iter != m_idToTemplateMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->CreateStateComparator(documentTypeId);
	}
	else{
		return NULL;
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CCompositeDocumentTemplateComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

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

	BaseClass::OnComponentDestroyed();
}


} // namespace idoc


