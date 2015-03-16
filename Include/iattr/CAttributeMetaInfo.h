#ifndef iattr_CAttributeMetaInfo_included
#define iattr_CAttributeMetaInfo_included


// ACF includes
#include "istd/TOptDelPtr.h"

// ACF-Solutions includes
#include "iattr/IAttributeMetaInfo.h"


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class CAttributeMetaInfo: virtual public IAttributeMetaInfo
{
public:
	CAttributeMetaInfo(const QByteArray& typeId = "");

	void SetAttributeTypeId(const QByteArray& typeId);
	void SetAttributeDescription(const QString& description);
	void SetAttributeFlags(int attributeFlags);
	void SetAttributeDefaultValue(const iser::IObject* defaultValuePtr, bool releaseFlag);

	// reimplemented (iattr::IAttributeMetaInfo)
	virtual QString GetAttributeDescription() const;
	virtual const iser::IObject* GetAttributeDefaultValue() const;
	virtual QByteArray GetAttributeTypeId() const;
	virtual int GetAttributeFlags() const;

private:
	QString m_description;
	QByteArray m_attributeTypeId;
	int m_attributeFlags;
	istd::TOptDelPtr<const iser::IObject> m_defaultValuePtr;
};


} // namespace iattr


#endif // !iattr_CAttributeMetaInfo_included


