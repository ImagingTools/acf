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

	/**
		Initialize registry with static info.
		\param	infoPtr		pointer to static info object. It cannot be NULL.
	*/
	void Initialize(const IComponentStaticInfo* infoPtr);


	virtual AttributeInfo* GetAttributeInfo(const std::string& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual I_DWORD GetElementFlags() const;
	virtual void SetElementFlags(I_DWORD flags);
	virtual const IComponentStaticInfo& GetComponentStaticInfo() const;
	virtual Ids GetAttributeIds() const;
	virtual AttributeInfo* InsertAttributeInfo(const std::string& attributeId);
	virtual iser::IObject* CreateAttribute(const std::string& attributeId) const;
	virtual const AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual bool RemoveAttribute(const std::string& attributeId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual const IAttributeStaticInfo* GetAttributeStaticInfo(const std::string& attributeId) const;

private:
	const IComponentStaticInfo* m_staticInfoPtr;

	I_DWORD m_elementFlags;

	typedef std::map< std::string, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


}//namespace icomp


#endif // !icomp_CRegistryElement_included


