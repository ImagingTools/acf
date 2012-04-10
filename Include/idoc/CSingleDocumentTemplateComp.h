#ifndef idoc_CSingleDocumentTemplateComp_included
#define idoc_CSingleDocumentTemplateComp_included


#include "iser/IFileLoader.h"

#include "imod/IModel.h"

#include "icomp/CComponentBase.h"

#include "idoc/CSingleDocumentTemplateBase.h"


namespace idoc
{		


class CSingleDocumentTemplateComp: public icomp::CComponentBase, public CSingleDocumentTemplateBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CSingleDocumentTemplateBase BaseClass2;
	
	I_BEGIN_COMPONENT(CSingleDocumentTemplateComp);
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate);
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of supported document", true, "Default");
		I_ASSIGN(m_documentTypeNameAttrPtr, "DocumentTypeName", "Human readable name of document", false, "");
		I_ASSIGN(m_defaultDirectoryAttrPtr, "DefaultDirectory", "Default file directory for open file dialog", true, ".");
		I_ASSIGN(m_documentCompFact, "DocumentFactory", "Document factory", true, "DocumentFactory");
		I_ASSIGN_TO(m_modelCompFact, m_documentCompFact, true);
		I_ASSIGN(m_viewCompFact, "ViewFactory", "Create of document GUI", true, "ViewFactory");
		I_ASSIGN(m_fileLoaderCompPtr, "DocumentLoader", "Provide document loading and saving", true, "DocumentLoader");
		I_ASSIGN(m_isNewSupportedAttrPtr, "IsNewSupported", "Template supports the new operation", true, false);
		I_ASSIGN(m_isEditSupportedAttrPtr, "IsEditSupported", "Template supports the edit operation", true, false);
	I_END_COMPONENT;

	// reimplemented (idoc::IDocumentTemplate)
	virtual iser::IFileLoader* GetFileLoader(const QByteArray& documentTypeId) const;
	virtual istd::IChangeable* CreateDocument(const QByteArray& documentTypeId) const;
	virtual istd::IPolymorphic* CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const;
	imod::IUndoManager* CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const;

protected:
	/**
		Extract pointer of view interface from factorisied view component.
		This functionality is provided to allow derrived classes
		to extract implementation-specific GUI interfaces from created composited instances.
		Default is instance of interface imod::IObserver extracted, but for
		composite components it can be inaccurate for some cases.
	*/
	virtual istd::IPolymorphic* ExtractViewInterface(icomp::IComponent* componentPtr) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(bool, m_isNewSupportedAttrPtr);
	I_ATTR(bool, m_isEditSupportedAttrPtr);
	I_ATTR(QByteArray, m_documentTypeIdAttrPtr);
	I_ATTR(QString, m_documentTypeNameAttrPtr);
	I_ATTR(QString, m_defaultDirectoryAttrPtr);
	I_FACT(istd::IChangeable, m_documentCompFact);
	I_FACT(imod::IModel, m_modelCompFact);
	I_FACT(imod::IObserver, m_viewCompFact);
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);

	int m_featureFlags;
};


} // namespace idoc


#endif // !idoc_CSingleDocumentTemplateComp_included

