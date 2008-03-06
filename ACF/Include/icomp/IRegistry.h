#ifndef icomp_IRegistry_included
#define icomp_IRegistry_included


#include <string>
#include <set>
#include <map>

#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"

#include "icomp/IRegistryElement.h"


namespace icomp
{


class IRegistry: virtual public iser::ISerializable
{
public:
	typedef ::std::set<::std::string> Ids;
	typedef istd::TDelPtr<IRegistryElement> ElementPtr;

	/**
		Type of element.
	*/
	enum ElementType
	{
		ET_NONE,
		ET_COMPONENT,	///<	Normal component loaded from dynamic link library.
		ET_COMPOSITION	///<	Composition of components used as single component.
	};

	/**
		Describe information stored with each element.
	*/
	struct ElementInfo
	{
		ElementPtr elementPtr;		///<	Pointer to registry element
		int elementType;			///<	Type of element. \sa ElementType
		::std::string packageId;	///<	ID of package. It can be for example part of dynamic link library storing set of component or folder name with composed components
		::std::string factoryId;	///<	ID of component factory inside package.
	};

	/**
		Information for exported interface.
	*/
	struct InterfaceInfo
	{
		::std::string componentId;
		::std::string subId;
	};

	typedef ::std::map<::std::string, InterfaceInfo> InterfaceInfos;
	/**
		Map assigning exported sub-component names to internal subcomponent ID's.
	*/
	typedef ::std::map<::std::string, ::std::string> SubcomponentMap;

	/**
		Get ID list of existing elements.
	*/
	virtual Ids GetElementIds() const = 0;

	/**
		Insert new attribute info object to collection of attributes.
		\param	attributeId		unique ID of new attribute.
	*/
	virtual ElementInfo* InsertElementInfo(
				const ::std::string& elementId,
				int elementType,
				const ::std::string& packageId,
				const ::std::string& factoryId,
				bool ensureElementCreated = true) = 0;

	/**
		Get access to stored attribute info structure.
	*/
	virtual const ElementInfo* GetElementInfo(const ::std::string& elementId) const = 0;

	/**
		Removes attribute info structure from this collection.
	*/
	virtual bool RemoveElementInfo(const ::std::string& elementId) = 0;

	/**
		Get access to information structore of exported interfaces.
	*/
	virtual const InterfaceInfos& GetExportedInterfaceInfos() const = 0;

	/**
		Get access to map used to convert exported sub-components to internal sub-component ID's.
	*/
	virtual const SubcomponentMap& GetExportedSubcomponentMap() const = 0;
};


}//namespace icomp


#endif // !icomp_IRegistry_included


