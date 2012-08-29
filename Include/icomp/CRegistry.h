#ifndef icomp_CRegistry_included
#define icomp_CRegistry_included


#include "istd/TChangeDelegator.h"

#include "imod/TModelWrap.h"

#include "icomp/IRegistry.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


/**
	Standard implementation of registry.
*/
class CRegistry: virtual public IRegistry
{
public:
	// reimplemented (icomp::IRegistry)
	virtual Ids GetElementIds() const;
	virtual const ElementInfo* GetElementInfo(const QByteArray& elementId) const;
	virtual ElementInfo* InsertElementInfo(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);
	virtual bool RemoveElementInfo(const QByteArray& elementId);
	virtual bool RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId);
	virtual Ids GetEmbeddedRegistryIds() const;
	virtual IRegistry* GetEmbeddedRegistry(const QByteArray& registryId) const;
	virtual IRegistry* InsertEmbeddedRegistry(const QByteArray& registryId);
	virtual bool RemoveEmbeddedRegistry(const QByteArray& registryId);
	virtual bool RenameEmbeddedRegistry(const QByteArray& oldRegistryId, const QByteArray& newRegistryId);
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const;
	virtual const ExportedElementsMap& GetExportedElementsMap() const;
	virtual void SetElementInterfaceExported(
				const QByteArray& elementId,
				const QByteArray& interfaceName,
				bool state = true);
	virtual void SetElementExported(
				const QByteArray& exportId,
				const QByteArray& elementId);
	virtual const QString& GetDescription() const;
	virtual void SetDescription(const QString& description);
	virtual const QString& GetKeywords() const;
	virtual void SetKeywords(const QString& keywords);
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual quint32 GetMinimalVersion(int versionId) const;

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CRegistryElement> > Element;

	/**
		Called to create instance of registry element.
	*/
	virtual icomp::IRegistryElement* CreateRegistryElement(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address) const;
	virtual bool SerializeComponents(iser::IArchive& archive);
	virtual bool SerializeEmbeddedRegistries(iser::IArchive& archive);
	virtual bool SerializeExportedInterfaces(iser::IArchive& archive);
	virtual bool SerializeExportedComponents(iser::IArchive& archive);

private:
	typedef QMap<QByteArray, ElementInfo> ComponentsMap;
	typedef istd::TDelPtr<IRegistry> RegistryPtr;
	typedef QMap<QByteArray, RegistryPtr> EmbeddedRegistriesMap;

	ComponentsMap m_componentsMap;
	EmbeddedRegistriesMap m_embeddedRegistriesMap;
	ExportedInterfacesMap m_exportedInterfacesMap;
	ExportedElementsMap m_exportedComponentsMap;

	QString m_description;
	QString m_keywords;
};


} // namespace icomp


#endif // !icomp_CRegistry_included


