#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>
#include <QtCore/QMap>

// ACF includes
#include <istd/TDelPtr.h>
#include <iser/ISerializable.h>
#include <icomp/IRegistryElement.h>
#include <icomp/CComponentAddress.h>


namespace icomp
{


/**
	Interface representing information stored in component registry.
	Registry is a model representing smallest component topology unit.
	Registry can be imagined as a representation of an \c acc file.

	\par Thread-Safety Notes:
	The CRegistry implementation uses internal locking (QMutex/QRecursiveMutex) to protect
	internal data structures. However, callers must be aware of the following:
	
	- GetExportedInterfacesMap() and GetExportedElementsMap() return const references to
	  internal maps. While the reference is returned under lock protection, the lock is
	  released when the function returns. Callers should make a copy if they need to
	  iterate or store the map to avoid race conditions:
	  \code
	  // Thread-safe pattern - make a copy
	  auto mapCopy = registry.GetExportedInterfacesMap();
	  for (auto iter = mapCopy.begin(); iter != mapCopy.end(); ++iter) { ... }
	  \endcode
	
	- InsertElementInfo() returns a pointer to an element in the internal map. This pointer
	  is only safe to use if the caller ensures no concurrent modifications occur. When
	  called from within other registry methods (like Serialize), the pointer is safe because
	  the calling method holds the lock.

	\ingroup ComponentConcept
*/
class IRegistry: virtual public iser::ISerializable
{
public:
	typedef QSet<QByteArray> Ids;
	typedef istd::TDelPtr<IRegistryElement> ElementPtr;

	/**
		Change flags.
	*/
	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 0xf72390,
		CF_ELEMENT_REMOVED,
		CF_ELEMENT_EXPORTED,
		CF_ELEMENT_RENAMED,
		CF_EMBEDDED
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
	typedef QMap<QByteArray, QByteArray> ExportedInterfacesMap;

	/**
		Map assigning exported sub-component names to internal subcomponent ID's.
	*/
	typedef QMap<QByteArray, QByteArray> ExportedElementsMap;

	/**
		Get ID list of existing elements.
	*/
	virtual Ids GetElementIds() const = 0;

	/**
		Get access to stored attribute info structure.
	*/
	virtual const ElementInfo* GetElementInfo(const QByteArray& elementId) const = 0;

	/**
		Insert new attribute info object to collection of attributes.
		\param	elementId				unique ID of new element (component).
		\param	address					address describing element type.
		\param	ensureElementCreated	if true new element will be created.
	*/
	virtual ElementInfo* InsertElementInfo(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true) = 0;

	/**
		Removes attribute info structure from this collection.
	*/
	virtual bool RemoveElementInfo(const QByteArray& elementId) = 0;

	/**
		Rename an registry element.
		\param	oldElementId		ID of element to be renamed.
		\param	newElementId		the new ID of element.
	*/
	virtual bool RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId) = 0;

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
	virtual IRegistry* GetEmbeddedRegistry(const QByteArray& registryId) const = 0;

	/**
		Insert embedded registry with specified ID.
		\return	instance of new created embedded registry.
	*/
	virtual IRegistry* InsertEmbeddedRegistry(const QByteArray& registryId) = 0;

	/**
		Remove embedded registry with specified ID.
		\return	true, if registry could be removed.
	*/
	virtual bool RemoveEmbeddedRegistry(const QByteArray& registryId) = 0;

	/**
		Rename a embedded registry.
		\param	oldElementId		ID of element to be renamed.
		\param	newElementId		the new ID of element.
	*/
	virtual bool RenameEmbeddedRegistry(const QByteArray& oldRegistryId, const QByteArray& newRegistryId) = 0;

	/**
		Get access to information structure of exported interfaces.
	*/
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const = 0;

	/**
		Get access to map used to convert exported sub-elements to internal sub-element ID's.
	*/
	virtual const ExportedElementsMap& GetExportedElementsMap() const = 0;

	/**
		Enable or disable exporting single or all interfaces of element.
		\param	elementId			ID of element in registry.
		\param	exportInterfaceInfo	specify interface. If must be invalid.
		\param	state				the export state. If \c false, the existing export will be removed.
	*/
	virtual void SetElementInterfaceExported(
				const QByteArray& elementId,
				const QByteArray& interfaceName,
				bool state = true) = 0;

	/**
		Set element to be exported.
		\param	exportId		ID of exported name, or NULL if this export should be removed.
		\param	elementId		ID of element or its subelement (separated with slash) or empty string, if this export should be removed.
	*/
	virtual void SetElementExported(
				const QByteArray& exportId,
				const QByteArray& elementId) = 0;

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


