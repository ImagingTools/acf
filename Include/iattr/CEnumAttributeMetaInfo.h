#ifndef iattr_CEnumAttributeMetaInfo_included
#define iattr_CEnumAttributeMetaInfo_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/TOptDelPtr.h"

// ACF-Solutions includes
#include "iattr/IEnumAttributeMetaInfo.h"
#include "iattr/IAttributesProvider.h"


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class CEnumAttributeMetaInfo:
				virtual public IEnumAttributeMetaInfo,
				virtual protected IAttributesProvider
{
public:
	CEnumAttributeMetaInfo();

	void SetOtherValueAllowed(bool state);
	bool InsertOption(const QString& description, const iser::IObject* valuePtr, bool releaseFlag);

	// reimplemented (iattr::IEnumAttributeMetaInfo)
	virtual bool IsAnyValueAllowed() const;
	virtual int GetEnumsCount() const;
	virtual QString GetEnumDescription(int index) const;
	virtual const iser::IObject& GetEnum(int index) const;

private:
	bool m_isAnyValueAllowed;
	struct EnumInfo
	{
		QString description;
		istd::TOptDelPtr<const iser::IObject> attributePtr;
	};
	QList<EnumInfo> m_enums;
};


} // namespace iattr


#endif // !iattr_CEnumAttributeMetaInfo_included


