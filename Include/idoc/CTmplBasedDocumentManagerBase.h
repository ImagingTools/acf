#ifndef idoc_CTmplBasedDocumentManagerBase_included
#define idoc_CTmplBasedDocumentManagerBase_included


// ACF includes
#include <idoc/IDocumentManager.h>


namespace idoc
{


class IDocumentTemplate;


/**
	Base implementation of document manager.
	This implementation base on the document template concept.
	\sa IDocumentTemplate
*/
class CTmplBasedDocumentManagerBase: virtual public idoc::IDocumentManager
{
public:
	CTmplBasedDocumentManagerBase();

	// reimplemented (idoc::IDocumentManager)
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const override;

	// reimplemented (idoc::IDocumentTypesInfo)
	virtual bool IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const override;
	virtual Ids GetDocumentTypeIds() const override;
	virtual QString GetDocumentTypeName(const QByteArray& documentTypeId) const override;
	virtual ifile::IFileTypeInfo* GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const override;
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const override;
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const QByteArray* documentTypeIdPtr = NULL) const override;

protected:
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const;
	void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);

private:
	const IDocumentTemplate* m_documentTemplatePtr;

	int m_baseAllowedFlags;
};


} // namespace idoc


#endif // idoc_CTmplBasedDocumentManagerBase_included


