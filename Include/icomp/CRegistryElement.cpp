#include <icomp/CRegistryElement.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iattr/TAttribute.h>
#include <iattr/TMultiAttribute.h>
#include <icomp/IAttributeStaticInfo.h>
#include <icomp/IComponentStaticInfo.h>
#include <icomp/CReferenceAttribute.h>
#include <icomp/CFactoryAttribute.h>
#include <icomp/CMultiReferenceAttribute.h>
#include <icomp/CMultiFactoryAttribute.h>


namespace icomp
{


// static constants
static const iser::CArchiveTag s_flagsTag("Flags", "Flags of registry element", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_attributeInfosTag("AttributeInfoMap", "List of attribute infos", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_attributeInfoTag("AttributeInfo", "Attribute info", iser::CArchiveTag::TT_GROUP, &s_attributeInfosTag, true);
static const iser::CArchiveTag s_attributeIdTag("Id", "Attribute ID", iser::CArchiveTag::TT_LEAF, &s_attributeInfoTag);
static const iser::CArchiveTag s_attributeTypeTag("Type", "Type of attribute", iser::CArchiveTag::TT_LEAF, &s_attributeInfoTag);
static const iser::CArchiveTag s_exportIdTag("ExportId", "ID used for export", iser::CArchiveTag::TT_LEAF, &s_attributeInfoTag);
static const iser::CArchiveTag s_dataTag("Data", "ID used for export", iser::CArchiveTag::TT_GROUP, &s_attributeInfoTag, true);
static const iser::CArchiveTag s_isEnabledTag("IsEnabled", "Is attribute enabled", iser::CArchiveTag::TT_LEAF, &s_dataTag);


CRegistryElement::CRegistryElement()
:	m_elementFlags(0)
{
}


IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const QByteArray& attributeId)
{
	AttributeInfoMap::iterator iter = m_attributeInfos.find(attributeId);
	if (iter != m_attributeInfos.end()){
		return &iter.value();
	}

	return NULL;
}


// reimplemented (icomp::IRegistryElement)

quint32 CRegistryElement::GetElementFlags() const
{
	return m_elementFlags;
}


void CRegistryElement::SetElementFlags(quint32 flags)
{
	if (flags != m_elementFlags){
		istd::CChangeNotifier notifier(this);

		m_elementFlags = flags;
	}
}


IRegistryElement::AttributeInfo* CRegistryElement::InsertAttributeInfo(
			const QByteArray& attributeId,
			const QByteArray& attributeType)
{
	if (m_attributeInfos.find(attributeId) != m_attributeInfos.end()){
		return NULL;
	}

	AttributeInfo& info = m_attributeInfos[attributeId];
	info.attributeTypeName = attributeType;

	return &info;
}


iser::IObject* CRegistryElement::CreateAttribute(const QByteArray& attributeType) const
{
	static QByteArray integerAttrTypeName = iattr::CIntegerAttribute::GetTypeName();
	static QByteArray realAttrTypeName = iattr::CRealAttribute::GetTypeName();
	static QByteArray booleanAttrTypeName = iattr::CBooleanAttribute::GetTypeName();
	static QByteArray stringAttrTypeName = iattr::CStringAttribute::GetTypeName();
	static QByteArray idAttrTypeName = iattr::CIdAttribute::GetTypeName();

	static QByteArray boolListAttrTypeName = iattr::CBooleanListAttribute::GetTypeName();
	static QByteArray realListAttrTypeName = iattr::CRealListAttribute::GetTypeName();
	static QByteArray integerListAttrTypeName = iattr::CIntegerListAttribute::GetTypeName();
	static QByteArray stringListAttrTypeName = iattr::CStringListAttribute::GetTypeName();
	static QByteArray idListAttrTypeName = iattr::CIdListAttribute::GetTypeName();

	static QByteArray referenceAttrTypeName = icomp::CReferenceAttribute::GetTypeName();
	static QByteArray multiReferenceAttrTypeName = icomp::CMultiReferenceAttribute::GetTypeName();
	static QByteArray factoryAttrTypeName = icomp::CFactoryAttribute::GetTypeName();
	static QByteArray multiFactoryAttrTypeName = icomp::CMultiFactoryAttribute::GetTypeName();

	if (attributeType == integerAttrTypeName){
		return new iattr::CIntegerAttribute();
	}
	else if (attributeType == realAttrTypeName){
		return new iattr::CRealAttribute();
	}
	else if (attributeType == booleanAttrTypeName){
		return new iattr::CBooleanAttribute();
	}
	else if (attributeType == stringAttrTypeName){
		return new iattr::CStringAttribute();
	}
	else if (attributeType == idAttrTypeName){
		return new iattr::CIdAttribute();
	}
	else if (attributeType == integerListAttrTypeName){
		return new iattr::CIntegerListAttribute();
	}
	else if (attributeType == realListAttrTypeName){
		return new iattr::CRealListAttribute();
	}
	else if (attributeType == boolListAttrTypeName){
		return new iattr::CBooleanListAttribute();
	}
	else if (attributeType == stringListAttrTypeName){
		return new iattr::CStringListAttribute();
	}
	else if (attributeType == idListAttrTypeName){
		return new iattr::CIdListAttribute();
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


const IRegistryElement::AttributeInfo* CRegistryElement::GetAttributeInfo(const QByteArray& attributeId) const
{
	AttributeInfoMap::ConstIterator iter = m_attributeInfos.constFind(attributeId);
	if (iter != m_attributeInfos.constEnd()){
		return &iter.value();
	}

	return NULL;
}


bool CRegistryElement::RemoveAttribute(const QByteArray& attributeId)
{
	istd::CChangeNotifier notifier(this);

	return m_attributeInfos.remove(attributeId) > 0;
}


// reimplemented (iattr::IAttributesProvider)

iattr::IAttributesProvider::AttributeIds CRegistryElement::GetAttributeIds() const
{
	AttributeIds retVal;

	for (		AttributeInfoMap::const_iterator iter = m_attributeInfos.begin();
				iter != m_attributeInfos.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


iser::IObject* CRegistryElement::GetAttribute(const QByteArray& attributeId) const
{
	AttributeInfoMap::ConstIterator iter = m_attributeInfos.constFind(attributeId);
	if (iter != m_attributeInfos.constEnd()){
		const AttributeInfo& attributeInfo = iter.value();

		return attributeInfo.attributePtr.GetPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CRegistryElement::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_flagsTag);
	retVal = retVal && archive.Process(m_elementFlags);
	retVal = retVal && archive.EndTag(s_flagsTag);

	if (archive.IsStoring()){
		int attributesCount = 0;
		for (		AttributeInfoMap::const_iterator checkInfoIter = m_attributeInfos.begin();	// calculate number of real used attributes
					checkInfoIter != m_attributeInfos.end();
					++checkInfoIter){
			const AttributeInfo& info = checkInfoIter.value();

			if (info.exportId.isEmpty() && !info.attributePtr.IsValid()){
				continue;
			}

			++attributesCount;
		}

		retVal = retVal && archive.BeginMultiTag(s_attributeInfosTag, s_attributeInfoTag, attributesCount);

		for (		AttributeInfoMap::iterator iter = m_attributeInfos.begin();
					iter != m_attributeInfos.end();
					++iter){
			AttributeInfo& info = iter.value();

			if (info.exportId.isEmpty() && !info.attributePtr.IsValid()){
				continue;
			}

			retVal = retVal && archive.BeginTag(s_attributeInfoTag);

			retVal = retVal && archive.BeginTag(s_attributeIdTag);
			QByteArray attributeId = iter.key();
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(s_attributeIdTag);

			bool isEnabled = info.attributePtr.IsValid();
			QByteArray attributeType = isEnabled? info.attributePtr->GetFactoryId(): info.attributeTypeName;

			retVal = retVal && archive.BeginTag(s_attributeTypeTag);
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(s_attributeTypeTag);

			retVal = retVal && archive.BeginTag(s_exportIdTag);
			retVal = retVal && archive.Process(info.exportId);
			retVal = retVal && archive.EndTag(s_exportIdTag);

			retVal = retVal && archive.BeginTag(s_dataTag);

			retVal = retVal && archive.BeginTag(s_isEnabledTag);
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(s_isEnabledTag);

			if (isEnabled){
				retVal = retVal && info.attributePtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(s_dataTag);

			retVal = retVal && archive.EndTag(s_attributeInfoTag);
		}

		retVal = retVal && archive.EndTag(s_attributeInfosTag);
	}
	else{
		int attributesCount = 0;
		retVal = retVal && archive.BeginMultiTag(s_attributeInfosTag, s_attributeInfoTag, attributesCount);

		if (!retVal){
			return false;
		}

		for (int i = 0; i < attributesCount; ++i){
			retVal = retVal && archive.BeginTag(s_attributeInfoTag);

			retVal = retVal && archive.BeginTag(s_attributeIdTag);
			QByteArray attributeId;
			retVal = retVal && archive.Process(attributeId);
			retVal = retVal && archive.EndTag(s_attributeIdTag);

			if (!retVal){
				return false;
			}

			QByteArray attributeType;
			retVal = retVal && archive.BeginTag(s_attributeTypeTag);
			retVal = retVal && archive.Process(attributeType);
			retVal = retVal && archive.EndTag(s_attributeTypeTag);

			retVal = retVal && archive.BeginTag(s_exportIdTag);
			QByteArray exportId;
			retVal = retVal && archive.Process(exportId);
			retVal = retVal && archive.EndTag(s_exportIdTag);

			retVal = retVal && archive.BeginTag(s_dataTag);

			retVal = retVal && archive.BeginTag(s_isEnabledTag);
			bool isEnabled = true;
			retVal = retVal && archive.Process(isEnabled);
			retVal = retVal && archive.EndTag(s_isEnabledTag);

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

			retVal = retVal && archive.EndTag(s_dataTag);

			retVal = retVal && archive.EndTag(s_attributeInfoTag);
		}

		retVal = retVal && archive.EndTag(s_attributeInfosTag);
	}

	return retVal;
}


} // namespace icomp


