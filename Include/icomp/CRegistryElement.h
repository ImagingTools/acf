#ifndef icomp_CRegistryElement_included
#define icomp_CRegistryElement_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include "icomp/IRegistryElement.h"


namespace icomp
{


class IAttributeStaticInfo;


class CRegistryElement: virtual public IRegistryElement
{
public:
	CRegistryElement();

	virtual AttributeInfo* GetAttributeInfo(const QByteArray& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual quint32 GetElementFlags() const;
	virtual void SetElementFlags(quint32 flags);
	virtual AttributeInfo* InsertAttributeInfo(const QByteArray& attributeId, const QByteArray& attributeType);
	virtual iser::IObject* CreateAttribute(const QByteArray& attributeType) const;
	virtual const AttributeInfo* GetAttributeInfo(const QByteArray& attributeId) const;
	virtual bool RemoveAttribute(const QByteArray& attributeId);

	// reimplemented (iattr::IAttributesProvider)
	virtual AttributeIds GetAttributeIds() const;
	virtual iser::IObject* GetAttribute(const QByteArray& attributeId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	quint32 m_elementFlags;

	typedef QMap<QByteArray, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


} // namespace icomp


#endif // !icomp_CRegistryElement_included


