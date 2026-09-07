// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include <icomp/IRegistry.h>
#include <icomp/IPackagesManager.h>
#include <icomp/IRegistriesManager.h>
#include <ilog/TLoggerCompWrap.h>


namespace ipackage
{


/**
	Validates structural consistency of a registry.

	This component checks that all component references, embedded registries,
	and internal element references within a registry are valid and resolvable.
	It reports any inconsistencies via the logging system.

	This is a simplified consistency checker suitable for use in the Arxc compiler
	context, where loaded DLL packages are not available. It validates:
	- All referenced packages are defined.
	- All composite component registries can be resolved.
	- All embedded component registries exist.
	- All reference/factory attributes point to existing elements in the registry.
*/
class CRegistryConsistInfoComp: public ilog::CLoggerComponentBase
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_UNDEFINED_PACKAGE = 0x819f0,
		MI_UNDEFINED_COMPONENT,
		MI_UNDEFINED_ELEMENT,
		MI_BAD_REFERENCE,
		MI_REFERENCE_TO_MISSING_ELEMENT
	};

	I_BEGIN_COMPONENT(CRegistryConsistInfoComp);
		I_REGISTER_INTERFACE(istd::IPolymorphic);
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager providing access to package information", true, "PackagesManager");
		I_ASSIGN(m_registriesManagerCompPtr, "RegistriesManager", "Registries manager providing access to all composite component registries", true, "RegistriesManager");
	I_END_COMPONENT;

	/**
		Check if the given registry is structurally consistent.
		\param registry the registry to validate.
		\return true if the registry is consistent, false if errors were found.
	*/
	bool IsRegistryConsistent(const icomp::IRegistry& registry) const;

protected:
	/**
		Validate all elements in a registry and their sub-registries recursively.
		\param registry the registry to validate.
		\param registryPath path for diagnostics messages.
		\param visitedAddresses set of already visited composite addresses (to detect cycles).
		\return true if all elements are consistent.
	*/
	bool ValidateElements(
				const icomp::IRegistry& registry,
				const QStringList& registryPath,
				QSet<icomp::CComponentAddress>& visitedAddresses) const;

	/**
		Validate that internal references in a registry element point to existing elements.
		\param elementId ID of the element being checked.
		\param registry the containing registry.
		\param registryPath path for diagnostics messages.
		\return true if all references are valid.
	*/
	bool ValidateElementReferences(
				const QByteArray& elementId,
				const icomp::IRegistry& registry,
				const QStringList& registryPath) const;

	/**
		Check if a referenced element name can be resolved within the registry.
		\param referencedElementName the element name referenced by an attribute.
		\param registry the containing registry.
		\return true if the referenced element exists.
	*/
	bool IsElementResolvable(
				const QByteArray& referencedElementName,
				const icomp::IRegistry& registry) const;

private:
	I_REF(icomp::IPackagesManager, m_packagesManagerCompPtr);
	I_REF(icomp::IRegistriesManager, m_registriesManagerCompPtr);
};


} // namespace ipackage


