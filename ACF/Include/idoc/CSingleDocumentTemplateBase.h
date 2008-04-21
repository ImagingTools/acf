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
	virtual void SetDocumentTypeId(const std::string& documentTypeId);
	virtual void SetFileFilters(const istd::CStringList& fileFilters);
	virtual void SetDefaultDirectory(const istd::CString& defaultDirectory);
	virtual void SetFileExtensions(const istd::CStringList& fileExtensions);

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetDocumentTypeIds() const;
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const;
	imod::IUndoManager* CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const;
	virtual istd::CStringList GetFileFilters(const std::string* documentTypeIdPtr = NULL) const;
	virtual istd::CStringList GetFileExtensions(const std::string* documentTypeIdPtr = NULL) const;
	virtual istd::CString GetDefaultDirectory(const istd::CString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const;

protected:
	bool IsDocumentTypeSupported(const std::string& documentTypeId) const;
	bool IsViewTypeSupported(const std::string& viewTypeId) const;

	idoc::IDocumentManager* m_documentManagerPtr;

private:
	istd::CStringList m_fileFilters;
	istd::CString m_defaultDirectory;
	istd::CStringList m_fileExtensions;

	std::string m_documentTypeId;
	std::string m_viewTypeId;
};


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

