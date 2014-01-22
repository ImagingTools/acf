#ifndef idoc_CSingleDocumentTemplateBase_included
#define idoc_CSingleDocumentTemplateBase_included


// ACF includes
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
	const QByteArray& GetDocumentTypeId() const;
	virtual void SetSupportedFeatures(int featureFlags);
	virtual void SetDocumentTypeId(const QByteArray& id);
	virtual void SetDocumentTypeName(const QString& name);
	virtual void SetViewTypeId(const QByteArray& id);
	virtual void SetViewTypeName(const QString& name);
	virtual void SetDefaultDirectory(const QString& defaultDirectory);

	// reimplemented (idoc::IDocumentTypesInfo)
	virtual bool IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const;
	virtual Ids GetDocumentTypeIds() const;
	virtual QString GetDocumentTypeName(const QByteArray& documentTypeId) const;
	virtual ifile::IFileTypeInfo* GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const;
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const;
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const QByteArray* documentTypeIdPtr = NULL) const;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetViewTypeIds(const QByteArray& documentTypeId) const;
	virtual QString GetViewTypeName(
				const QByteArray& documentTypeId,
				const QByteArray& viewTypeId) const;

protected:
	bool IsDocumentTypeSupported(const QByteArray& documentTypeId) const;
	bool IsViewTypeSupported(const QByteArray& viewTypeId) const;

private:
	QString m_defaultDirectory;

	QByteArray m_documentTypeId;
	QString m_documentTypeName;
	QByteArray m_viewTypeId;
	QString m_viewTypeName;

	int m_supportedFeatures;
};


// inline methods

inline const QByteArray& CSingleDocumentTemplateBase::GetDocumentTypeId() const
{
	return m_documentTypeId;
}


// inline protected methods

inline bool CSingleDocumentTemplateBase::IsDocumentTypeSupported(const QByteArray& documentTypeId) const
{
	return documentTypeId.isEmpty() || (documentTypeId == m_documentTypeId);	// default or the same type is accepted
}


inline bool CSingleDocumentTemplateBase::IsViewTypeSupported(const QByteArray& viewTypeId) const
{
	return viewTypeId.isEmpty() || (viewTypeId == m_viewTypeId);	// default or the same type is accepted
}


} // namespace idoc


#endif // !idoc_CSingleDocumentTemplateBase_included

