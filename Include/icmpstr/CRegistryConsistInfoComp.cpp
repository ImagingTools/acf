#include "icmpstr/CRegistryConsistInfoComp.h"


// Qt includes
#include <QDir>


// ACF includes
#include "icomp/CCompositeComponentStaticInfo.h"

#include "ibase/CMessage.h"


namespace icmpstr
{


// reimplemented (icmpstr::IRegistryConsistInfo)

icomp::IRegistry::Ids CRegistryConsistInfoComp::GetCompatibleElements(
			const icomp::IComponentStaticInfo::Ids& interfaceNames,
			const icomp::IRegistry& registry,
			int queryFlags) const
{
	icomp::IRegistry::Ids retVal;

	bool includeUndefined = (queryFlags & QF_INCLUDE_UNDEFINED) != 0;

	if (m_envManagerCompPtr.IsValid()){
		icomp::IRegistry::Ids elementIds = registry.GetElementIds();
		for (		icomp::IRegistry::Ids::const_iterator index = elementIds.begin();
					index != elementIds.end();
					index++){
			const std::string& elementId = *index;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
			I_ASSERT(elementInfoPtr != NULL);	// element ID was taken from this registry, it must exist

			icomp::IRegistry::Ids subIds;

			const icomp::CComponentAddress& elementAddress = elementInfoPtr->address;
			if (!elementAddress.GetPackageId().empty()){
				const icomp::IComponentStaticInfo* infoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementAddress);

				if (infoPtr != NULL){
					subIds = GetCompatibleSubcomponents(
								elementId,
								*infoPtr,
								interfaceNames);
				}
				else if (includeUndefined){
					retVal.insert(elementId);
				}
			}
			else{
				const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(elementAddress.GetComponentId());
				if (embeddedRegistryPtr == NULL){
					if (includeUndefined){
						retVal.insert(elementId);
					}

					continue;
				}

				icomp::CCompositeComponentStaticInfo info(*embeddedRegistryPtr, *m_envManagerCompPtr);
				subIds = GetCompatibleSubcomponents(
							elementId,
							info,
							interfaceNames);
			}

			retVal.insert(subIds.begin(), subIds.end());
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsRegistryValid(
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	bool retVal = true;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		const std::string& elementId = *iter;

		bool status = IsElementValid(elementId, registry, ignoreUndef, allReasons, reasonConsumerPtr);

		if (!status && !allReasons){
			return false;
		}

		retVal = retVal && status;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsElementValid(
			const std::string& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	I_ASSERT(m_envManagerCompPtr.IsValid());

	bool retVal = true;

	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementName);
	if (infoPtr != NULL){
		const icomp::CComponentAddress& elementAddress = infoPtr->address;
		if (!elementAddress.GetPackageId().empty()){
			retVal = IsElementWithInfoValid(
						elementName,
						*infoPtr,
						m_envManagerCompPtr->GetComponentMetaInfo(elementAddress),
						registry,
						ignoreUndef,
						allReasons,
						reasonConsumerPtr) && retVal;
		}
		else{
			icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(elementAddress.GetComponentId());
			if (embeddedRegistryPtr != NULL){
				icomp::CCompositeComponentStaticInfo embeddedInfo(*embeddedRegistryPtr, *m_envManagerCompPtr);
				retVal = IsElementWithInfoValid(
							elementName,
							*infoPtr,
							&embeddedInfo,
							registry,
							ignoreUndef,
							allReasons,
							reasonConsumerPtr) && retVal;
			}
			else{
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(new ibase::CMessage(
								istd::ILogger::MC_WARNING,
								MI_COMPONENT_INACTIVE,
								iqt::GetCString(tr("Element %1 uses unknown embedded composite component %2").arg(elementName.c_str()).arg(elementAddress.GetComponentId().c_str())),
								iqt::GetCString(tr("Element Consistency Check")),
								0));
				}

				retVal = false;
			}
		}

		if (!retVal && !allReasons){
			return false;
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(new ibase::CMessage(
						istd::ILogger::MC_ERROR,
						MI_NO_ELEMENT_INFO,
						iqt::GetCString(tr("No element info available for %1").arg(elementName.c_str())),
						iqt::GetCString(tr("Element Consistency Check")),
						0));
		}

		retVal = false;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsElementWithInfoValid(
			const std::string& elementName,
			const icomp::IRegistry::ElementInfo& elementInfo,
			const icomp::IComponentStaticInfo* metaInfoPtr,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	bool retVal = true;

	if (metaInfoPtr != NULL){
		icomp::IComponentStaticInfo::Ids attributeIds = metaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_ATTRIBUTES);
		for (		icomp::IComponentStaticInfo::Ids::const_iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const std::string& attributeId = *attrIter;

			retVal = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr) && retVal;

			if (!retVal && !allReasons){
				return false;
			}
		}
	}
	else{
		if (!ignoreUndef){
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(new ibase::CMessage(
							istd::ILogger::MC_WARNING,
							MI_COMPONENT_INACTIVE,
							iqt::GetCString(tr("Element %1 uses inactive component %2")
										.arg(elementName.c_str())
										.arg(iqt::GetQString(elementInfo.address.ToString()))),
							iqt::GetCString(tr("Element Consistency Check")),
							0));
			}

			retVal = false;

			if (!allReasons){
				return false;
			}
		}
	}

	const icomp::IRegistryElement* elementPtr = elementInfo.elementPtr.GetPtr();
	if (elementPtr != NULL){
		if (metaInfoPtr == NULL){	// if no meta info, we try to check attributes from existing attributes list
			icomp::IRegistryElement::Ids ids = elementPtr->GetAttributeIds();

			for (		icomp::IRegistryElement::Ids::const_iterator iter = ids.begin();
						iter != ids.end();
						++iter){
				const std::string& attributeId = *iter;

				retVal = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr) && retVal;

				if (!retVal && !allReasons){
					return false;
				}
			}
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(new ibase::CMessage(
						istd::ILogger::MC_WARNING,
						MI_COMPONENT_INACTIVE,
						iqt::GetCString(tr("Element %1 is not loaded").arg(elementName.c_str())),
						iqt::GetCString(tr("Element Consistency Check")),
						0));
		}

		return false;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsAttributeValid(
			const std::string& attributeName,
			const std::string& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementName);
	if (infoPtr != NULL){
		const icomp::IComponentStaticInfo* compInfoPtr = NULL;
		if (m_envManagerCompPtr.IsValid()){
			compInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(infoPtr->address);
		}

		if (infoPtr->elementPtr.IsValid() && (compInfoPtr != NULL)){
			const icomp::IAttributeStaticInfo* attrMetaInfoPtr = compInfoPtr->GetAttributeInfo(attributeName);
			if (attrMetaInfoPtr != NULL){
				const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = infoPtr->elementPtr->GetAttributeInfo(attributeName);
				if (attrInfoPtr != NULL){
					if (attrMetaInfoPtr->GetAttributeTypeName() != attrInfoPtr->attributeTypeName){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(new ibase::CMessage(
										istd::ILogger::MC_ERROR,
										MI_BAD_ATTRIBUTE_TYPE,
										iqt::GetCString(tr("Attribute %1 in %2 is defined as %3, but in registry it has type %4")
													.arg(attributeName.c_str())
													.arg(elementName.c_str())
													.arg(attrMetaInfoPtr->GetAttributeTypeName().c_str())
													.arg(attrInfoPtr->attributeTypeName.c_str())),
										iqt::GetCString(tr("Attribute Consistency Check")),
										0));
						}

						return false;
					}

					if (attrInfoPtr->attributePtr.IsValid()){
						if (!CheckAttributeCompatibility(
									*attrInfoPtr->attributePtr,
									*attrMetaInfoPtr,
									attributeName,
									elementName,
									registry,
									ignoreUndef,
									allReasons,
									reasonConsumerPtr)){
							return false;
						}
					}
					else if (	attrInfoPtr->exportId.empty() &&
								((attrMetaInfoPtr->GetAttributeFlags() & icomp::IAttributeStaticInfo::AF_NULLABLE) == 0)){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(new ibase::CMessage(
										istd::ILogger::MC_ERROR,
										MI_REF_NOT_RESOLVED,
										iqt::GetCString(tr("Reference or factory %1 in %2 cannot be undefined")
													.arg(attributeName.c_str())
													.arg(elementName.c_str())),
										iqt::GetCString(tr("Attribute Consistency Check")),
										0));
						}

						return false;
					}
				}
				else if ((attrMetaInfoPtr->GetAttributeFlags() & icomp::IAttributeStaticInfo::AF_NULLABLE) == 0){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_REF_NOT_RESOLVED,
									iqt::GetCString(tr("Reference or factory %1 in %2 cannot be undefined")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())),
									iqt::GetCString(tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}
			}
			else if (!ignoreUndef){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(new ibase::CMessage(
								istd::ILogger::MC_ERROR,
								MI_UNDEF_ATTRIBUTE,
								iqt::GetCString(tr("Attribute %1 in %2 not exists in component specification")
											.arg(attributeName.c_str())
											.arg(elementName.c_str())),
								iqt::GetCString(tr("Attribute Consistency Check")),
								0));
				}

				return false;
			}
		}
	}

	return true;
}


QIcon CRegistryConsistInfoComp::GetComponentIcon(const icomp::CComponentAddress& address) const
{
	if (m_externalMetaInfoManagerCompPtr.IsValid()){
		istd::CString infoPath = m_externalMetaInfoManagerCompPtr->GetComponentInfoPath(address);
		if (!infoPath.IsEmpty()){
			IconCache::const_iterator cachedIter = m_iconCache.find(address);
			if (cachedIter != m_iconCache.end()){
				return cachedIter->second;
			}
			else{
				QIcon& icon = m_iconCache[address];

				QDir packageDir(iqt::GetQString(infoPath));
				if (packageDir.exists("Image.png")){
					icon = QIcon(packageDir.filePath("Image.png"));
				}
				else if (packageDir.exists("Image.svg")){
					icon = QIcon(packageDir.filePath("Image.svg"));
				}

				return icon;
			}
		}
	}

	return QIcon();
}


// protected methods

icomp::IRegistry::Ids CRegistryConsistInfoComp::GetCompatibleSubcomponents(
			const std::string& elementId,
			const icomp::IComponentStaticInfo& elementStaticInfo,
			const icomp::IComponentStaticInfo::Ids& interfaceNames) const
{
	icomp::IRegistry::Ids retVal;

	bool areInterfacesSupported = true;
	for (		icomp::IComponentStaticInfo::Ids::const_iterator interfaceIter = interfaceNames.begin();
				interfaceIter != interfaceNames.end();
				++interfaceIter){
		const std::string& interfaceName = *interfaceIter;

		if ((interfaceName == "icomp::IComponent") || (interfaceName == "void")){
			continue;
		}

		icomp::IComponentStaticInfo::Ids supportedInterfaces = elementStaticInfo.GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);
		if (supportedInterfaces.find(interfaceName) == supportedInterfaces.end()){
			areInterfacesSupported = false;

			break;
		}
	}

	if (areInterfacesSupported){
		retVal.insert(elementId);
	}

	const icomp::IComponentStaticInfo::Ids subcomponentIds = elementStaticInfo.GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBCOMPONENTS);

	for (		icomp::IComponentStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
				subIter != subcomponentIds.end();
				++subIter){
		const std::string& subcomponentId = *subIter;

		const icomp::IComponentStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubcomponentInfo(subcomponentId);
		if (subcomponentInfoPtr != NULL){
			icomp::IRegistry::Ids subIds = GetCompatibleSubcomponents(
						icomp::CInterfaceManipBase::JoinId(elementId, subcomponentId),
						*subcomponentInfoPtr,
						interfaceNames);

			retVal.insert(subIds.begin(), subIds.end());
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::CheckAttributeCompatibility(
			const iser::IObject& attribute,
			const icomp::IAttributeStaticInfo& attributeMetaInfo,
			const std::string& attributeName,
			const std::string& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	const icomp::TAttribute<std::string>* idPtr = dynamic_cast<const icomp::TAttribute<std::string>*>(&attribute);
	if (idPtr != NULL){		
		icomp::IComponentStaticInfo::Ids interfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					0,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
		const std::string& componentId = idPtr->GetValue();

		if (!CheckPointedElementCompatibility(
					componentId,
					interfaceNames,
					attributeName,
					elementName,
					registry,
					ignoreUndef,
					reasonConsumerPtr)){
			return false;
		}
	}

	bool retVal = true;

	const icomp::TMultiAttribute<std::string>* multiIdPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		icomp::IComponentStaticInfo::Ids interfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					0,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			const std::string& componentId = multiIdPtr->GetValueAt(idIndex);

			if (!CheckPointedElementCompatibility(
						componentId,
						interfaceNames,
						attributeName,
						elementName,
						registry,
						ignoreUndef,
						reasonConsumerPtr)){
				if (!allReasons){
					return false;
				}
				else{
					retVal = false;
				}
			}
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::CheckPointedElementCompatibility(
			const std::string& pointedElementName,
			const icomp::IComponentStaticInfo::Ids& interfaceNames,
			const std::string& attributeName,
			const std::string& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	std::string baseId;
	std::string subId;
	if (istd::CIdManipBase::SplitId(pointedElementName, baseId, subId)){
		const icomp::IRegistry::ElementInfo* pointedInfoPtr = registry.GetElementInfo(baseId);
		if (pointedInfoPtr != NULL){
			const icomp::IRegistry* pointedRegistryPtr = m_envManagerCompPtr->GetRegistry(pointedInfoPtr->address, &registry);
			if (pointedRegistryPtr != NULL){
				const icomp::IRegistry::ExportedComponentsMap& exportedMap = pointedRegistryPtr->GetExportedComponentsMap();
				icomp::IRegistry::ExportedComponentsMap::const_iterator exportIter = exportedMap.find(subId);
				if (exportIter == exportedMap.end()){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									iqt::GetCString(tr("Reference or factory '%1' in '%2' point at '%3', but subelement '%4' doesn't exist")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementName.c_str())
												.arg(subId.c_str())),
									iqt::GetCString(tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}

				const std::string localId = exportIter->second;
				if (!CheckPointedElementCompatibility(
							localId,
							interfaceNames,
							attributeName,
							elementName,
							*pointedRegistryPtr,
							ignoreUndef,
							NULL)){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									iqt::GetCString(tr("Reference or factory '%1' in '%2' point at '%3', but exported element '%4' is not compatible")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementName.c_str())
												.arg(localId.c_str())),
									iqt::GetCString(tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}
			}
			else if (!ignoreUndef){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(new ibase::CMessage(
								istd::ILogger::MC_WARNING,
								MI_COMPOSITE_FOUND,
								iqt::GetCString(tr("Reference or factory '%1' in '%2' point at '%3', but element '%4' is not accessible in actual configuration")
											.arg(attributeName.c_str())
											.arg(elementName.c_str())
											.arg(pointedElementName.c_str())
											.arg(baseId.c_str())),
								iqt::GetCString(tr("Attribute Consistency Check")),
								0));
				}

				return false;
			}
		}
		else{
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(new ibase::CMessage(
							istd::ILogger::MC_ERROR,
							MI_COMPONENT_NOT_FOUND,
							iqt::GetCString(tr("Reference or factory '%1' in '%2' contains '%3', but element '%4' doesn't exist")
										.arg(attributeName.c_str())
										.arg(elementName.c_str())
										.arg(pointedElementName.c_str())
										.arg(baseId.c_str())),
							iqt::GetCString(tr("Attribute Consistency Check")),
							0));
			}

			return false;
		}
	}
	else{
		const icomp::IRegistry::ElementInfo* pointedInfoPtr = registry.GetElementInfo(pointedElementName);
		if (pointedInfoPtr != NULL){
			const icomp::CComponentAddress& pointedElementAddress = pointedInfoPtr->address;
			if (!pointedElementAddress.GetPackageId().empty()){
				const icomp::IComponentStaticInfo* pointedMetaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(pointedElementAddress);

				return CheckPointedElementInfoCompatibility(
							baseId,
							pointedMetaInfoPtr,
							interfaceNames,
							attributeName,
							elementName,
							ignoreUndef,
							reasonConsumerPtr);
			}
			else{
				icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(pointedElementAddress.GetComponentId());
				if (embeddedRegistryPtr != NULL){
					icomp::CCompositeComponentStaticInfo info(*embeddedRegistryPtr, *m_envManagerCompPtr);
					return CheckPointedElementInfoCompatibility(
								pointedElementName,
								&info,
								interfaceNames,
								attributeName,
								elementName,
								ignoreUndef,
								reasonConsumerPtr);
				}
				else{
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									iqt::GetCString(tr("Reference or factory '%1' in '%2' uses embedded type '%3', but this type is undefined")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementAddress.GetComponentId().c_str())),
									iqt::GetCString(tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}
			}
		}
		else{
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(new ibase::CMessage(
							istd::ILogger::MC_ERROR,
							MI_COMPONENT_NOT_FOUND,
							iqt::GetCString(tr("Reference or factory '%1' in '%2' contains '%3', but this element doesn't exist")
										.arg(attributeName.c_str())
										.arg(elementName.c_str())
										.arg(pointedElementName.c_str())),
							iqt::GetCString(tr("Attribute Consistency Check")),
							0));
			}

			return false;
		}
	}

	return true;
}


bool CRegistryConsistInfoComp::CheckPointedElementInfoCompatibility(
			const std::string& pointedElementName,
			const icomp::IComponentStaticInfo* pointedMetaInfoPtr,
			const icomp::IComponentStaticInfo::Ids& interfaceNames,
			const std::string& attributeName,
			const std::string& elementName,
			bool ignoreUndef,
			ibase::IMessageConsumer* reasonConsumerPtr) const
{
	if (pointedMetaInfoPtr != NULL){
		const icomp::IComponentStaticInfo::Ids& supportedInterfaces = pointedMetaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);
		for (		icomp::IComponentStaticInfo::Ids::const_iterator interfaceIter = interfaceNames.begin();
					interfaceIter != interfaceNames.end();
					++interfaceIter){
			const std::string& interfaceName = *interfaceIter;

			if ((interfaceName == "icomp::IComponent") || (interfaceName == "void")){
				continue;
			}

			if (supportedInterfaces.find(interfaceName) == supportedInterfaces.end()){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(new ibase::CMessage(
								istd::ILogger::MC_ERROR,
								MI_WRONG_INTERFACE,
								iqt::GetCString(tr("Reference or factory '%1' in '%2' point at '%3', but it doesn't implement interface %4")
											.arg(attributeName.c_str())
											.arg(elementName.c_str())
											.arg(pointedElementName.c_str())
											.arg(interfaceName.c_str())),
								iqt::GetCString(tr("Attribute Consistency Check")),
								0));
				}

				return false;
			}
		}
	}
	else if (!ignoreUndef){
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(new ibase::CMessage(
						istd::ILogger::MC_WARNING,
						MI_COMPOSITE_FOUND,
						iqt::GetCString(tr("Reference or factory '%1' in '%2' point at '%3', but it is not accessible in actual configuration")
									.arg(attributeName.c_str())
									.arg(elementName.c_str())
									.arg(pointedElementName.c_str())),
						iqt::GetCString(tr("Attribute Consistency Check")),
						0));
		}

		return false;
	}

	return true;
}


} // namespace icmpstr


