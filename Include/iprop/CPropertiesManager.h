#ifndef iprop_CPropertiesManager_included
#define iprop_CPropertiesManager_included


#include "istd/TComposedFactory.h"
#include "istd/TSingleFactory.h"
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"
#include "iprop/IPropertiesManager.h"


namespace iprop
{


/**
	Basic implementation of an attribute container.
*/
class CPropertiesManager: virtual public iprop::IPropertiesManager
{
public:
	typedef istd::TComposedFactory<iser::IObject> BaseClass;

	/*
		Internal attribute info item.
	*/
	struct AttributeInfo
	{
		std::string attributeId;
		std::string attributeDescription;
		int attributeFlags;

		istd::TOptDelPtr<iser::IObject> objectPtr; 
	};

	CPropertiesManager();

	/**
		Get attribute info object for a given attribute ID.
	*/
	AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;

	/**
		Register a new attribute type.
	*/
	template <typename AttributeType>
	static bool RegisterAttributeType();

	// reimplemented (iprop::IPropertiesManager)
	virtual int GetAttributesCount() const;
	virtual iser::IObject* GetAttribute(int attributeIndex) const;
	virtual std::string GetAttributeId(int attributeIndex) const;
	virtual istd::CString GetAttributeDescription(int attributeIndex) const;
	virtual void InsertAttribute(
				iser::IObject* objectPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				bool releaseFlag);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	bool ReadAttributes(
				iser::IArchive& archive,
				const iser::CArchiveTag& attributesTag,
				const iser::CArchiveTag& attributeTag);

	bool WriteAttributes(
				iser::IArchive& archive,
				const iser::CArchiveTag& attributesTag,
				const iser::CArchiveTag& attributeTag) const;

private:
	typedef istd::TComposedFactory<iser::IObject> AttributesFactory;

	istd::TPointerVector<AttributeInfo> m_attributesList;

	static AttributesFactory s_attributesFactory;
};


// public static methods

template <typename AttributeImpl>
bool CPropertiesManager::RegisterAttributeType()
{
	return s_attributesFactory.RegisterFactory(new istd::TSingleFactory<iser::IObject, AttributeImpl>(AttributeImpl::GetTypeName()), true);
}


} // namespace iprop


#endif // !iprop_CPropertiesManager_included


