#ifndef idoc_CStandardDocumentMetaInfo_included
#define idoc_CStandardDocumentMetaInfo_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iser/ISerializable.h"
#include "idoc/IDocumentMetaInfo.h"


namespace idoc
{


/**
	Simple implementation of a text document model.
*/
class CStandardDocumentMetaInfo:
			virtual public IDocumentMetaInfo,
			virtual public iser::ISerializable
{
public:
	// reimplemented (idoc::IDocumentMetaInfo)
	virtual MetaInfoTypes GetSupportedMetaInfoTypes(bool allowReadOnly = true) const;
	virtual QVariant GetDocumentMetaInfo(int metaInfoType) const;
	virtual bool SetDocumentMetaInfo(int metaInfoType, const QVariant& metaInfo);
	virtual QString GetMetaInfoName(int metaInfoType) const;
	virtual QString GetMetaInfoDescription(int metaInfoType) const;
	virtual bool IsMetaInfoWritable(int metaInfoType) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const IChangeable& object) const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	typedef QMap<int, QVariant> MetaInfoMap;

	QMap<int, QVariant> m_infosMap;
};


} // namespace idoc


#endif // !idoc_CStandardDocumentMetaInfo_included


