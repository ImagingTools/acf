#ifndef icomp_CRegistryElement_included
#define icomp_CRegistryElement_included


#include <map>
#include <string>

#include "icomp/IRegistryElement.h"


namespace icomp
{


class IAttributeStaticInfo;


class CRegistryElement: virtual public IRegistryElement
{
public:
	/**
		Create this component registry.
		This registry must be initilize using method Initialize().
	*/
	CRegistryElement();

	virtual AttributeInfo* GetAttributeInfo(const std::string& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual I_DWORD GetElementFlags() const;
	virtual void SetElementFlags(I_DWORD flags);
	virtual Ids GetAttributeIds() const;
	virtual AttributeInfo* InsertAttributeInfo(const std::string& attributeId, const std::string& attributeType);
	virtual iser::IObject* CreateAttribute(const std::string& attributeType) const;
	virtual const AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual bool RemoveAttribute(const std::string& attributeId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_DWORD m_elementFlags;

	typedef std::map< std::string, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


}//namespace icomp


#endif // !icomp_CRegistryElement_included


