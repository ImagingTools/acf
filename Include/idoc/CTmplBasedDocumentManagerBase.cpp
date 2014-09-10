#include "idoc/CTmplBasedDocumentManagerBase.h"


#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CTmplBasedDocumentManagerBase::CTmplBasedDocumentManagerBase()
:	m_documentTemplatePtr(NULL),
	m_baseAllowedFlags(0)
{
}


// reimplemented (idoc::IDocumentManager)

int CTmplBasedDocumentManagerBase::GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr) const
{
	int retVal = m_baseAllowedFlags;

	if (viewPtr == NULL){
		viewPtr = GetActiveView();
		if (viewPtr == NULL){
			return retVal;
		}
	}

	DocumentInfo docInfo;
	const istd::IChangeable* documentPtr = GetDocumentFromView(*viewPtr, &docInfo);
	if (documentPtr == NULL){
		return retVal;
	}

	if ((m_documentTemplatePtr != NULL) && (documentPtr != NULL)){
		QByteArray docTypeId = GetDocumentTypeId(*documentPtr);
		ifile::IFilePersistence* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
		if (loaderPtr != NULL){
			if (docInfo.filePath.isEmpty() || loaderPtr->IsOperationSupported(
						documentPtr,
						&docInfo.filePath,
						ifile::IFilePersistence::QF_SAVE | ifile::IFilePersistence::QF_FILE)){
				retVal |= OF_FILE_SAVE;
			}

			if (loaderPtr->IsOperationSupported(
						documentPtr,
						NULL,
						ifile::IFilePersistence::QF_SAVE | ifile::IFilePersistence::QF_FILE)){
				retVal |= OF_FILE_SAVE_AS;
			}
		}
	}

	if (!docInfo.filePath.isEmpty()){
		retVal |= OF_KNOWN_PATH;
	}

	return retVal;
}


// reimplemented (idoc::IDocumentTypesInfo)

bool CTmplBasedDocumentManagerBase::IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->IsFeatureSupported(featureFlags, documentTypeId);
	}

	return false;
}


IDocumentTypesInfo::Ids CTmplBasedDocumentManagerBase::GetDocumentTypeIds() const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeIds();
	}

	return Ids();
}


QString CTmplBasedDocumentManagerBase::GetDocumentTypeName(const QByteArray& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeName(documentTypeId);
	}

	return "";
}


ifile::IFileTypeInfo* CTmplBasedDocumentManagerBase::GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetFileLoader(documentTypeId);
	}

	return NULL;
}


IDocumentTypesInfo::Ids CTmplBasedDocumentManagerBase::GetDocumentTypeIdsForFile(const QString& filePath) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);
	}

	return Ids();
}


QString CTmplBasedDocumentManagerBase::GetDefaultDirectory(const QString& sugestedDir, const QByteArray* documentTypeIdPtr) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDefaultDirectory(sugestedDir, documentTypeIdPtr);
	}

	return "";
}


// protected methods

const idoc::IDocumentTemplate* CTmplBasedDocumentManagerBase::GetDocumentTemplate() const
{
	return m_documentTemplatePtr;
}


void CTmplBasedDocumentManagerBase::SetDocumentTemplate(const IDocumentTemplate* documentTemplatePtr)
{
	m_documentTemplatePtr = documentTemplatePtr;

	m_baseAllowedFlags = 0;

	if (m_documentTemplatePtr != NULL){
		m_baseAllowedFlags |= OF_FILE_NEW;

		IDocumentTemplate::Ids docTypeIds = m_documentTemplatePtr->GetDocumentTypeIds();
		for (		IDocumentTemplate::Ids::const_iterator iter = docTypeIds.begin();
					iter != docTypeIds.end();
					++iter){
			const QByteArray& docTypeId = *iter;

			ifile::IFilePersistence* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
			if (		(loaderPtr != NULL) &&
						loaderPtr->IsOperationSupported(NULL, NULL, ifile::IFilePersistence::QF_LOAD | ifile::IFilePersistence::QF_FILE)){
				m_baseAllowedFlags |= OF_FILE_OPEN;

				break;
			}
		}
	}
}


} // namespace idoc


