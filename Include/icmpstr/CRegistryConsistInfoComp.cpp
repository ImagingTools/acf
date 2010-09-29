#include "icmpstr/CRegistryConsistInfoComp.h"


// Qt includes
#include <QDir>

#include "ibase/CMessage.h"


namespace icmpstr
{


// reimplemented (icmpstr::IRegistryConsistInfo)

icomp::IRegistry::Ids CRegistryConsistInfoComp::GetCompatibleElements(
			const istd::CClassInfo& interfaceInfo,
			const icomp::IRegistry& registry,
			bool includeUndefined) const
{
	icomp::IRegistry::Ids retVal;

	if (m_envManagerCompPtr.IsValid()){
		icomp::IRegistry::Ids elementIds = registry.GetElementIds();
		for (		icomp::IRegistry::Ids::const_iterator index = elementIds.begin();
					index != elementIds.end();
					index++){
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(*index);
			I_ASSERT(elementInfoPtr != NULL);	// element ID was taken from this registry, it must exist

			const icomp::IComponentStaticInfo* infoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementInfoPtr->address);

			if (infoPtr != NULL){
				icomp::IRegistry::Ids subIds = GetCompatibleSubcomponents(
							*index,
							*infoPtr,
							interfaceInfo);

				retVal.insert(subIds.begin(), subIds.end());
			}
			else if (includeUndefined){
				retVal.insert(*index);
			}
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
	bool retVal = true;

	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementName);
	if (infoPtr != NULL){
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(infoPtr->address);
		if (metaInfoPtr != NULL){
			const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = metaInfoPtr->GetAttributeInfos();
			for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
				const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);

				bool status = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr);

				if (!status && !allReasons){
					return false;
				}

				retVal = retVal && status;
			}
		}
		else{
			if (!ignoreUndef){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(new ibase::CMessage(
								istd::ILogger::MC_WARNING,
								MI_COMPONENT_INACTIVE,
								iqt::GetCString(QObject::tr("Element %1 uses inactive component %2")
											.arg(elementName.c_str())
											.arg(iqt::GetQString(GetAddressName(infoPtr->address)))),
								iqt::GetCString(QObject::tr("Element Consistency Check")),
								0));
				}

				if (allReasons){
					retVal = false;
				}
				else{
					return false;
				}
			}
		}

		const icomp::IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
		if (elementPtr != NULL){
			if (metaInfoPtr == NULL){	// if no meta info, we try to check attributes from existing attributes list
				icomp::IRegistryElement::Ids ids = elementPtr->GetAttributeIds();

				for (		icomp::IRegistryElement::Ids::const_iterator iter = ids.begin();
							iter != ids.end();
							++iter){
					const std::string& attributeId = *iter;

					bool status = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr);

					if (!status && !allReasons){
						return false;
					}

					retVal = retVal && status;
				}
			}
		}
		else{
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(new ibase::CMessage(
							istd::ILogger::MC_WARNING,
							MI_COMPONENT_INACTIVE,
							iqt::GetCString(QObject::tr("Element %1 is not loaded").arg(elementName.c_str())),
							iqt::GetCString(QObject::tr("Element Consistency Check")),
							0));
			}

			if (allReasons){
				retVal = false;
			}
			else{
				return false;
			}
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(new ibase::CMessage(
						istd::ILogger::MC_ERROR,
						MI_NO_ELEMENT_INFO,
						iqt::GetCString(QObject::tr("No element info available for %1").arg(elementName.c_str())),
						iqt::GetCString(QObject::tr("Element Consistency Check")),
						0));
		}

		if (allReasons){
			retVal = false;
		}
		else{
			return false;
		}
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

		if (infoPtr->elementPtr.IsValid()){
			const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = infoPtr->elementPtr->GetAttributeInfo(attributeName);

			if (compInfoPtr != NULL){
				const icomp::IComponentStaticInfo::AttributeInfos& attrInfos = compInfoPtr->GetAttributeInfos();
				const icomp::IComponentStaticInfo::AttributeInfos::ValueType* attrMetaInfoPtr2 = attrInfos.FindElement(attributeName);

				if (attrMetaInfoPtr2 != NULL){
					if (*attrMetaInfoPtr2 != NULL){
						if (attrInfoPtr != NULL){
							if ((*attrMetaInfoPtr2)->GetAttributeTypeName() != attrInfoPtr->attributeTypeName){
								if (reasonConsumerPtr != NULL){
									reasonConsumerPtr->AddMessage(new ibase::CMessage(
												istd::ILogger::MC_ERROR,
												MI_BAD_ATTRIBUTE_TYPE,
												iqt::GetCString(QObject::tr("Attribute %1 in %2 is defined as %3, but in registry it has type %4")
															.arg(attributeName.c_str())
															.arg(elementName.c_str())
															.arg((*attrMetaInfoPtr2)->GetAttributeTypeName().c_str())
															.arg(attrInfoPtr->attributeTypeName.c_str())),
												iqt::GetCString(QObject::tr("Attribute Consistency Check")),
												0));
								}

								return false;
							}

							if (attrInfoPtr->attributePtr.IsValid()){
								if (!CheckAttributeCompatibility(
											*attrInfoPtr->attributePtr,
											**attrMetaInfoPtr2,
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
										(*attrMetaInfoPtr2)->IsObligatory() &&
										!(*attrMetaInfoPtr2)->GetRelatedInterfaceType().IsVoid()){
								if (reasonConsumerPtr != NULL){
									reasonConsumerPtr->AddMessage(new ibase::CMessage(
												istd::ILogger::MC_ERROR,
												MI_REF_NOT_RESOLVED,
												iqt::GetCString(QObject::tr("Reference of factory %1 in %2 is undefined and it is obligatory")
															.arg(attributeName.c_str())
															.arg(elementName.c_str())),
												iqt::GetCString(QObject::tr("Attribute Consistency Check")),
												0));
								}

								return false;
							}
						}
						else if (	(*attrMetaInfoPtr2)->IsObligatory() &&
									!(*attrMetaInfoPtr2)->GetRelatedInterfaceType().IsVoid()){
							if (reasonConsumerPtr != NULL){
								reasonConsumerPtr->AddMessage(new ibase::CMessage(
											istd::ILogger::MC_ERROR,
											MI_REF_NOT_RESOLVED,
											iqt::GetCString(QObject::tr("Reference of factory %1 in %2 is undefined and it is obligatory")
														.arg(attributeName.c_str())
														.arg(elementName.c_str())),
											iqt::GetCString(QObject::tr("Attribute Consistency Check")),
											0));
							}

							return false;
						}
					}
					else if (!ignoreUndef){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(new ibase::CMessage(
										istd::ILogger::MC_WARNING,
										MI_UNDEF_ATTRIBUTE,
										iqt::GetCString(QObject::tr("Attribute %1 in %2 is not enabled in component specification")
													.arg(attributeName.c_str())
													.arg(elementName.c_str())),
										iqt::GetCString(QObject::tr("Attribute Consistency Check")),
										0));
						}

						return false;
					}
				}
				else{
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_UNDEF_ATTRIBUTE,
									iqt::GetCString(QObject::tr("Attribute %1 in %2 not exists in component specification")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())),
									iqt::GetCString(QObject::tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}
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
			const istd::CClassInfo& interfaceInfo) const
{
	icomp::IRegistry::Ids retVal;

	static istd::CClassInfo compTypeInfo(typeid(icomp::IComponent));
	if (interfaceInfo.IsVoid() || (interfaceInfo == compTypeInfo)){
		retVal.insert(elementId);
	}
	else{
		const icomp::IComponentStaticInfo::InterfaceExtractors& interfaceExtractors = elementStaticInfo.GetInterfaceExtractors();
		const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceInfo);
		if (extractorPtr != NULL){
			retVal.insert(elementId);
		}
	}

	const icomp::IComponentStaticInfo::Ids subcomponentIds = elementStaticInfo.GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
				subIter != subcomponentIds.end();
				++subIter){
		const std::string& subcomponentId = *subIter;

		const icomp::IComponentStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubcomponentInfo(subcomponentId);
		if (subcomponentInfoPtr != NULL){
			icomp::IRegistry::Ids subIds = GetCompatibleSubcomponents(icomp::CInterfaceManipBase::JoinId(elementId, subcomponentId), *subcomponentInfoPtr, interfaceInfo);

			retVal.insert(subIds.begin(), subIds.end());
		}
	}

	return retVal;
}


istd::CString CRegistryConsistInfoComp::GetAddressName(const icomp::CComponentAddress& address) const
{
	return address.GetPackageId() + "/" + address.GetComponentId();
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
		const istd::CClassInfo& interfaceInfo = attributeMetaInfo.GetRelatedInterfaceType();
		const std::string& componentId = idPtr->GetValue();

		if (!CheckPointedElementCompatibility(
					componentId,
					interfaceInfo,
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
		const istd::CClassInfo& interfaceInfo = attributeMetaInfo.GetRelatedInterfaceType();
		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			const std::string& componentId = multiIdPtr->GetValueAt(idIndex);

			if (!CheckPointedElementCompatibility(
						componentId,
						interfaceInfo,
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
			const istd::CClassInfo& interfaceInfo,
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
			const icomp::IRegistry* pointedRegistryPtr = m_envManagerCompPtr->GetRegistry(pointedInfoPtr->address);
			if (pointedRegistryPtr != NULL){
				const icomp::IRegistry::ExportedComponentsMap& exportedMap = pointedRegistryPtr->GetExportedComponentsMap();
				icomp::IRegistry::ExportedComponentsMap::const_iterator exportIter = exportedMap.find(subId);
				if (exportIter == exportedMap.end()){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' point at '%3', but subelement '%4' doesn't exist")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementName.c_str())
												.arg(subId.c_str())),
									iqt::GetCString(QObject::tr("Attribute Consistency Check")),
									0));
					}

					return false;
				}

				const std::string localId = exportIter->second;
				if (!CheckPointedElementCompatibility(localId, interfaceInfo, attributeName, elementName, *pointedRegistryPtr, ignoreUndef, NULL)){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' point at '%3', but exported element '%4' is not compatible")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementName.c_str())
												.arg(localId.c_str())),
									iqt::GetCString(QObject::tr("Attribute Consistency Check")),
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
								iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' point at '%3', but element '%4' is not composited or cannot be loaded")
											.arg(attributeName.c_str())
											.arg(elementName.c_str())
											.arg(pointedElementName.c_str())
											.arg(baseId.c_str())),
								iqt::GetCString(QObject::tr("Attribute Consistency Check")),
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
							iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' contains '%3', but element '%4' doesn't exist")
										.arg(attributeName.c_str())
										.arg(elementName.c_str())
										.arg(pointedElementName.c_str())
										.arg(baseId.c_str())),
							iqt::GetCString(QObject::tr("Attribute Consistency Check")),
							0));
			}

			return false;
		}
	}
	else{
		const icomp::IRegistry::ElementInfo* pointedInfoPtr = registry.GetElementInfo(pointedElementName);
		if (pointedInfoPtr != NULL){
			static istd::CClassInfo compTypeInfo(typeid(icomp::IComponent));
			if (interfaceInfo.IsVoid() || (interfaceInfo == compTypeInfo)){
				return true;
			}

			const icomp::IComponentStaticInfo* pointedMetaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(pointedInfoPtr->address);
			if (pointedMetaInfoPtr != NULL){
				const icomp::IComponentStaticInfo::InterfaceExtractors& extractors = pointedMetaInfoPtr->GetInterfaceExtractors();
				if (extractors.FindElement(interfaceInfo) == NULL){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(new ibase::CMessage(
									istd::ILogger::MC_ERROR,
									MI_WRONG_INTERFACE,
									iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' point at '%3', but it doesn't implement interface %4")
												.arg(attributeName.c_str())
												.arg(elementName.c_str())
												.arg(pointedElementName.c_str())
												.arg(interfaceInfo.GetName().c_str())),
									iqt::GetCString(QObject::tr("Attribute Consistency Check")),
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
								iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' point at '%3', but element '%4' is not composited or cannot be loaded")
											.arg(attributeName.c_str())
											.arg(elementName.c_str())
											.arg(pointedElementName.c_str())
											.arg(baseId.c_str())),
								iqt::GetCString(QObject::tr("Attribute Consistency Check")),
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
							iqt::GetCString(QObject::tr("Reference of factory '%1' in '%2' contains '%3', but this element doesn't exist")
										.arg(attributeName.c_str())
										.arg(elementName.c_str())
										.arg(pointedElementName.c_str())),
							iqt::GetCString(QObject::tr("Attribute Consistency Check")),
							0));
			}

			return false;
		}
	}

	return true;
}


} // namespace icmpstr


