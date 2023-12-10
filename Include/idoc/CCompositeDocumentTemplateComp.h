#ifndef idoc_CCompositeDocumentTemplateComp_included
#define idoc_CCompositeDocumentTemplateComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <idoc/IDocumentTemplate.h>


namespace idoc
{


/**
	Complex document template consists of more simple templates.
*/
class CCompositeDocumentTemplateComp:
			public icomp::CComponentBase,
			virtual public IDocumentTemplate
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCompositeDocumentTemplateComp);
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate);
		I_ASSIGN_MULTI_0(m_slaveTemplatesCompPtr, "SlaveTemplates", "Set of document templates", true);
	I_END_COMPONENT;

	// reimplemented (idoc::IDocumentTypesInfo)
	virtual bool IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const override;
	virtual Ids GetDocumentTypeIds() const override;
	virtual QString GetDocumentTypeName(const QByteArray& documentTypeId) const override;
	virtual ifile::IFileTypeInfo* GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const override;
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const override;
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const QByteArray* documentTypeIdPtr = NULL) const override;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetViewTypeIds(const QByteArray& documentTypeId) const override;
	virtual QString GetViewTypeName(
				const QByteArray& documentTypeId,
				const QByteArray& viewTypeId) const override;
	virtual ifile::IFilePersistence* GetFileLoader(const QByteArray& documentTypeId) const override;
	virtual istd::IChangeable* CreateDocument(
				QByteArray& documentTypeId,
				bool initialize = true,
				bool beQuiet = false,
				bool* ignoredFlagPtr = NULL) const override;
	virtual istd::IPolymorphic* CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const override;
	virtual idoc::IUndoManager* CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_MULTIREF(idoc::IDocumentTemplate, m_slaveTemplatesCompPtr);

	typedef QMap<QByteArray, const idoc::IDocumentTemplate*> IdToTemplateMap;
	IdToTemplateMap m_idToTemplateMap;
};


} // namespace idoc


#endif // !idoc_CCompositeDocumentTemplateComp_included


