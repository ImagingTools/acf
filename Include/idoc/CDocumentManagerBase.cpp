#include "idoc/CDocumentManagerBase.h"


#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
:	m_documentTemplatePtr(NULL)
{
}


void CDocumentManagerBase::SetDocumentTemplate(const IDocumentTemplate* documentTemplatePtr)
{
	m_documentTemplatePtr = documentTemplatePtr;

	m_baseAllowedFlags = 0;

	if (m_documentTemplatePtr != NULL){
		m_baseAllowedFlags |= OF_FILE_NEW;

		IDocumentTemplate::Ids docTypeIds = m_documentTemplatePtr->GetDocumentTypeIds();
		for (		IDocumentTemplate::Ids::const_iterator iter = docTypeIds.begin();
					iter != docTypeIds.end();
					++iter){
			const std::string& docTypeId = *iter;

			iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
			if (		(loaderPtr != NULL) &&
						loaderPtr->IsOperationSupported(NULL, NULL, iser::IFileLoader::QF_NAMED_ONLY | iser::IFileLoader::QF_NO_SAVING)){
				m_baseAllowedFlags |= OF_FILE_OPEN;

				break;
			}
		}
	}
}


// reimplemented (idoc::IDocumentManager)

int CDocumentManagerBase::GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr) const
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
		std::string docTypeId = GetDocumentTypeId(*documentPtr);
		iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
		if (loaderPtr != NULL){
			if (docInfo.filePath.IsEmpty() || loaderPtr->IsOperationSupported(
						documentPtr,
						&docInfo.filePath,
						iser::IFileLoader::QF_NAMED_ONLY | iser::IFileLoader::QF_NO_LOADING)){
				retVal |= OF_FILE_SAVE;
			}

			if (loaderPtr->IsOperationSupported(
						documentPtr,
						NULL,
						iser::IFileLoader::QF_NAMED_ONLY | iser::IFileLoader::QF_NO_LOADING)){
				retVal |= OF_FILE_SAVE_AS;
			}
		}
	}

	return retVal;
}


const idoc::IDocumentTemplate* CDocumentManagerBase::GetDocumentTemplate() const
{
	return m_documentTemplatePtr;
}


} // namespace idoc


