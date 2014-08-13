#ifndef icomp_CPackageStaticInfo_included
#define icomp_CPackageStaticInfo_included


#include "icomp/CComponentStaticInfoBase.h"


namespace iser
{
	class IArchive;
};


namespace icomp
{


/**
	Static info for component package.
	Please note, that in composed component concept there is no distinguish between package and component.
*/
class CPackageStaticInfo: virtual public CComponentStaticInfoBase
{
public:
	CPackageStaticInfo();
	CPackageStaticInfo(
				const QString& description,
				const QString& keywords);

	void Reset();

	virtual bool SerializeMeta(iser::IArchive& archive);

	/**
		Register embedded component static info.
		\param	embeddedId			ID of this embedded component.
		\param	componentInfoPtr	embedded component static info object used to describe its type.
									It cannot be NULL.
	*/
	virtual void RegisterEmbeddedComponentInfo(const QByteArray& embeddedId, const IComponentStaticInfo* componentInfoPtr);

	// reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const QByteArray& embeddedId) const;

	// reimplemented (icomp::IComponentStaticInfo)
	virtual const QString& GetDescription() const;
	virtual const QString& GetKeywords() const;

private:
	typedef QMap<QByteArray, const IComponentStaticInfo*> EmbeddedComponentInfos;
	EmbeddedComponentInfos m_embeddedComponentInfos;

	QString m_description;
	QString m_keywords;
};


} // namespace icomp


#endif // !icomp_CPackageStaticInfo_included


