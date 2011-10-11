#include "icomp/CRegistryElement.h"


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TAttribute.h"
#include "icomp/TMultiAttribute.h"
#include "icomp/CReferenceAttribute.h"
#include "icomp/CFactoryAttribute.h"
#include "icomp/CMultiReferenceAttribute.h"
#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


CRegistryElement::CRegistryElement()
:	m_elementFlags(0)
{
}


IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const std::string& attributeId)
{
	AttributeInfoMap::iterator iter = m_attributeInfos.find(attributeId);
	if (iter != m_attributeInfos.end()){
		return &iter->second;
	}

	return NULL;
}


// reimplemented (icomp::IRegistryElement)

I_DWORD CRegistryElement::GetElementFlags() const
{
	return m_elementFlags;
}


void CRegistryElement::SetElementFlags(I_DWORD flags)
{
	if (flags != m_elementFlags){
		istd::CChangeNotifier notifier(this);

		m_elementFlags = flags;
	}
}


IRegistryElement::Ids CRegistryElement::GetAttributeIds() const
{
	Ids retVal;

	for (		AttributeInfoMap::const_iterator iter = m_attributeInfos.begin();
				iter != m_attributeInfos.end();
				++iter){
		retVal.insert(iter->first);
	}

	return retVal;
}


IRegistryElement::AttributeInfo* CRegistryElement::InsertAttributeInfo(
			const std::string& attributeId,
			const std::string& attributeType)
{
	if (m_attributeInfos.find(attributeId) != m_attributeInfos.end()){
		return NULL;
	}

	AttributeInfo& info = m_attributeInfos[attributeId];
	info.attributeTypeName = attributeType;

	return &info;
}


iser::IObject* CRegistryElement::CreateAttribute(const std::string& attributeType) const
{
	static std::string boolAttrTypeName = istd::CClassInfo::GetName<icomp::CBoolAttribute>();
	static std::string doubleAttrTypeName = istd::CClassInfo::GetName<icomp::CDoubleAttribute>();
	static std::string intAttrTypeName = istd::CClassInfo::GetName<icomp::CIntAttribute>();
	static std::string stringAttrTypeName = istd::CClassInfo::GetName<icomp::CStringAttribute>();
	static std::string multiBoolAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiBoolAttribute>();
	static std::string multiDoubleAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiDoubleAttribute>();
	static std::string multiIntAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiIntAttribute>();
	static std::string multiStringAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiStringAttribute>();
	static std::string referenceAttrTypeName = istd::CClassInfo::GetName<icomp::CReferenceAttribute>();
	static std::string multiReferenceAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiReferenceAttribute>();
	static std::string factoryAttrTypeName = istd::CClassInfo::GetName<icomp::CFactoryAttribute>();
	static std::string multiFactoryAttrTypeName = istd::CClassInfo::GetName<icomp::CMultiFactoryAttribute>();

	if (attributeType == boolAttrTypeName){
		return new icomp::CBoolAttribute();
	}
	else if (attributeType == doubleAttrTypeName){
		return new icomp::CDoubleAttribute();
	}
	else if (attributeType == intAttrTypeName){
		return new icomp::CIntAttribute();
	}
	else if (attributeType == stringAttrTypeName){
		return new icomp::CStringAttribute();
	}
	else if (attributeType == multiBoolAttrTypeName){
		return new icomp::CMultiBoolAttribute();
	}
	else if (attributeType == multiDoubleAttrTypeName){
		return new icomp::CMultiDoubleAttribute();
	}
	else if (attributeType == multiIntAttrTypeName){
		return new icomp::CMultiIntAttribute();
	}
	else if (attributeType == multiStringAttrTypeName){
		return new icomp::CMultiStringAttribute();
	}
	else if (attributeType == referenceAttrTypeName){
		return new icomp::CReferenceAttribute();
	}
	else if (attributeType == multiReferenceAttrTypeName){
		return new icomp::CMultiReferenceAttribute();
	}
	else if (attributeType == factoryAttrTypeName){
		return new icomp::CFactoryAttribute();
	}
	else if (attributeType == multiFactoryAttrTypeName){
		return new icomp::CMultiFactoryAttribute();
	}
	else{
		return NULL;
	}
}


const IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const std::string& attributeId) const
{
	AttributeInfoMap::const_iterator iter = m_attributeInfos.find(attributeId);

	if (iter != m_attributeInfos.end()){
		return &iter->second;
	}

	return NULL;
}


bool CRegistryElement::RemoveAttribute(const std::string& attributeId)
{
	istd::CChangeNotifier notifier(this);

	return m_attributeInfos.erase(attributeId) > 0;
}


// reimplemented (iser::ISerializable)

bool CRegistryElement::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag flagsTag("Flags", "Flags of registry element");
	static iser::CArchiveTag attributeInfosTag("AttributeInfoMap", "List of attribute infos");
	static iser::CArchiveTag attributeInfoTag("AttributeInfo", "Attribute info", true);
	static iser::CArchiveTag attributeIdTag("Id", "Attribute ID");
	static iser::CArchiveTag attributeTypeTag("Type", "Type of attribute");
	static iser::CArchiveTag exportIdTag("ExportId", "ID used for export");
	static iser::CArchiveTag dataTag("Data", "ID used for export", true);
	static iser::CArchiveTag isEnabledTag("IsEnabled", "Is attribute enabled");

	bool retVal = true;

	bool isStoring = archive.IsStoring();

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	I_DWORD versionNumber = 0xffffffff;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, versionNumber);

	if (versionNumber >= 1052){
		retVal = retVal && archive.BeginTag(flagsTag);
		retVal = retVal && archive.Process(m_elementFlags);
		retVal = retVal && archive.EndTag(flagsTag);
	}
	else if (!isStoring){
		m_elementFlags = 0;
	}

	if (archive.IsStoring()){
		int attributesCount = 0;
		for (		AttributeInfoMap::const_iterator checkInfoIter = m_attributeInfos.begin();	// calculate number of real used attributes
					checkInfoIter != m_attributeInfos.end();
					++checkInfoIter){
			const AttributeInfo& info = checkInfoIter->second;

			if (info.exportId.empty() && !info.attributePtr.IsValid()){
				continue;
			}

			++attributesCount;
		}

		retVal = retVal && archive.BeginMultiTag(attributeInfosTag, attributeInfoTag, attributesCount);

		for (		AttributeInfoMap::iterator iter = m_attributeInfos.begin();
					iter != m_attributeInfos.end();
					++iter){
			AttributeInfo& info = iter->second;

			if (info.exportId.empty() && !info.attributePtr.IsValid()){
				continue;
			}

			retVal = retVal && archive.BeginTag(attributeInfoTag);

			retVal = retVal && archive.BeginTag(attributeIdTag);
			std::string attributeId = iter->first;
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(attributeIdTag);

			bool isEnabled = info.attributePtr.IsValid();
			std::string attributeType = isEnabled? info.attributePtr->GetFactoryId(): info.attributeTypeName;

			retVal = retVal && archive.BeginTag(attributeTypeTag);
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(attributeTypeTag);

			retVal = retVal && archive.BeginTag(exportIdTag);
			retVal = retVal && archive.Process(info.exportId);
			retVal = retVal && archive.EndTag(exportIdTag);

			retVal = retVal && archive.BeginTag(dataTag);

			retVal = retVal && archive.BeginTag(isEnabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(isEnabledTag);

			if (isEnabled){
				retVal = retVal && info.attributePtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(attributeInfoTag);
		}

		retVal = retVal && archive.EndTag(attributeInfosTag);
	}
	else{
		int attributesCount = 0;
		retVal = retVal && archive.BeginMultiTag(attributeInfosTag, attributeInfoTag, attributesCount);

		if (!retVal){
			return false;
		}

		for (int i = 0; i < attributesCount; ++i){
			retVal = retVal && archive.BeginTag(attributeInfoTag);

			retVal = retVal && archive.BeginTag(attributeIdTag);
			std::string attributeId;
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(attributeIdTag);

			if (!retVal){
				return false;
			}

			std::string attributeType;
			retVal = retVal && archive.BeginTag(attributeTypeTag);
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(attributeTypeTag);

			retVal = retVal && archive.BeginTag(exportIdTag);
			std::string exportId;
			retVal = retVal && archive.Process(exportId);
			retVal = retVal && archive.EndTag(exportIdTag);

			retVal = retVal && archive.BeginTag(dataTag);

			retVal = retVal && archive.BeginTag(isEnabledTag);
			bool isEnabled = true;
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(isEnabledTag);

			if (!retVal){
				return false;
			}

			AttributeInfo* infoPtr = InsertAttributeInfo(attributeId, attributeType);

			if (infoPtr != NULL){
				infoPtr->exportId = exportId;
				infoPtr->attributeTypeName = attributeType;

				if (isEnabled){
					iser::IObject* attributePtr = CreateAttribute(attributeType);
					if (attributePtr == NULL){
						return false;
					}

					infoPtr->attributePtr.SetPtr(attributePtr);

					retVal = retVal && attributePtr->Serialize(archive);
				}
			}
			else if (!archive.IsTagSkippingSupported()){
				return false;
			}

			retVal = retVal && archive.EndTag(dataTag);

			retVal = retVal && archive.EndTag(attributeInfoTag);
		}

		retVal = retVal && archive.EndTag(attributeInfosTag);
	}

	return retVal;
}


}//namespace icomp


