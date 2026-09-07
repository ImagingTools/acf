// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ipackage/CRegistryConsistInfoComp.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CIdManipBase.h>
#include <icomp/CReferenceAttribute.h>
#include <icomp/CMultiReferenceAttribute.h>


namespace ipackage
{


// public methods

bool CRegistryConsistInfoComp::IsRegistryConsistent(const icomp::IRegistry& registry) const
{
	QSet<icomp::CComponentAddress> visitedAddresses;

	return ValidateElements(registry, QStringList(), visitedAddresses);
}


// protected methods

bool CRegistryConsistInfoComp::ValidateElements(
			const icomp::IRegistry& registry,
			const QStringList& registryPath,
			QSet<icomp::CComponentAddress>& visitedAddresses) const
{
	if (!m_packagesManagerCompPtr.IsValid() || !m_registriesManagerCompPtr.IsValid()){
		SendErrorMessage(
					MI_UNDEFINED_PACKAGE,
					QObject::tr("Consistency check cannot be performed: required manager references are not set"));
		return false;
	}

	bool retVal = true;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (	icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
			elementIter != ids.end();
			++elementIter){
		const QByteArray& elementId = *elementIter;

		QStringList elementPath = registryPath;
		elementPath << elementId;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementId);
		Q_ASSERT(infoPtr != NULL);

		const QByteArray& packageId = infoPtr->address.GetPackageId();

		if (!packageId.isEmpty()){
			int packageType = m_packagesManagerCompPtr->GetPackageType(packageId);

			switch (packageType){
			case icomp::IPackagesManager::PT_COMPOSED:
				{
					if (visitedAddresses.contains(infoPtr->address)){
						break;	// already visited, skip to avoid infinite recursion
					}

					const icomp::IRegistry* subRegistryPtr = m_registriesManagerCompPtr->GetRegistry(infoPtr->address);
					if (subRegistryPtr != NULL){
						visitedAddresses.insert(infoPtr->address);

						if (!ValidateElements(*subRegistryPtr, elementPath, visitedAddresses)){
							retVal = false;
						}
					}
					else{
						SendErrorMessage(
									MI_UNDEFINED_COMPONENT,
									QObject::tr("%1: Composite component '%2' is undefined")
												.arg(elementPath.join("/"))
												.arg(infoPtr->address.ToString()));

						retVal = false;
					}
				}
				break;

			case icomp::IPackagesManager::PT_UNKNOWN:
			case icomp::IPackagesManager::PT_REAL:
				// Real or unknown packages are accepted
				break;

			default:
				SendErrorMessage(
							MI_UNDEFINED_PACKAGE,
							QObject::tr("%1: Package '%2' is undefined")
										.arg(elementPath.join("/"))
										.arg(QString(packageId)));
				retVal = false;
				break;
			}
		}
		else{
			const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(infoPtr->address.GetComponentId());
			if (embeddedRegistryPtr != NULL){
				if (!ValidateElements(*embeddedRegistryPtr, elementPath, visitedAddresses)){
					retVal = false;
				}
			}
			else{
				SendErrorMessage(
							MI_UNDEFINED_COMPONENT,
							QObject::tr("%1: Embedded component '%2' is undefined")
										.arg(elementPath.join("/"))
										.arg(infoPtr->address.ToString()));

				retVal = false;
			}
		}

		// Validate internal element references
		if (!ValidateElementReferences(elementId, registry, registryPath)){
			retVal = false;
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::ValidateElementReferences(
			const QByteArray& elementId,
			const icomp::IRegistry& registry,
			const QStringList& registryPath) const
{
	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementId);
	if (infoPtr == NULL || !infoPtr->elementPtr.IsValid()){
		return true;	// no element info, nothing to validate
	}

	bool retVal = true;

	iattr::IAttributesProvider::AttributeIds attributeIds = infoPtr->elementPtr->GetAttributeIds();
	for (	iattr::IAttributesProvider::AttributeIds::ConstIterator attrIter = attributeIds.constBegin();
			attrIter != attributeIds.constEnd();
			++attrIter){
		const QByteArray& attributeId = *attrIter;

		const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = infoPtr->elementPtr->GetAttributeInfo(attributeId);
		if (attrInfoPtr == NULL || !attrInfoPtr->attributePtr.IsValid()){
			continue;
		}

		QStringList elementPath = registryPath;
		elementPath << elementId;
		QString elementPathStr = elementPath.join("/");

		// Check single reference attributes
		const icomp::CReferenceAttribute* refPtr = dynamic_cast<const icomp::CReferenceAttribute*>(attrInfoPtr->attributePtr.GetPtr());
		if (refPtr != NULL){
			const QByteArray& referencedId = refPtr->GetValue();
			if (!referencedId.isEmpty() && !IsElementResolvable(referencedId, registry)){
				SendErrorMessage(
							MI_REFERENCE_TO_MISSING_ELEMENT,
							QObject::tr("%1: Attribute '%2' references element '%3' which does not exist in the registry")
										.arg(elementPathStr)
										.arg(QString(attributeId))
										.arg(QString(referencedId)));
				retVal = false;
			}
		}

		// Check multi-reference attributes
		const icomp::CMultiReferenceAttribute* multiRefPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attrInfoPtr->attributePtr.GetPtr());
		if (multiRefPtr != NULL){
			int count = multiRefPtr->GetValuesCount();
			for (int i = 0; i < count; ++i){
				const QByteArray& referencedId = multiRefPtr->GetValueAt(i);
				if (!referencedId.isEmpty() && !IsElementResolvable(referencedId, registry)){
					SendErrorMessage(
								MI_REFERENCE_TO_MISSING_ELEMENT,
								QObject::tr("%1: Attribute '%2' references element '%3' which does not exist in the registry")
											.arg(elementPathStr)
											.arg(QString(attributeId))
											.arg(QString(referencedId)));
					retVal = false;
				}
			}
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsElementResolvable(
			const QByteArray& referencedElementName,
			const icomp::IRegistry& registry) const
{
	QByteArray baseId;
	QByteArray subId;
	istd::CIdManipBase::SplitId(referencedElementName, baseId, subId);

	const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(baseId);

	return (elementInfoPtr != NULL);
}


} // namespace ipackage


