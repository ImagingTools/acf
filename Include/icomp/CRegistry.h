#ifndef icomp_CRegistry_included
#define icomp_CRegistry_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include <istd/TChangeDelegator.h>
#include <imod/TModelWrap.h>
#include <icomp/IRegistry.h>
#include <icomp/CRegistryElement.h>


namespace icomp
{


/**
	Standard implementation of registry.
*/
class CRegistry: virtual public IRegistry
{
public:
	CRegistry();

	// reimplemented (icomp::IRegistry)
	virtual Ids GetElementIds() const override;
	virtual const ElementInfo* GetElementInfo(const QByteArray& elementId) const override;
	virtual ElementInfo* InsertElementInfo(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true) override;
	virtual bool RemoveElementInfo(const QByteArray& elementId) override;
	virtual bool RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId) override;
	virtual Ids GetEmbeddedRegistryIds() const override;
	virtual IRegistry* GetEmbeddedRegistry(const QByteArray& registryId) const override;
	virtual IRegistry* InsertEmbeddedRegistry(const QByteArray& registryId) override;
	virtual bool RemoveEmbeddedRegistry(const QByteArray& registryId) override;
	virtual bool RenameEmbeddedRegistry(const QByteArray& oldRegistryId, const QByteArray& newRegistryId) override;
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const override;
	virtual const ExportedElementsMap& GetExportedElementsMap() const override;
	virtual void SetElementInterfaceExported(
				const QByteArray& elementId,
				const QByteArray& interfaceName,
				bool state = true) override;
	virtual void SetElementExported(
				const QByteArray& exportId,
				const QByteArray& elementId) override;
	virtual const QString& GetDescription() const override;
	virtual void SetDescription(const QString& description) override;
	virtual const QString& GetKeywords() const override;
	virtual void SetKeywords(const QString& keywords) override;
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;
	virtual quint32 GetMinimalVersion(int versionId) const override;

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

	typedef istd::TDelPtr<IRegistry> RegistryPtr;
	typedef QMap<QByteArray, RegistryPtr> EmbeddedRegistriesMap;
	EmbeddedRegistriesMap m_embeddedRegistriesMap;

private:
	typedef QMap<QByteArray, ElementInfo> ComponentsMap;

	ComponentsMap m_componentsMap;
	ExportedInterfacesMap m_exportedInterfacesMap;
	ExportedElementsMap m_exportedComponentsMap;

	QString m_description;
	QString m_keywords;

#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_mutex;
#else
	mutable QMutex m_mutex;
#endif
};


} // namespace icomp


#endif // !icomp_CRegistry_included


