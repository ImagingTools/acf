#ifndef idoc_CCompositeDocumentTemplateComp_included
#define idoc_CCompositeDocumentTemplateComp_included


#include "icomp/CComponentBase.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{


/**
	Complex document template consists of more simple templates.
*/
class CCompositeDocumentTemplateComp: public icomp::CComponentBase, virtual public IDocumentTemplate
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCompositeDocumentTemplateComp)
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate)
		I_ASSIGN_MULTI_0(m_slaveTemplatesCompPtr, "SlaveTemplates", "Set of document templates", true);
	I_END_COMPONENT

	// reimplemented (idoc::IDocumentTemplate)
	virtual bool IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIds() const;
	virtual Ids GetViewTypeIds(const std::string& documentTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const;
	virtual iser::IFileLoader* GetFileLoader(const std::string& documentTypeId) const;
	virtual istd::IChangeable* CreateDocument(const std::string& documentTypeId) const;
	virtual istd::IPolymorphic* CreateView(
				const std::string& documentTypeId,
				istd::IChangeable* documentPtr,
				const std::string& viewTypeId = std::string()) const;
	virtual imod::IUndoManager* CreateUndoManager(const std::string& documentTypeId, istd::IChangeable* documentPtr) const;
	virtual IDocumentStateComparator* CreateStateComparator(const std::string& documentTypeId) const;
	virtual istd::CString GetDefaultDirectory(const istd::CString& sugestedDir = "", const std::string* documentTypeIdPtr = NULL) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(idoc::IDocumentTemplate, m_slaveTemplatesCompPtr);

	typedef std::map<std::string, const idoc::IDocumentTemplate*> IdToTemplateMap;
	IdToTemplateMap m_idToTemplateMap;
};


} // namespace idoc


#endif // !idoc_CCompositeDocumentTemplateComp_included


