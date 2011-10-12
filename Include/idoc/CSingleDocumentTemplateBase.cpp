#include "idoc/CSingleDocumentTemplateBase.h"


// STL includes
#include <algorithm>


// ACF includes
#include "istd/CStaticServicesProvider.h"

#include "iser/ISerializable.h"

#include "isys/IFileSystem.h"

#include "idoc/IDocumentManager.h"
#include "idoc/CSerializedStateComparator.h"


namespace idoc
{


void CSingleDocumentTemplateBase::SetSupportedFeatures(int featureFlags)
{
	m_supportedFeatures = featureFlags;
}


void CSingleDocumentTemplateBase::SetDocumentTypeId(const std::string& id)
{
	m_documentTypeId = id;
}


void CSingleDocumentTemplateBase::SetDocumentTypeName(const istd::CString& name)
{
	m_documentTypeName = name;
}


void CSingleDocumentTemplateBase::SetViewTypeId(const std::string& id)
{
	m_viewTypeId = id;
}


void CSingleDocumentTemplateBase::SetViewTypeName(const istd::CString& name)
{
	m_viewTypeName = name;
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


istd::CString CSingleDocumentTemplateBase::GetDocumentTypeName(const std::string& documentTypeId) const
{
	if (documentTypeId == m_documentTypeId){
		if (!m_documentTypeName.IsEmpty()){
			return m_documentTypeName;
		}
		else{
			return documentTypeId;
		}
	}
	else{
		return "";
	}
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetViewTypeIds(const std::string& documentTypeId) const
{
	IDocumentTemplate::Ids retVal;

	if (documentTypeId == m_documentTypeId){
		retVal.push_back(m_viewTypeId);
	}

	return retVal;
}


istd::CString CSingleDocumentTemplateBase::GetViewTypeName(
			const std::string& documentTypeId,
			const std::string& viewTypeId) const
{
	if ((documentTypeId == m_documentTypeId) && (viewTypeId == m_viewTypeId)){
		if (!m_viewTypeName.IsEmpty()){
			return m_viewTypeName;
		}
		else{
			return m_viewTypeId;
		}
	}
	else{
		return "";
	}
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


