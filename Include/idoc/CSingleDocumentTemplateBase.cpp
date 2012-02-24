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


void CSingleDocumentTemplateBase::SetDocumentTypeName(const QString& name)
{
	m_documentTypeName = name;
}


void CSingleDocumentTemplateBase::SetViewTypeId(const std::string& id)
{
	m_viewTypeId = id;
}


void CSingleDocumentTemplateBase::SetViewTypeName(const QString& name)
{
	m_viewTypeName = name;
}


void CSingleDocumentTemplateBase::SetDefaultDirectory(const QString& defaultDirectory)
{
	m_defaultDirectory = defaultDirectory;
}


// reimplemented (idoc::IDocumentTypesInfo)

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


QString CSingleDocumentTemplateBase::GetDocumentTypeName(const std::string& documentTypeId) const
{
	if (documentTypeId == m_documentTypeId){
		if (!m_documentTypeName.isEmpty()){
			return m_documentTypeName;
		}
		else{
			return QString::fromStdString(documentTypeId);
		}
	}
	else{
		return "";
	}
}


iser::IFileTypeInfo* CSingleDocumentTemplateBase::GetDocumentFileTypeInfo(const std::string& documentTypeId) const
{
	return GetFileLoader(documentTypeId);
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetDocumentTypeIdsForFile(const QString& filePath) const
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


QString CSingleDocumentTemplateBase::GetDefaultDirectory(const QString& sugestedDir, const std::string* documentTypeIdPtr) const
{
	if (sugestedDir.isEmpty()){
		if ((documentTypeIdPtr == NULL) || IsDocumentTypeSupported(*documentTypeIdPtr)){
			return m_defaultDirectory;
		}
		else{
			return QString();
		}
	}

	return sugestedDir;
}


// reimplemented (idoc::IDocumentTemplate)

IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetViewTypeIds(const std::string& documentTypeId) const
{
	IDocumentTemplate::Ids retVal;

	if (documentTypeId == m_documentTypeId){
		retVal.push_back(m_viewTypeId);
	}

	return retVal;
}


QString CSingleDocumentTemplateBase::GetViewTypeName(
			const std::string& documentTypeId,
			const std::string& viewTypeId) const
{
	if ((documentTypeId == m_documentTypeId) && (viewTypeId == m_viewTypeId)){
		if (!m_viewTypeName.isEmpty()){
			return m_viewTypeName;
		}
		else{
			return QString::fromStdString(m_viewTypeId);
		}
	}

	return QString();
}


IDocumentStateComparator* CSingleDocumentTemplateBase::CreateStateComparator(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return new CSerializedStateComparator;
	}

	return NULL;
}


} // namespace idoc


