#ifndef icomp_TAttributeStaticInfo_included
#define icomp_TAttributeStaticInfo_included


// STL includes
#include <map>


// ACF includes
#include "istd/CClassInfo.h"

#include "icomp/IRealAttributeStaticInfo.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


template <class Attribute>
class TAttributeStaticInfo: virtual public IRealAttributeStaticInfo
{
public:
	TAttributeStaticInfo(
				icomp::CBaseComponentStaticInfo& staticInfo,
				const std::string& id,
				const std::string& description,
				const Attribute* defaultValuePtr,
				int flags,
				const istd::CClassInfo& relatedInterfaceInfo);

	// reimplemented (icomp::IRealAttributeStaticInfo)
	virtual const std::string& GetAttributeId() const;
	virtual void AddRelatedMetaId(int metaGroupId, const std::string& id, int flags);

	// reimplemented (icomp::IAttributeStaticInfo)
	virtual const std::string& GetAttributeDescription() const;
	virtual const iser::IObject* GetAttributeDefaultValue() const;
	virtual std::string GetAttributeTypeName() const;
	virtual IElementStaticInfo::Ids GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const;
	virtual int GetAttributeFlags() const;

private:
	std::string m_id;
	std::string m_description;
	const Attribute* m_defaultValuePtr;
	int m_attributeFlags;

	typedef std::map<std::string, int> IdsToFlagsMap;
	typedef std::map<int, IdsToFlagsMap> RelatedIdsMap;
	RelatedIdsMap m_relatedInterfacesMap;
};


// public methods

template <class Attribute>
TAttributeStaticInfo<Attribute>::TAttributeStaticInfo(
			icomp::CBaseComponentStaticInfo& staticInfo,
			const std::string& id,
			const std::string& description,
			const Attribute* defaultValuePtr,
			int flags,
			const istd::CClassInfo& relatedInterfaceInfo)
:	m_id(id),
	m_description(description),
	m_defaultValuePtr(defaultValuePtr),
	m_attributeFlags(flags)
{
	if (relatedInterfaceInfo.IsValid() && !relatedInterfaceInfo.IsVoid()){
		m_relatedInterfacesMap[IComponentStaticInfo::MGI_INTERFACES][relatedInterfaceInfo.GetName()] = flags;
	}

	staticInfo.RegisterAttributeInfo(id, this);
}


// reimplemented (icomp::IRealAttributeStaticInfo)

template <class Attribute>
const std::string& TAttributeStaticInfo<Attribute>::GetAttributeId() const
{
	return m_id;
}


template <class Attribute>
void TAttributeStaticInfo<Attribute>::AddRelatedMetaId(int metaGroupId, const std::string& id, int flags)
{
	IdsToFlagsMap& groupMap = m_relatedInterfacesMap[metaGroupId];

	IdsToFlagsMap::iterator foundIter = groupMap.find(id);
	if (foundIter != groupMap.end()){
		I_ASSERT((flags & (AF_REFERENCE | AF_FACTORY)) == (foundIter->second & (AF_REFERENCE | AF_FACTORY)));	// factory can be combined with factory only and reference with reference

		foundIter->second |= flags & AF_OBLIGATORY;	// is obligatory if once obligatory registred is
		foundIter->second &= flags | ~AF_NULLABLE;	// is nullable if always nullable registred is
	}
	else{
		groupMap[id] = flags;
	}

	if (metaGroupId == IComponentStaticInfo::MGI_INTERFACES){
		m_attributeFlags |= flags & AF_OBLIGATORY;	// attribute is obligatory if one of declared interfaces obligatory is
		m_attributeFlags &= flags | ~AF_NULLABLE;	// attribute is nullable if all of declared interfaces nullable are
	}
}


// reimplemented (icomp::IAttributeStaticInfo)

template <class Attribute>
const std::string& TAttributeStaticInfo<Attribute>::GetAttributeDescription() const
{
	return m_description;
}


template <class Attribute>
const iser::IObject* TAttributeStaticInfo<Attribute>::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr;
}


template <class Attribute>
std::string TAttributeStaticInfo<Attribute>::GetAttributeTypeName() const
{
	return Attribute::GetTypeName();
}


template <class Attribute>
IElementStaticInfo::Ids TAttributeStaticInfo<Attribute>::GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const
{
	IElementStaticInfo::Ids retVal;

	RelatedIdsMap::const_iterator foundGroupIter = m_relatedInterfacesMap.find(metaGroupId);
	if (foundGroupIter != m_relatedInterfacesMap.end()){
		const IdsToFlagsMap& groupMap = foundGroupIter->second;

		for (		IdsToFlagsMap::const_iterator iter = groupMap.begin();
					iter != groupMap.end();
					++iter){
						const std::string& id = iter->first;
			int idFlags = iter->second;

			if (((idFlags & flagsMask) == flags)){
				retVal.insert(id);
			}
		}
	}

	return retVal;
}


template <class Attribute>
int TAttributeStaticInfo<Attribute>::GetAttributeFlags() const
{
	return m_attributeFlags;
}


} // namespace icomp


#endif // !icomp_TAttributeStaticInfo_included


