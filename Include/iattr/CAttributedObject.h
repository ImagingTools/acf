#ifndef iattr_CAttributedObject_included
#define iattr_CAttributedObject_included


#include "istd/TComposedFactory.h"
#include "istd/TSingleFactory.h"
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iattr/TAttribute.h"
#include "iattr/IAttributesManager.h"


namespace iattr
{


/**
	Basic implementation of an attribute container.
*/
class CAttributedObject: virtual public iattr::IAttributesManager
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

	CAttributedObject();

	AttributeInfo* GetAttributeInfo(const std::string& attributeId) const;

	template <typename AttributeType>
	static bool RegisterAttributeType();

	// reimplemented (iattr::IAttributesManager)
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
bool CAttributedObject::RegisterAttributeType()
{
	return s_attributesFactory.RegisterFactory(new istd::TSingleFactory<iser::IObject, AttributeImpl>(AttributeImpl::GetTypeName()), true);
}


} // namespace iattr


#endif // !iattr_CAttributedObject_included


