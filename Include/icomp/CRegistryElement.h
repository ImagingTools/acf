#ifndef icomp_CRegistryElement_included
#define icomp_CRegistryElement_included


#include <map>
#include <string>

#include "istd/TChangeDelegator.h"

#include "icomp/IRegistryElement.h"

#include "imod/TModelWrap.h"


namespace icomp
{


class IAttributeStaticInfo;


class CRegistryElement: public imod::TModelWrap<istd::TChangeDelegator<IRegistryElement> >
{
public:
	/**
		Create this component registry info and initialize it with static info.
		\param	infoPtr		pointer to static info object. It cannot be NULL.
	*/
	CRegistryElement(const IComponentStaticInfo* infoPtr);

	virtual AttributeInfo* GetAttributeInfo(const std::string& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual const IComponentStaticInfo& GetComponentStaticInfo() const;
	virtual Ids GetAttributeIds() const;
	virtual AttributeInfo* InsertAttributeInfo(const std::string& attributeId, bool createAttribute = true);
	virtual iser::ISerializable* CreateAttribute(const std::string& attributeId) const;
	virtual const AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual bool RemoveAttribute(const std::string& attributeId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	const IAttributeStaticInfo* GetAttributeStaticInfo(const std::string& attributeId) const;

private:
	const IComponentStaticInfo& m_staticInfo;

	typedef std::map< std::string, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


}//namespace icomp


#endif // !icomp_CRegistryElement_included


