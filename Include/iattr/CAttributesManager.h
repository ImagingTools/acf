#ifndef iattr_CAttributesManager_included
#define iattr_CAttributesManager_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TOptDelPtr.h>
#include <imod/CModelUpdateBridge.h>
#include <iser/IArchive.h>
#include <iser/IObject.h>
#include <iser/CArchiveTag.h>
#include <iattr/IAttributesManager.h>
#include <iattr/CStandardAttributesFactory.h>


namespace iattr
{


/**
	Basic implementation of an property container.
*/
class CAttributesManager:
			virtual public IAttributesManager,
			virtual public iser::ISerializable
{
public:
	explicit CAttributesManager(const iser::IObjectFactory* factoryPtr = &CStandardAttributesFactory::GetInstance());

	void SetAttributesFactory(const iser::IObjectFactory* factoryPtr);

	// reimplemented (iattr::IAttributesManager)
	virtual void RemoveAllAttributes() override;
	virtual bool InsertAttribute(
				const QByteArray& attributeId,
				iser::IObject* attributePtr,
				bool releaseFlag) override;
	virtual bool RemoveAttribute(const QByteArray& attributeId) override;

	// reimplemented (iattr::IAttributesProvider)
	virtual AttributeIds GetAttributeIds() const override;
	virtual iser::IObject* GetAttribute(const QByteArray& attributeId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	typedef istd::TOptDelPtr<iser::IObject> AttributePtr;
	typedef QMap<QByteArray, AttributePtr> AttributesMap;
	AttributesMap m_attributesMap;

	const iser::IObjectFactory* m_attributesFactoryPtr;

	imod::CModelUpdateBridge m_attributesUpdateBridge;
};


} // namespace iattr


#endif // !iattr_CAttributesManager_included


