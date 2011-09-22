#include "iattr/CAttributedObject.h"


// ACF includes
#include "iattr/TAttribute.h"
#include "iattr/TMultiAttribute.h"


namespace iattr
{


// public methods

CAttributedObject::CAttributedObject()
{
}


CAttributedObject::AttributeInfo* CAttributedObject::GetAttributeInfo(const std::string& attributeId) const
{
	int attributesCount = m_attributesList.GetCount();

	for (int attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++){
		CAttributedObject::AttributeInfo* attributeInfoPtr = m_attributesList.GetAt(attributeIndex);
		I_ASSERT(attributeInfoPtr != NULL);

		if (attributeInfoPtr->attributeId == attributeId){
			return attributeInfoPtr;
		}
	}

	return NULL;
}


// reimplemented (iattr::IAttributesManager)

int CAttributedObject::GetAttributesCount() const
{
	return m_attributesList.GetCount();
}


iser::IObject* CAttributedObject::GetAttribute(int attributeIndex) const
{
	I_ASSERT(attributeIndex >= 0);
	I_ASSERT(attributeIndex < m_attributesList.GetCount());

	return m_attributesList.GetAt(attributeIndex)->objectPtr.GetPtr();
}


std::string CAttributedObject::GetAttributeId(int attributeIndex) const
{
	I_ASSERT(attributeIndex >= 0);
	I_ASSERT(attributeIndex < m_attributesList.GetCount());

	return m_attributesList.GetAt(attributeIndex)->attributeId;
}


istd::CString CAttributedObject::GetAttributeDescription(int attributeIndex) const
{
	I_ASSERT(attributeIndex >= 0);
	I_ASSERT(attributeIndex < m_attributesList.GetCount());

	return m_attributesList.GetAt(attributeIndex)->attributeDescription;
}


void CAttributedObject::InsertAttribute(
			iser::IObject* objectPtr,
			const std::string& attributeId,
			const std::string& attributeDescription,
			int attributeFlags,
			bool releaseFlag)
{
	AttributeInfo* existingInfoPtr = GetAttributeInfo(attributeId);
	I_ASSERT(existingInfoPtr == NULL);
	if (objectPtr != NULL && existingInfoPtr == NULL){
		AttributeInfo* attributeInfoPtr = new AttributeInfo;

		attributeInfoPtr->objectPtr.SetPtr(objectPtr, releaseFlag);
		attributeInfoPtr->attributeId = attributeId;
		attributeInfoPtr->attributeDescription = attributeDescription;
		attributeInfoPtr->attributeFlags = attributeFlags;

		m_attributesList.PushBack(attributeInfoPtr);
	}
}


// reimplemented (ISerializable)

bool CAttributedObject::Serialize(iser::IArchive& archive)
{
	iser::CArchiveTag attributesTag("Attributes", "Liste of object attributes");
	iser::CArchiveTag attributeTag("Attribute", "Object attribute");

	if (archive.IsStoring()){
		return WriteAttributes(archive, attributesTag, attributeTag);
	}

	return ReadAttributes(archive, attributesTag, attributeTag);
}


bool CAttributedObject::ReadAttributes(
			iser::IArchive& archive,
			const iser::CArchiveTag& attributesTag,
			const iser::CArchiveTag& attributeTag)
{
	bool retVal = true;

	int attributesCount = m_attributesList.GetCount();

	retVal = retVal && archive.BeginMultiTag(attributesTag, attributeTag, attributesCount);

	for (int attributeIndex = 0; attributeIndex < attributesCount; ++attributeIndex){
		retVal = retVal && archive.BeginTag(attributeTag);

		std::string attributeId;
		std::string attributeTypeId;

		iser::CArchiveTag attributeTypeIdTag("AttributeTypeId", "ID of the attribute object");
		retVal = retVal && archive.BeginTag(attributeTypeIdTag);
		retVal = retVal && archive.Process(attributeTypeId);
		retVal = retVal && archive.EndTag(attributeTypeIdTag);

		iser::CArchiveTag attributeIdTag("AttributeId", "Name of the attribute object");
		retVal = retVal && archive.BeginTag(attributeIdTag);
		retVal = retVal && archive.Process(attributeId);
		retVal = retVal && archive.EndTag(attributeIdTag);

		if (retVal){
			AttributeInfo* existingAttrPtr = GetAttributeInfo(attributeId);

			if (		existingAttrPtr != NULL &&
						existingAttrPtr->objectPtr->GetFactoryId() == attributeTypeId &&
						((existingAttrPtr->attributeFlags & iattr::IAttribute::AF_PERSISTENT) != 0)){
				retVal = retVal && existingAttrPtr->objectPtr->Serialize(archive);
			}
			else{
				// try to serialize deprecated attribute:
				istd::TDelPtr<iser::IObject> objectPtr(s_attributesFactory.CreateInstance(attributeTypeId));
				if (objectPtr.IsValid()){
					retVal = retVal && objectPtr->Serialize(archive);
				}
			}
		}

		retVal = retVal && archive.EndTag(attributeTag);
	}

	retVal = retVal && archive.EndTag(attributesTag);

	return retVal;
}


bool CAttributedObject::WriteAttributes(
			iser::IArchive& archive,
			const iser::CArchiveTag& attributesTag,
			const iser::CArchiveTag& attributeTag) const
{

	bool retVal = true;
	int attributesCount = m_attributesList.GetCount();
	int persistentAttributesCount = 0;

	for (int attributeIndex = 0; attributeIndex < attributesCount; ++attributeIndex){
		AttributeInfo* attributeInfoPtr = m_attributesList.GetAt(attributeIndex);

		bool isPersistent = ((attributeInfoPtr->attributeFlags & iattr::IAttribute::AF_PERSISTENT) != 0);
		if (isPersistent){
			++persistentAttributesCount;
		}
	}

	retVal = retVal && archive.BeginMultiTag(attributesTag, attributeTag, persistentAttributesCount);

	for (int attributeIndex = 0; attributeIndex < attributesCount; ++attributeIndex){
		AttributeInfo* attributeInfoPtr = m_attributesList.GetAt(attributeIndex);

		bool isPersistent = ((attributeInfoPtr->attributeFlags & iattr::IAttribute::AF_PERSISTENT) != 0);
		if (!isPersistent){
			continue;
		}

		iser::IObject* objectPtr = attributeInfoPtr->objectPtr.GetPtr();
		std::string attributeId = attributeInfoPtr->attributeId;
		std::string attributeTypeId = objectPtr->GetFactoryId();

		retVal = retVal && archive.BeginTag(attributeTag);

		iser::CArchiveTag attributeTypeIdTag("AttributeTypeId", "ID of the attribute object");
		retVal = retVal && archive.BeginTag(attributeTypeIdTag);
		retVal = retVal && archive.Process(attributeTypeId);
		retVal = retVal && archive.EndTag(attributeTypeIdTag);

		iser::CArchiveTag attributeIdTag("AttributeId", "Name of the attribute object");
		retVal = retVal && archive.BeginTag(attributeIdTag);
		retVal = retVal && archive.Process(attributeId);
		retVal = retVal && archive.EndTag(attributeIdTag);

		retVal = retVal && objectPtr->Serialize(archive);

		retVal = retVal && archive.EndTag(attributeTag);
	}

	retVal = retVal && archive.EndTag(attributesTag);

	return retVal;
}


// private static members

CAttributedObject::AttributesFactory CAttributedObject::s_attributesFactory;

static struct DefaultAttributeTypesRegistrator
{
	DefaultAttributeTypesRegistrator()
	{
		CAttributedObject::RegisterAttributeType<iattr::CIntAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CBoolAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CDoubleAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CStringAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CMultiIntAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CMultiBoolAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CMultiDoubleAttribute>();
		CAttributedObject::RegisterAttributeType<iattr::CMultiStringAttribute>();
	}

} s_defaultAttributeTypesRegistrator;


} // namespace iattr


