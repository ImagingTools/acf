#ifndef icomp_IRegistry_included
#define icomp_IRegistry_included


#include <string>
#include <set>
#include <map>

#include "istd/TDelPtr.h"
#include "istd/CClassInfo.h"

#include "iser/ISerializable.h"

#include "icomp/IRegistryElement.h"
#include "icomp/CComponentAddress.h"


namespace icomp
{


class IRegistry: virtual public iser::ISerializable
{
public:
	typedef std::set< std::string> Ids;
	typedef istd::TDelPtr<IRegistryElement> ElementPtr;

	/**
		Change flags.
	*/
	enum ChangeFlags
	{
		CF_COMPONENT_ADDED = 0x100,
		CF_COMPONENT_REMOVED = 0x200,
		CF_COMPONENT_EXPORTED = 0x400
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
	typedef std::map<istd::CClassInfo, std::string> ExportedInterfacesMap;

	/**
		Map assigning exported sub-component names to internal subcomponent ID's.
	*/
	typedef std::map< std::string, std::string> ExportedComponentsMap;

	/**
		Get ID list of existing elements.
	*/
	virtual Ids GetElementIds() const = 0;

	/**
		Insert new attribute info object to collection of attributes.
		\param	elementId		unique ID of new element (component).
	*/
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true) = 0;

	/**
		Get access to stored attribute info structure.
	*/
	virtual const ElementInfo* GetElementInfo(const std::string& elementId) const = 0;

	/**
		Removes attribute info structure from this collection.
	*/
	virtual bool RemoveElementInfo(const std::string& elementId) = 0;

	/**
		Get access to information structore of exported interfaces.
	*/
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const = 0;

	/**
		Get access to map used to convert exported sub-components to internal sub-component ID's.
	*/
	virtual const ExportedComponentsMap& GetExportedComponentsMap() const = 0;

	/**
		Enable or disable exporting single or all interfaces of component.
		\param	elementId			ID of element in registry.
		\param	exportInterfaceInfo	specify interface. If it is invalid, all interfaces will be taken.
		\param	state				the export state. If \c false, the existing export will be removed.
	*/
	virtual void SetElementExported(
				const std::string& elementId,
				const istd::CClassInfo& exportInterfaceInfo = istd::CClassInfo(),
				bool state = true) = 0;

	/**
		Get human readable description of this registry.
	*/
	virtual const istd::CString& GetDescription() const = 0;

	/**
		set human readable description of this registry.
	*/
	virtual void SetDescription(const istd::CString& description) = 0;

	/**
		Get keywords for this registry used for search.
	*/
	virtual const istd::CString& GetKeywords() const = 0;

	/**
		Set keywords for this registry used for search.
	*/
	virtual void SetKeywords(const istd::CString& keywords) = 0;
};


}//namespace icomp


#endif // !icomp_IRegistry_included


