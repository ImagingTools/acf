#ifndef idoc_CSingleDocumentTemplateBase_included
#define idoc_CSingleDocumentTemplateBase_included


// STL includes
#include <limits>

#include "imod/IModel.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


class IDocumentManager;


/**
	Base class implementing document template for single document type.
*/
class CSingleDocumentTemplateBase: virtual public idoc::IDocumentTemplate
{
public:
	const std::string& GetDocumentTypeId() const;
	virtual void SetSupportedFeatures(int featureFlags);
	virtual void SetDocumentTypeId(const std::string& id);
	virtual void SetDocumentTypeName(const QString& name);
	virtual void SetViewTypeId(const std::string& id);
	virtual void SetViewTypeName(const QString& name);
	virtual void SetDefaultDirectory(const QString& defaultDirectory);

	// reimplemented (idoc::IDocumentTypesInfo)
	virtual bool IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIds() const;
	virtual QString GetDocumentTypeName(const std::string& documentTypeId) const;
	virtual iser::IFileTypeInfo* GetDocumentFileTypeInfo(const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const;
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const;
	virtual QString GetViewTypeName(
				const std::string& documentTypeId,
				const std::string& viewTypeId) const;
	virtual IDocumentStateComparator* CreateStateComparator(const std::string& documentTypeId) const;

protected:
	bool IsDocumentTypeSupported(const std::string& documentTypeId) const;
	bool IsViewTypeSupported(const std::string& viewTypeId) const;

private:
	QString m_defaultDirectory;

	std::string m_documentTypeId;
	QString m_documentTypeName;
	std::string m_viewTypeId;
	QString m_viewTypeName;

	int m_supportedFeatures;
};


// inline methods

inline const std::string& CSingleDocumentTemplateBase::GetDocumentTypeId() const
{
	return m_documentTypeId;
}


// inline protected methods

inline bool CSingleDocumentTemplateBase::IsDocumentTypeSupported(const std::string& documentTypeId) const
{
	return documentTypeId.empty() || (documentTypeId == m_documentTypeId);	// default or the same type is accepted
}


inline bool CSingleDocumentTemplateBase::IsViewTypeSupported(const std::string& viewTypeId) const
{
	return viewTypeId.empty() || (viewTypeId == m_viewTypeId);	// default or the same type is accepted
}


} // namespace idoc


#endif // !idoc_CSingleDocumentTemplateBase_included

