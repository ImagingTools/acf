#include <i2d/CObject2dFactory.h>


// ACF includes
#include <imod/TModelWrap.h>

#include <i2d/CLine2d.h>
#include <i2d/CCircle.h>
#include <i2d/CAnnulus.h>
#include <i2d/CAnnulusSegment.h>
#include <i2d/CRectangle.h>
#include <i2d/CPolypoint.h>
#include <i2d/CPolyline.h>
#include <i2d/CPolygon.h>
#include <i2d/CPosition2d.h>
#include <i2d/CLabel.h>

namespace i2d
{


static QByteArray s_line2dTypeId = CLine2d::GetTypeName();
static QByteArray s_label2dTypeId = CLine2d::GetTypeName();
static QByteArray s_circle2dTypeId = CCircle::GetTypeName();
static QByteArray s_ring2dTypeId = CAnnulus::GetTypeName();
static QByteArray s_ringSegment2dTypeId = CAnnulusSegment::GetTypeName();
static QByteArray s_rect2dTypeId = CRectangle::GetTypeName();
static QByteArray s_polypoint2dTypeId = CPolypoint::GetTypeName();
static QByteArray s_polyline2dTypeId = CPolyline::GetTypeName();
static QByteArray s_polygon2dTypeId = CPolygon::GetTypeName();
static QByteArray s_position2dTypeId = CPosition2d::GetTypeName();


// reimplemented (istd::TIFactory<iser::IObject>)

iser::IObject* CObject2dFactory::CreateInstance(const QByteArray& keyId) const
{
	if (keyId == s_line2dTypeId){
		return new imod::TModelWrap<CLine2d>();
	}
	else if (keyId == s_label2dTypeId){
		return new imod::TModelWrap<CLabel>();
	}
	else if (keyId == s_position2dTypeId){
		return new imod::TModelWrap<CPosition2d>();
	}
	else if (keyId == s_polypoint2dTypeId){
		return new imod::TModelWrap<CPolypoint>();
	}
	else if (keyId == s_polyline2dTypeId){
		return new imod::TModelWrap<CPolyline>();
	}
	else if (keyId == s_polygon2dTypeId){
		return new imod::TModelWrap<CPolygon>();
	}
	else if (keyId == s_circle2dTypeId){
		return new imod::TModelWrap<CCircle>();
	}
	else if (keyId == s_ring2dTypeId){
		return new imod::TModelWrap<CAnnulus>();
	}
	else if (keyId == s_ringSegment2dTypeId){
		return new imod::TModelWrap<CAnnulusSegment>();
	}
	else if (keyId == s_rect2dTypeId){
		return new imod::TModelWrap<CRectangle>();
	}
	else{
		return NULL;
	}
}


// reimplemented (istd::IFactoryInfo)

CObject2dFactory::KeyList CObject2dFactory::GetFactoryKeys() const
{
	return s_factoryKeys;
}


// static methods

const CObject2dFactory& CObject2dFactory::GetInstance()
{
	return s_instance;
}


// private static methods

CObject2dFactory::KeyList CObject2dFactory::GetInitialFactoryKeys()
{
	KeyList retVal;

	retVal.insert(s_line2dTypeId);

	return retVal;
}


// static attributes

CObject2dFactory::KeyList CObject2dFactory::s_factoryKeys = CObject2dFactory::GetInitialFactoryKeys();

CObject2dFactory CObject2dFactory::s_instance;


} // namespace i2d


