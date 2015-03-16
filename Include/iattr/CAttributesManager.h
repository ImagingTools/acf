#ifndef iattr_CAttributesManager_included
#define iattr_CAttributesManager_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "istd/TOptDelPtr.h"
#include "imod/CMultiModelBridgeBase.h"
#include "iser/IArchive.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"
#include "iattr/IAttributesManager.h"
#include "iattr/CStandardAttributesFactory.h"


namespace iattr
{


/**
	Basic implementation of an property container.
*/
class CAttributesManager: virtual public IAttributesManager
{
public:
	explicit CAttributesManager(const iser::IObjectFactory* factoryPtr = &CStandardAttributesFactory::GetInstance());

	void SetAttributesFactory(const iser::IObjectFactory* factoryPtr);

	// reimplemented (iattr::IAttributesManager)
	virtual void RemoveAllAttributes();
	virtual bool InsertAttribute(
				const QByteArray& attributeId,
				iser::IObject* attributePtr,
				bool releaseFlag);

	// reimplemented (iattr::IAttributesProvider)
	virtual AttributeIds GetAttributeIds() const;
	virtual iser::IObject* GetAttribute(const QByteArray& attributeId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef istd::TOptDelPtr<iser::IObject> AttributePtr;
	typedef QMap<QByteArray, AttributePtr> AttributesMap;
	AttributesMap m_attributesMap;

	const iser::IObjectFactory* m_attributesFactoryPtr;

	imod::CMultiModelBridgeBase m_attributesUpdateBridge;
};


} // namespace iattr


#endif // !iattr_CAttributesManager_included


