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
	*/
	CRegistry(const IComponentStaticInfo* factoryPtr);

	// reimplemented (icomp::IRegistry)
	virtual Ids GetElementIds() const;
	virtual ElementInfo* InsertElementInfo(
			const ::std::string& elementId,
			int elementType,
			const ::std::string& packageId,
			const ::std::string& factoryId,
			bool ensureElementCreated = true);
	virtual const ElementInfo* GetElementInfo(const ::std::string& elementId) const;
	virtual bool RemoveElementInfo(const ::std::string& elementId);
	virtual const InterfaceInfos& GetExportedInterfaceInfos() const;
	virtual const SubcomponentMap& GetExportedSubcomponentMap() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef ::std::map< ::std::string, ElementInfo> ElementsMap;

	ElementsMap m_elementInfos;
	InterfaceInfos m_interfaceInfos;
	SubcomponentMap m_subcomponentMap;

	const IComponentStaticInfo& m_componentsFactory;
};


}//namespace icomp


#endif // !icomp_CRegistry_included


