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

	I_BEGIN_COMPONENT(CCompositeDocumentTemplateComp);
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate);
		I_ASSIGN_MULTI_0(m_slaveTemplatesCompPtr, "SlaveTemplates", "Set of document templates", true);
	I_END_COMPONENT;

	// reimplemented (idoc::IDocumentTypesInfo)
	virtual bool IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const;
	virtual Ids GetDocumentTypeIds() const;
	virtual QString GetDocumentTypeName(const QByteArray& documentTypeId) const;
	virtual iser::IFileTypeInfo* GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const;
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const QByteArray* documentTypeIdPtr = NULL) const;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetViewTypeIds(const QByteArray& documentTypeId) const;
	virtual QString GetViewTypeName(
				const QByteArray& documentTypeId,
				const QByteArray& viewTypeId) const;
	virtual iser::IFileLoader* GetFileLoader(const QByteArray& documentTypeId) const;
	virtual istd::IChangeable* CreateDocument(const QByteArray& documentTypeId) const;
	virtual istd::IPolymorphic* CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const;
	virtual idoc::IUndoManager* CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const;
	virtual IDocumentStateComparator* CreateStateComparator(const QByteArray& documentTypeId) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(idoc::IDocumentTemplate, m_slaveTemplatesCompPtr);

	typedef QMap<QByteArray, const idoc::IDocumentTemplate*> IdToTemplateMap;
	IdToTemplateMap m_idToTemplateMap;
};


} // namespace idoc


#endif // !idoc_CCompositeDocumentTemplateComp_included


