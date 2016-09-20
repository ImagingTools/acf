#include <icomp/CRelatedInfoRegistrator.h>


namespace icomp
{


CRelatedInfoRegistrator::CRelatedInfoRegistrator(
			IRealAttributeStaticInfo& baseAttributeInfo,
			int metaGroupId,
			const QByteArray& id,
			int flags)
{
	int baseFlags = baseAttributeInfo.GetAttributeFlags();
	if ((baseFlags & IAttributeStaticInfo::AF_NULLABLE) != 0){
		flags |= IAttributeStaticInfo::AF_NULLABLE;
	}
	baseAttributeInfo.AddRelatedMetaId(metaGroupId, id, flags);
}


} // namespace icomp


