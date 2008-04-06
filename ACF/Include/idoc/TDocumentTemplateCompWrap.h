#ifndef idoc_TDocumentTemplateCompWrap_included
#define idoc_TDocumentTemplateCompWrap_included


#include "idoc/CMultiDocumentTemplate.h"
#include "idoc/CSingleDocumentTemplate.h"

#include "icomp/CComponentBase.h"


namespace idoc
{		


template <class DocumentTemplateImpl> 
class TDocumentTemplateCompWrap: public DocumentTemplateImpl, public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef DocumentTemplateImpl BaseClass2;
	
	I_BEGIN_COMPONENT(TDocumentTemplateCompWrap)
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate)
		I_ASSIGN(m_documentFactoryCompPtr, "DocumentFactory", "Document factory", true, "DocumentFactory")
		I_ASSIGN(m_viewFactoryCompPtr, "ViewFactory", "View factory", true, "ViewFactory")
		I_ASSIGN_MULTI_1(m_fileFiltersAttrPtr, "FileFilter", "File filter for this document", true, "Document Files (*.*)")
		I_ASSIGN(m_defaultDirectoryAttrPtr, "DefaultDirectory", "Default file directory for open file dialog", true, ".")
		I_ASSIGN(m_defaultTitleAttrPtr, "DefaultTitle", "Default title for the new document", true, "Untitled")
		I_ASSIGN_MULTI_1(m_fileExtensionsAttrPtr, "FileExtensions", "The list of possible file extensions for the document", true, "*")
	I_END_COMPONENT

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTI_ATTR(istd::CString, m_fileFiltersAttrPtr);
	I_ATTR(istd::CString, m_defaultDirectoryAttrPtr);
	I_ATTR(istd::CString, m_defaultTitleAttrPtr);
	I_MULTI_ATTR(istd::CString, m_fileExtensionsAttrPtr);
	I_REF(idoc::IDocumentTemplate::IDocumentFactory, m_documentFactoryCompPtr);
	I_REF(idoc::IDocumentTemplate::IViewFactory, m_viewFactoryCompPtr);

};


// reimplemented (icomp::CComponentBase)

template <class DocumentTemplateImpl> 
void TDocumentTemplateCompWrap<DocumentTemplateImpl>::OnComponentCreated()
{

	if (m_fileFiltersAttrPtr.IsValid()){
		istd::CStringList fileFilters;
		for (int index = 0; index < m_fileFiltersAttrPtr.GetCount(); index++){
			fileFilters.push_back(m_fileFiltersAttrPtr[index]);		
		}

		SetFileFilters(fileFilters);
	}

	if (m_defaultDirectoryAttrPtr.IsValid()){
		SetDefaultDirectory(m_defaultDirectoryAttrPtr->GetValue());
	}

	if (m_fileExtensionsAttrPtr.IsValid()){
		istd::CStringList fileExtensions;
		for (int index = 0; index < m_fileExtensionsAttrPtr.GetCount(); index++){
			fileExtensions.push_back(m_fileExtensionsAttrPtr[index]);		
		}
		
		SetFileExtensions(fileExtensions);
	}

	if (m_documentFactoryCompPtr.IsValid()){
		RegisterDocumentFactory(m_documentFactoryCompPtr.GetPtr());
	}

	if (m_viewFactoryCompPtr.IsValid()){
		RegisterViewFactory(m_viewFactoryCompPtr.GetPtr());
	}
}


} // namespace idoc


#endif // !idoc_TDocumentTemplateCompWrap_included

