#include "icomp/CRegistryElement.h"


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


CRegistryElement::CRegistryElement()
:	m_staticInfoPtr(NULL),
	m_elementFlags(0)
{
}


void CRegistryElement::Initialize(const IComponentStaticInfo* infoPtr)
{
	I_ASSERT(infoPtr != NULL);

	m_staticInfoPtr = infoPtr;
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


const IComponentStaticInfo& CRegistryElement::GetComponentStaticInfo() const
{
	I_ASSERT(m_staticInfoPtr != NULL);

	return *m_staticInfoPtr;
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


IRegistryElement::AttributeInfo* CRegistryElement::InsertAttributeInfo(const std::string& attributeId)
{
	if (m_attributeInfos.find(attributeId) != m_attributeInfos.end()){
		return NULL;
	}

	iser::ISerializable* attributePtr = NULL;

	AttributeInfo& info = m_attributeInfos[attributeId];

	info.attributePtr.SetPtr(attributePtr);

	return &info;
}


iser::ISerializable* CRegistryElement::CreateAttribute(const std::string& attributeId) const
{
	const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
	if (staticInfoPtr == NULL){
		I_TRACE_ONCE(istd::ErrorLevel, "Components", ("No attribute static info: " + attributeId).c_str());

		return NULL;
	}

	return staticInfoPtr->CreateAttribute();
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

	const iser::IVersionInfo& info = archive.GetVersionInfo();
	I_DWORD versionNumber = 0xffffffff;
	info.GetVersionNumber(iser::IVersionInfo::FrameworkVersionId, versionNumber);

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

			const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
			if (staticInfoPtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(attributeTypeTag);
			std::string attributeType = staticInfoPtr->GetAttributeType().GetName();
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(attributeTypeTag);

			retVal = retVal && archive.BeginTag(exportIdTag);
			retVal = retVal && archive.Process(info.exportId);
			retVal = retVal && archive.EndTag(exportIdTag);

			retVal = retVal && archive.BeginTag(dataTag);

			bool isEnabled = info.attributePtr.IsValid();
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

			const IAttributeStaticInfo* staticInfoPtr = GetAttributeStaticInfo(attributeId);
			if (staticInfoPtr != NULL){
				retVal = retVal && archive.BeginTag(attributeTypeTag);
				std::string attributeType;
				retVal = retVal && archive.Process(attributeType);
				retVal = retVal && archive.EndTag(attributeTypeTag);

				if (attributeType == staticInfoPtr->GetAttributeType().GetName()){
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

					AttributeInfo* infoPtr = InsertAttributeInfo(attributeId);

					if (infoPtr != NULL){
						infoPtr->exportId = exportId;

						if (isEnabled){
							iser::ISerializable* attributePtr = CreateAttribute(attributeId);
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
				}
			}

			retVal = retVal && archive.EndTag(attributeInfoTag);
		}

		retVal = retVal && archive.EndTag(attributeInfosTag);
	}

	return retVal;
}


// protected methods

const IAttributeStaticInfo* CRegistryElement::GetAttributeStaticInfo(const std::string& attributeId) const
{
	I_ASSERT(m_staticInfoPtr != NULL);

	const IComponentStaticInfo::AttributeInfos& infos = m_staticInfoPtr->GetAttributeInfos();

	int attributeIndex = infos.FindIndex(attributeId);

	if (attributeIndex >= 0){
		return infos.GetValueAt(attributeIndex);
	}

	return NULL;
}


}//namespace icomp


