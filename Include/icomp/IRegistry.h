#ifndef icomp_IRegistry_included
#define icomp_IRegistry_included


// STL includes
#include <string>
#include <set>
#include <map>


// ACF includes
#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"

#include "icomp/IRegistryElement.h"
#include "icomp/CComponentAddress.h"


namespace icomp
{


/**
	Interface representing information stored in component registry.
	Registry is model representating smallest component topology unit.
	Registry can be imagined as representation of arx files.

	\ingroup ComponentConcept
*/
class IRegistry: virtual public iser::ISerializable
{
public:
	typedef std::set<std::string> Ids;
	typedef istd::TDelPtr<IRegistryElement> ElementPtr;

	/**
		Change flags.
	*/
	enum ChangeFlags
	{
		CF_COMPONENT_ADDED = 1 << 24,
		CF_COMPONENT_REMOVED = 1 << 25,
		CF_COMPONENT_EXPORTED = 1 << 26,
		CF_COMPONENT_RENAMED = 1 << 27,
		CF_EMBEDDED = 1 << 28
	};

	/**
		Describe information stored with each element.
	*/
	struct ElementInfo
	{
		ElementPtr elementPtr;				///<	Pointer to registry element
		icomp::CComponentAddress address;	///<	Address of component used to create its instance.
	};

	/**
		Map assigning interface ID (undecorated) to component ID exporting this interface.
	*/
	typedef std::map<std::string, std::string> ExportedInterfacesMap;

	/**
		Map assigning exported sub-component names to internal subcomponent ID's.
	*/
	typedef std::map<std::string, std::string> ExportedComponentsMap;

	/**
		Get ID list of existing elements.
	*/
	virtual Ids GetElementIds() const = 0;

	/**
		Get access to stored attribute info structure.
	*/
	virtual const ElementInfo* GetElementInfo(const std::string& elementId) const = 0;

	/**
		Insert new attribute info object to collection of attributes.
		\param	elementId				unique ID of new element (component).
		\param	address					address describing element type.
		\param	ensureElementCreated	if true new element will be created.
	*/
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true) = 0;

	/**
		Removes attribute info structure from this collection.
	*/
	virtual bool RemoveElementInfo(const std::string& elementId) = 0;

	/**
		Rename an registry element.
		\param	oldElementId		ID of element to be renamed.
		\param	newElementId		the new ID of element.
	*/
	virtual bool RenameElement(const std::string& oldElementId, const std::string& newElementId) = 0;

	/**
		Get list of IDs of embedded registries.
		Embedded registries are used to manage local composite components.
		\return	set of IDs.
	*/
	virtual Ids GetEmbeddedRegistryIds() const = 0;

	/**
		Get access to embedded registry using its ID.
		Embedded registries are used to manage local composite components.
	*/
	virtual IRegistry* GetEmbeddedRegistry(const std::string& registryId) const = 0;

	/**
		Insert embedded registry with specified ID.
		\return	instance of new created embedded registry.
	*/
	virtual IRegistry* InsertEmbeddedRegistry(const std::string& registryId) = 0;

	/**
		Remove embedded registry with specified ID.
		\return	true, if registry could be removed.
	*/
	virtual bool RemoveEmbeddedRegistry(const std::string& registryId) = 0;

	/**
		Rename a embedded registry.
		\param	oldElementId		ID of element to be renamed.
		\param	newElementId		the new ID of element.
	*/
	virtual bool RenameEmbeddedRegistry(const std::string& oldRegistryId, const std::string& newRegistryId) = 0;

	/**
		Get access to information structure of exported interfaces.
	*/
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const = 0;

	/**
		Get access to map used to convert exported sub-components to internal sub-component ID's.
	*/
	virtual const ExportedComponentsMap& GetExportedComponentsMap() const = 0;

	/**
		Enable or disable exporting single or all interfaces of component.
		\param	elementId			ID of element in registry.
		\param	exportInterfaceInfo	specify interface. If must be invalid.
		\param	state				the export state. If \c false, the existing export will be removed.
	*/
	virtual void SetElementInterfaceExported(
				const std::string& elementId,
				const std::string& interfaceName,
				bool state = true) = 0;

	/**
		Set element to be exported.
		\param	exportId		ID of exported name, or NULL if this export should be removed.
		\param	elementId		ID of element or its subelement (with dot separated) or empty string, if this export should be removed.
	*/
	virtual void SetElementExported(
				const std::string& exportId,
				const std::string& elementId) = 0;

	/**
		Get human readable description of this registry.
	*/
	virtual const QString& GetDescription() const = 0;

	/**
		set human readable description of this registry.
	*/
	virtual void SetDescription(const QString& description) = 0;

	/**
		Get keywords for this registry used for search.
	*/
	virtual const QString& GetKeywords() const = 0;

	/**
		Set keywords for this registry used for search.
	*/
	virtual void SetKeywords(const QString& keywords) = 0;
};


} //namespace icomp


#endif // !icomp_IRegistry_included


