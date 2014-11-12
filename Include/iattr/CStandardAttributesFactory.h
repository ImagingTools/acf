#ifndef iattr_CStandardAttributesFactory_included
#define iattr_CStandardAttributesFactory_included


// ACF includes
#include "iser/IObject.h"


namespace iattr
{


/**
	Basic implementation of an property container.
*/
class CStandardAttributesFactory: virtual public iser::IObjectFactory
{
public:
	// reimplemented (istd::TIFactory<iser::IObject>)
	virtual iser::IObject* CreateInstance(const QByteArray& keyId = "") const;

	// reimplemented (istd::IFactoryInfo)
	virtual KeyList GetFactoryKeys() const;

	// static methods
	static const CStandardAttributesFactory& GetInstance();

private:
	static KeyList GetInitialFactoryKeys();

	// static attributes
	static KeyList s_factoryKeys;
	static CStandardAttributesFactory s_instance;
};


} // namespace iattr


#endif // !iattr_CStandardAttributesFactory_included


