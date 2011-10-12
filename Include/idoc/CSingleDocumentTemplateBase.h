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
	virtual void SetDocumentTypeName(const istd::CString& name);
	virtual void SetViewTypeId(const std::string& id);
	virtual void SetViewTypeName(const istd::CString& name);
	virtual void SetDefaultDirectory(const istd::CString& defaultDirectory);

	// reimplemented (idoc::IDocumentTemplate)
	virtual bool IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIds() const;
	virtual istd::CString GetDocumentTypeName(const std::string& documentTypeId) const;
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const;
	virtual istd::CString GetViewTypeName(
				const std::string& documentTypeId,
				const std::string& viewTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const;
	virtual IDocumentStateComparator* CreateStateComparator(const std::string& documentTypeId) const;
	virtual istd::CString GetDefaultDirectory(const istd::CString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const;

protected:
	bool IsDocumentTypeSupported(const std::string& documentTypeId) const;
	bool IsViewTypeSupported(const std::string& viewTypeId) const;

private:
	istd::CString m_defaultDirectory;

	std::string m_documentTypeId;
	istd::CString m_documentTypeName;
	std::string m_viewTypeId;
	istd::CString m_viewTypeName;

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

