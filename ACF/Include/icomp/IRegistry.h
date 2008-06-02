#ifndef icomp_IRegistry_included
#define icomp_IRegistry_included


#include <string>
#include <set>
#include <map>

#include "istd/TDelPtr.h"

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
		Map assigning interface ID to component ID exporting this interface.
	*/
	typedef std::map< std::string, std::string> ExportedInterfacesMap;
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
		Export interface(s) of component \elementId. If \c doExport is \c false, the existing export will be removed.
	*/
	virtual void ExportElementInterface(const std::string& elementId, bool doExport = true) = 0;
};


}//namespace icomp


#endif // !icomp_IRegistry_included


