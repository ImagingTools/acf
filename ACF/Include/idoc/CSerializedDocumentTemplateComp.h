#ifndef idoc_CSerializedDocumentTemplateComp_included
#define idoc_CSerializedDocumentTemplateComp_included


#include "iser/IFileSerializer.h"

#include "icomp/CComponentBase.h"

#include "idoc/CSingleDocumentTemplateBase.h"


namespace idoc
{		


class CSerializedDocumentTemplateComp: public icomp::CComponentBase, public CSingleDocumentTemplateBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CSingleDocumentTemplateBase BaseClass2;
	
	I_BEGIN_COMPONENT(CSerializedDocumentTemplateComp)
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate)
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of supported document", true, "Default");
		I_ASSIGN_MULTI_1(m_fileFiltersAttrPtr, "FileFilter", "File filter for this document", true, "Document Files (*.*)")
		I_ASSIGN(m_defaultDirectoryAttrPtr, "DefaultDirectory", "Default file directory for open file dialog", true, ".")
		I_ASSIGN_MULTI_1(m_fileExtensionsAttrPtr, "FileExtensions", "The list of possible file extensions for the document", true, "*.*")
		I_ASSIGN(m_documentCompFact, "DocumentFactory", "Document factory", true, "DocumentFactory")
		I_ASSIGN(m_viewCompFact, "ViewFactory", "Create of document GUI", true, "ViewFactory")
		I_ASSIGN(m_fileSerializerCompPtr, "DocumentSerializer", "Provide document loading and saving", true, "DocumentSerializer");
	I_END_COMPONENT

	// reimplemented (idoc::IDocumentTemplate)
	virtual bool LoadDocumentFromFile(const istd::CString& filePath, imod::IModel& result) const;
	virtual bool SaveDocumentToFile(const imod::IModel& document, const istd::CString& filePath) const;
	virtual imod::IModel* CreateDocument(const std::string& documentTypeId) const;
	virtual istd::IPolymorphic* CreateView(imod::IModel* documentPtr, const std::string& viewTypeId = std::string()) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_documentTypeIdAttrPtr);
	I_MULTI_ATTR(istd::CString, m_fileFiltersAttrPtr);
	I_ATTR(istd::CString, m_defaultDirectoryAttrPtr);
	I_MULTI_ATTR(istd::CString, m_fileExtensionsAttrPtr);
	I_FACT(imod::IModel, m_documentCompFact);
	I_FACT(imod::IObserver, m_viewCompFact);
	I_REF(iser::IFileSerializer, m_fileSerializerCompPtr);
};


} // namespace idoc


#endif // !idoc_CSerializedDocumentTemplateComp_included

