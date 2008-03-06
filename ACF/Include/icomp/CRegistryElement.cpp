#include "icomp/CRegistryElement.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


CRegistryElement::CRegistryElement(const IComponentStaticInfo* infoPtr)
:	m_staticInfo(*infoPtr)
{
	I_ASSERT(infoPtr != NULL);
}


IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const ::std::string& attributeId)
{
	AttributeInfoMap::iterator iter = m_attributeInfos.find(attributeId);
	if (iter != m_attributeInfos.end()){
		return &iter->second;
	}

	return NULL;
}


// reimplemented (icomp::IRegistryElement)

IRegistryElement::AttributeInfo* CRegistryElement::InsertAttributeInfo(const ::std::string& attributeId, bool createAttribute)
{
	if (m_attributeInfos.find(attributeId) != m_attributeInfos.end()){
		return NULL;
	}

	iser::ISerializable* attributePtr = NULL;

	if (createAttribute){
		attributePtr = CreateAttribute(attributeId);

		if (attributePtr == NULL){
			return NULL;
		}
	}

	AttributeInfo& info = m_attributeInfos[attributeId];

	info.attributePtr = AttributePtr(attributePtr);

	return &info;
}


iser::ISerializable* CRegistryElement::CreateAttribute(const ::std::string& attributeId) const
{
	const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
	if (staticInfoPtr == NULL){
		return NULL;
	}

	return staticInfoPtr->CreateAttribute();
}


bool CRegistryElement::RemoveAttribute(const ::std::string& attributeId)
{
	return m_attributeInfos.erase(attributeId) > 0;
}


// reimplemented (icomp::IRegistryElement)

const IComponentStaticInfo& CRegistryElement::GetComponentStaticInfo() const
{
	return m_staticInfo;
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


const IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const ::std::string& attributeId) const
{
	AttributeInfoMap::const_iterator iter = m_attributeInfos.find(attributeId);

	if (iter != m_attributeInfos.end()){
		return &iter->second;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CRegistryElement::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag attributeInfosTag("AttributeInfoMap", "List of attribute infos");
	static iser::CArchiveTag attributeInfoTag("AttributeInfo", "Attribute info", true);
	static iser::CArchiveTag attributeIdTag("Id", "Attribute ID");
	static iser::CArchiveTag attributeTypeTag("Type", "Type of attribute");
	static iser::CArchiveTag exportIdTag("ExportId", "ID used for export");
	static iser::CArchiveTag attributeTag("Attribute", "ID used for export");
	static iser::CArchiveTag isEnabledTag("IsEnabled", "Is attribute enabled");

	bool retVal = true;

	int attributesCount = int(m_attributeInfos.size());
	retVal = retVal && archive.BeginMultiTag(attributeInfosTag, attributeInfoTag, attributesCount);

	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (		AttributeInfoMap::iterator iter = m_attributeInfos.begin();
					iter != m_attributeInfos.end();
					++iter){
			retVal = retVal && archive.BeginTag(attributeInfoTag);

			retVal = retVal && archive.BeginTag(attributeIdTag);
			::std::string attributeId = iter->first;
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(attributeIdTag);

			const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
			if (staticInfoPtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(attributeTypeTag);
			::std::string attributeType = staticInfoPtr->GetAttributeType().name();
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(attributeTypeTag);

			AttributeInfo& info = iter->second;

			retVal = retVal && archive.BeginTag(exportIdTag);
			retVal = retVal && archive.Process(info.exportId);
			retVal = retVal && archive.EndTag(exportIdTag);

			retVal = retVal && archive.BeginTag(attributeTag);

			bool isEnabled = info.attributePtr.IsValid();
			retVal = retVal && archive.BeginTag(isEnabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(isEnabledTag);

			if (isEnabled){
				retVal = retVal && info.attributePtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(attributeTag);

			retVal = retVal && archive.EndTag(attributeInfoTag);
		}
	}
	else{
		for (int i = 0; i < attributesCount; ++i){
			retVal = retVal && archive.BeginTag(attributeInfoTag);

			retVal = retVal && archive.BeginTag(attributeIdTag);
			::std::string attributeId;
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(attributeIdTag);

			if (!retVal){
				return false;
			}

			const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
			if (staticInfoPtr != NULL){
				retVal = retVal && archive.BeginTag(attributeTypeTag);
				::std::string attributeType;
				retVal = retVal && archive.Process(attributeType);
				retVal = retVal && archive.EndTag(attributeTypeTag);

				if (attributeType == staticInfoPtr->GetAttributeType().name()){
					retVal = retVal && archive.BeginTag(exportIdTag);
					::std::string exportId;
					retVal = retVal && archive.Process(exportId);
					retVal = retVal && archive.EndTag(exportIdTag);

					retVal = retVal && archive.BeginTag(attributeTag);

					retVal = retVal && archive.BeginTag(isEnabledTag);
					bool isEnabled = true;
					retVal = retVal && archive.Process(isEnabled);
					retVal = retVal && archive.EndTag(isEnabledTag);

					if (!retVal){
						return false;
					}

					AttributeInfo* infoPtr = InsertAttributeInfo(attributeId, isEnabled);

					if (infoPtr == NULL){
						return false;
					}

					infoPtr->exportId = exportId;

					if (isEnabled){
						I_ASSERT(infoPtr->attributePtr.IsValid());
						retVal = retVal && infoPtr->attributePtr->Serialize(archive);
					}

					retVal = retVal && archive.EndTag(attributeTag);
				}
			}

			retVal = retVal && archive.EndTag(attributeInfoTag);
		}
	}

	retVal = retVal && archive.EndTag(attributeInfosTag);

	return retVal;
}


// protected methods

const IAttributeStaticInfo* CRegistryElement::GetAttributeStaticInfo(const ::std::string& attributeId) const
{
	const IComponentStaticInfo::AttributeInfos& infos = m_staticInfo.GetAttributeInfos();

	int attributeIndex = infos.FindIndex(attributeId);

	if (attributeIndex >= 0){
		return infos.GetValueAt(attributeIndex);
	}

	return NULL;
}


}//namespace icomp


