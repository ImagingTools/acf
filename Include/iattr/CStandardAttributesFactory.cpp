#include "iattr/CStandardAttributesFactory.h"


// ACF includes
#include "iattr/TAttribute.h"
#include "iattr/TMultiAttribute.h"


namespace iattr
{


static QByteArray integerAttrTypeName = CIntegerAttribute::GetTypeName();
static QByteArray realAttrTypeName = CRealAttribute::GetTypeName();
static QByteArray booleanAttrTypeName = CBooleanAttribute::GetTypeName();
static QByteArray stringAttrTypeName = CStringAttribute::GetTypeName();
static QByteArray idAttrTypeName = CIdAttribute::GetTypeName();

static QByteArray boolListAttrTypeName = CBooleanListAttribute::GetTypeName();
static QByteArray realListAttrTypeName = CRealListAttribute::GetTypeName();
static QByteArray integerListAttrTypeName = CIntegerListAttribute::GetTypeName();
static QByteArray stringListAttrTypeName = CStringListAttribute::GetTypeName();
static QByteArray idListAttrTypeName = CIdListAttribute::GetTypeName();


// reimplemented (istd::TIFactory<iser::IObject>)

iser::IObject* CStandardAttributesFactory::CreateInstance(const QByteArray& keyId) const
{
	if (keyId == integerAttrTypeName){
		return new CIntegerAttribute();
	}
	else if (keyId == realAttrTypeName){
		return new CRealAttribute();
	}
	else if (keyId == booleanAttrTypeName){
		return new CBooleanAttribute();
	}
	else if (keyId == stringAttrTypeName){
		return new CStringAttribute();
	}
	else if (keyId == idAttrTypeName){
		return new CIdAttribute();
	}
	else if (keyId == integerListAttrTypeName){
		return new CIntegerListAttribute();
	}
	else if (keyId == realListAttrTypeName){
		return new CRealListAttribute();
	}
	else if (keyId == boolListAttrTypeName){
		return new CBooleanListAttribute();
	}
	else if (keyId == stringListAttrTypeName){
		return new CStringListAttribute();
	}
	else if (keyId == idListAttrTypeName){
		return new CIdListAttribute();
	}
	else{
		return NULL;
	}
}


// reimplemented (istd::IFactoryInfo)

CStandardAttributesFactory::KeyList CStandardAttributesFactory::GetFactoryKeys() const
{
	return s_factoryKeys;
}


// static methods

const CStandardAttributesFactory& CStandardAttributesFactory::GetInstance()
{
	return s_instance;
}


// private static methods

CStandardAttributesFactory::KeyList CStandardAttributesFactory::GetInitialFactoryKeys()
{
	KeyList retVal;

	retVal.append(integerAttrTypeName);
	retVal.append(realAttrTypeName);
	retVal.append(booleanAttrTypeName);
	retVal.append(stringAttrTypeName);
	retVal.append(idAttrTypeName);
	retVal.append(integerListAttrTypeName);
	retVal.append(realListAttrTypeName);
	retVal.append(boolListAttrTypeName);
	retVal.append(stringListAttrTypeName);
	retVal.append(idListAttrTypeName);

	return retVal;
}


// static attributes
CStandardAttributesFactory::KeyList CStandardAttributesFactory::s_factoryKeys = CStandardAttributesFactory::GetInitialFactoryKeys();
CStandardAttributesFactory CStandardAttributesFactory::s_instance;


} // namespace iattr


