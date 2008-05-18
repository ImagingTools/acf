#ifndef icomp_CRegistry_included
#define icomp_CRegistry_included


#include "icomp/IRegistry.h"


namespace icomp
{


/**
	Standard implementation of registry.
*/
class CRegistry: virtual public IRegistry
{
public:
	/**
		Constructor.
		\param	factoryPtr	pointer to main static info object used to factorize real components.
		\sa SetComponentStaticInfo()
	*/
	CRegistry(const IComponentStaticInfo* factoryPtr = NULL);

	/**
		Return \c true, if the registry can be used.
	*/
	virtual bool IsValid() const;

	/**
		Set component factory. Only, if a component factory was set, the registry object becomes a valid state and can be used.
		\sa IsValid()
	*/
	virtual void SetComponentStaticInfo(const IComponentStaticInfo* factoryPtr);

	// reimplemented (icomp::IRegistry)
	virtual Ids GetElementIds() const;
	virtual ElementInfo* InsertElementInfo(
			const std::string& elementId,
			int elementType,
			const std::string& packageId,
			const std::string& componentId,
			bool ensureElementCreated = true);
	virtual const ElementInfo* GetElementInfo(const std::string& elementId) const;
	virtual bool RemoveElementInfo(const std::string& elementId);
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const;
	virtual const ExportedComponentsMap& GetExportedComponentsMap() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	bool SerializeComponents(iser::IArchive& archive);
	bool SerializeExportedInterfaces(iser::IArchive& archive);
	bool SerializeExportedComponents(iser::IArchive& archive);

private:
	typedef std::map< std::string, ElementInfo> ComponentsMap;

	ComponentsMap m_componentsMap;
	ExportedInterfacesMap m_exportedInterfacesMap;
	ExportedComponentsMap m_exportedComponentsMap;

	const IComponentStaticInfo* m_componentsFactoryPtr;
};


}//namespace icomp


#endif // !icomp_CRegistry_included


