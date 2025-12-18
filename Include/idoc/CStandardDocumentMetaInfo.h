#ifndef idoc_CStandardDocumentMetaInfo_included
#define idoc_CStandardDocumentMetaInfo_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <iser/ISerializable.h>
#include <idoc/IDocumentMetaInfo.h>


namespace idoc
{


/**
	Implementation of the basic document's meta information.
*/
class CStandardDocumentMetaInfo:
			virtual public IDocumentMetaInfo,
			virtual public iser::ISerializable
{
public:
	// reimplemented (idoc::IDocumentMetaInfo)
	virtual MetaInfoTypes GetMetaInfoTypes(bool allowReadOnly = true) const override;
	virtual QVariant GetMetaInfo(int metaInfoType) const override;
	virtual bool SetMetaInfo(int metaInfoType, const QVariant& metaInfo) override;
	virtual void RemoveMetaInfo(int metaInfoType) override;
	virtual QByteArray GetMetaInfoId(int metaInfoType) const override;
	virtual QString GetMetaInfoName(int metaInfoType) const override;
	virtual QString GetMetaInfoDescription(int metaInfoType) const override;
	virtual bool IsMetaInfoWritable(int metaInfoType) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	typedef QMap<int, QVariant> MetaInfoMap;
	MetaInfoMap m_infosMap;
};


} // namespace idoc


#endif // !idoc_CStandardDocumentMetaInfo_included


