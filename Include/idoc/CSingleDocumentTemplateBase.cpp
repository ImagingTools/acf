#include "idoc/CSingleDocumentTemplateBase.h"


// STL includes
#include <algorithm>

#include "istd/TDelPtr.h"
#include "istd/CStaticServicesProvider.h"

#include "iser/ISerializable.h"

#include "imod/IModel.h"
#include "imod/CSerializedUndoManager.h"
#include "imod/TModelWrap.h"

#include "isys/IFileSystem.h"

#include "idoc/IDocumentManager.h"
#include "idoc/CSerializedStateComparator.h"


namespace idoc
{


void CSingleDocumentTemplateBase::SetSupportedFeatures(int featureFlags)
{
	m_supportedFeatures = featureFlags;
}


void CSingleDocumentTemplateBase::SetDocumentTypeId(const std::string& documentTypeId)
{
	m_documentTypeId = documentTypeId;
}


void CSingleDocumentTemplateBase::SetDefaultDirectory(const istd::CString& defaultDirectory)
{
	m_defaultDirectory = defaultDirectory;
}


// reimplemented (idoc::IDocumentTemplate)

bool CSingleDocumentTemplateBase::IsFeatureSupported(int featureFlags, const std::string& documentId) const
{
	if (documentId == m_documentTypeId){
		return ((m_supportedFeatures & featureFlags) != 0);
	}

	return false;
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetDocumentTypeIds() const
{
	IDocumentTemplate::Ids retVal;

	retVal.push_back(m_documentTypeId);

	return retVal;
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetViewTypeIds(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return CSingleDocumentTemplateBase::GetDocumentTypeIds();
	}

	return Ids();
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetDocumentTypeIdsForFile(const istd::CString& filePath) const
{
	IDocumentTemplate::Ids retVal;

	Ids docTypeIds = GetDocumentTypeIds();
	for (		Ids::const_iterator iter = docTypeIds.begin();
				iter != docTypeIds.end();
				++iter){
		const std::string& id = *iter;

		iser::IFileLoader* loaderPtr = GetFileLoader(*iter);
		if (loaderPtr->IsOperationSupported(NULL, &filePath)){
			retVal.push_back(id);
		}
	}

	return retVal;
}


imod::IUndoManager* CSingleDocumentTemplateBase::CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(documentPtr);
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(documentPtr);
		if ((serializablePtr != NULL) && (modelPtr != NULL)){
			istd::TDelPtr<imod::TModelWrap<imod::CSerializedUndoManager> > undoManagerModelPtr(new imod::TModelWrap<imod::CSerializedUndoManager>);
			if (		undoManagerModelPtr.IsValid() &&
						modelPtr->AttachObserver(undoManagerModelPtr.GetPtr())){
				return undoManagerModelPtr.PopPtr();
			}
		}
	}

	return NULL;
}


IDocumentStateComparator* CSingleDocumentTemplateBase::CreateStateComparator(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return new CSerializedStateComparator;
	}

	return NULL;
}


istd::CString CSingleDocumentTemplateBase::GetDefaultDirectory(const istd::CString& sugestedDir, const std::string* documentTypeIdPtr) const
{
	if (sugestedDir.IsEmpty()){
		if ((documentTypeIdPtr == NULL) || IsDocumentTypeSupported(*documentTypeIdPtr)){
			return m_defaultDirectory;
		}
		else{
			return istd::CString();
		}
	}

	return sugestedDir;
}


} // namespace idoc


