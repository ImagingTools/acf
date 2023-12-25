#ifndef icomp_CRegistryElement_included
#define icomp_CRegistryElement_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include <icomp/IRegistryElement.h>


namespace icomp
{


class IAttributeStaticInfo;


class CRegistryElement: virtual public IRegistryElement
{
public:
	CRegistryElement();

	virtual AttributeInfo* GetAttributeInfo(const QByteArray& attributeId);

	// reimplemented (icomp::IRegistryElement)
	virtual quint32 GetElementFlags() const override;
	virtual void SetElementFlags(quint32 flags) override;
	virtual AttributeInfo* InsertAttributeInfo(const QByteArray& attributeId, const QByteArray& attributeType) override;
	virtual iser::IObject* CreateAttribute(const QByteArray& attributeType) const override;
	virtual const AttributeInfo* GetAttributeInfo(const QByteArray& attributeId) const override;
	virtual bool RemoveAttribute(const QByteArray& attributeId) override;

	// reimplemented (iattr::IAttributesProvider)
	virtual AttributeIds GetAttributeIds() const override;
	virtual iser::IObject* GetAttribute(const QByteArray& attributeId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	iser::IObject* CreateAttributeIntern(const QByteArray& attributeType) const;

private:
	quint32 m_elementFlags;

	typedef QMap<QByteArray, AttributeInfo> AttributeInfoMap;
	AttributeInfoMap m_attributeInfos;
};


} // namespace icomp


#endif // !icomp_CRegistryElement_included


