#ifndef icomp_CRegistryElement_included
#define icomp_CRegistryElement_included


// STL includes
#include <QtCore/QMap>
#include <string>


// ACF includes
#include "icomp/IRegistryElement.h"


namespace icomp
{


class IAttributeStaticInfo;


class CRegistryElement: virtual public IRegistryElement
{
public:
	CRegistryElement();

	virtual AttributeInfo* GetAttributeInfo(const std::string& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual quint32 GetElementFlags() const;
	virtual void SetElementFlags(quint32 flags);
	virtual Ids GetAttributeIds() const;
	virtual AttributeInfo* InsertAttributeInfo(const std::string& attributeId, const std::string& attributeType);
	virtual iser::IObject* CreateAttribute(const std::string& attributeType) const;
	virtual const AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual bool RemoveAttribute(const std::string& attributeId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	quint32 m_elementFlags;

	typedef QMap< std::string, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


} // namespace icomp


#endif // !icomp_CRegistryElement_included


