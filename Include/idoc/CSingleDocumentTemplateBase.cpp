#include "idoc/CSingleDocumentTemplateBase.h"


// ACF includes
#include "iser/ISerializable.h"

#include "idoc/IDocumentManager.h"
#include "idoc/CSerializedStateComparator.h"


namespace idoc
{


void CSingleDocumentTemplateBase::SetSupportedFeatures(int featureFlags)
{
	m_supportedFeatures = featureFlags;
}


void CSingleDocumentTemplateBase::SetDocumentTypeId(const QByteArray& id)
{
	m_documentTypeId = id;
}


void CSingleDocumentTemplateBase::SetDocumentTypeName(const QString& name)
{
	m_documentTypeName = name;
}


void CSingleDocumentTemplateBase::SetViewTypeId(const QByteArray& id)
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

bool CSingleDocumentTemplateBase::IsFeatureSupported(int featureFlags, const QByteArray& documentId) const
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


QString CSingleDocumentTemplateBase::GetDocumentTypeName(const QByteArray& documentTypeId) const
{
	if (documentTypeId == m_documentTypeId){
		if (!m_documentTypeName.isEmpty()){
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


iser::IFileTypeInfo* CSingleDocumentTemplateBase::GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const
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
		const QByteArray& id = *iter;

		iser::IFileLoader* loaderPtr = GetFileLoader(*iter);
		if (loaderPtr->IsOperationSupported(NULL, &filePath)){
			retVal.push_back(id);
		}
	}

	return retVal;
}


QString CSingleDocumentTemplateBase::GetDefaultDirectory(const QString& sugestedDir, const QByteArray* documentTypeIdPtr) const
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

IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetViewTypeIds(const QByteArray& documentTypeId) const
{
	IDocumentTemplate::Ids retVal;

	if (documentTypeId == m_documentTypeId){
		retVal.push_back(m_viewTypeId);
	}

	return retVal;
}


QString CSingleDocumentTemplateBase::GetViewTypeName(
			const QByteArray& documentTypeId,
			const QByteArray& viewTypeId) const
{
	if ((documentTypeId == m_documentTypeId) && (viewTypeId == m_viewTypeId)){
		if (!m_viewTypeName.isEmpty()){
			return m_viewTypeName;
		}
		else{
			return m_viewTypeId;
		}
	}

	return QString();
}


IDocumentStateComparator* CSingleDocumentTemplateBase::CreateStateComparator(const QByteArray& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return new CSerializedStateComparator;
	}

	return NULL;
}


} // namespace idoc


