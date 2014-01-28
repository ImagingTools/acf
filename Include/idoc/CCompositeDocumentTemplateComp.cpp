#include "idoc/CCompositeDocumentTemplateComp.h"


namespace idoc
{


// reimplemented (idoc::IDocumentTypesInfo)

bool CCompositeDocumentTemplateComp::IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const
{
	IdToTemplateMap::ConstIterator foundTemplateIter = m_idToTemplateMap.constFind(documentTypeId);
	if (foundTemplateIter != m_idToTemplateMap.constEnd()){
		return foundTemplateIter.value()->IsFeatureSupported(featureFlags, documentTypeId);
	}

	return false;
}


IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetDocumentTypeIds() const
{
	Ids retVal;

	for (		IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constBegin();
				iter != m_idToTemplateMap.constEnd();
				++iter){
		const QByteArray& documentTypeId = iter.key();
		if (!documentTypeId.isEmpty()){
			retVal.push_back(iter.key());
		}
	}

	return retVal;
}


QString CCompositeDocumentTemplateComp::GetDocumentTypeName(const QByteArray& documentTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->GetDocumentTypeName(documentTypeId);
	}
	else{
		return "";
	}
}


ifile::IFileTypeInfo* CCompositeDocumentTemplateComp::GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->GetDocumentFileTypeInfo(documentTypeId);
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

			retVal += slaveIds;
		}
	}

	return retVal;
}


QString CCompositeDocumentTemplateComp::GetDefaultDirectory(const QString& sugestedDir, const QByteArray* documentTypeIdPtr) const
{
	if (documentTypeIdPtr != NULL){
		IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(*documentTypeIdPtr);
		if (iter != m_idToTemplateMap.constEnd()){
			Q_ASSERT(iter.value() != NULL);

			return iter.value()->GetDefaultDirectory(sugestedDir, documentTypeIdPtr);
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

IDocumentTemplate::Ids CCompositeDocumentTemplateComp::GetViewTypeIds(const QByteArray& documentTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->GetViewTypeIds(documentTypeId);
	}
	else{
		return Ids();
	}
}


QString CCompositeDocumentTemplateComp::GetViewTypeName(
			const QByteArray& documentTypeId,
			const QByteArray& viewTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->GetViewTypeName(documentTypeId, viewTypeId);
	}
	else{
		return "";
	}
}


ifile::IFilePersistence* CCompositeDocumentTemplateComp::GetFileLoader(const QByteArray& documentTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->GetFileLoader(documentTypeId);
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CCompositeDocumentTemplateComp::CreateDocument(QByteArray& documentTypeId, bool initialize) const
{
	if (documentTypeId.isEmpty()){
		IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constBegin();
		if (iter != m_idToTemplateMap.constEnd()){
			Q_ASSERT(iter.value() != NULL);

			documentTypeId = iter.key();

			return iter.value()->CreateDocument(documentTypeId, initialize);
		}
	}
	else{
		IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
		if (iter != m_idToTemplateMap.constEnd()){
			Q_ASSERT(iter.value() != NULL);

			return iter.value()->CreateDocument(documentTypeId, initialize);
		}
	}

	return NULL;
}


istd::IPolymorphic* CCompositeDocumentTemplateComp::CreateView(
			const QByteArray& documentTypeId,
			istd::IChangeable* documentPtr,
			const QByteArray& viewTypeId) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->CreateView(documentTypeId, documentPtr, viewTypeId);
	}
	else{
		return NULL;
	}
}


idoc::IUndoManager* CCompositeDocumentTemplateComp::CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const
{
	IdToTemplateMap::ConstIterator iter = m_idToTemplateMap.constFind(documentTypeId);
	if (iter != m_idToTemplateMap.constEnd()){
		Q_ASSERT(iter.value() != NULL);

		return iter.value()->CreateUndoManager(documentTypeId, documentPtr);
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
				const QByteArray& documentTypeId = *idIter;
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


