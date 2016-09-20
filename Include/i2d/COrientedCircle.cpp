#include <i2d/COrientedCircle.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <i2d/CPolyline.h>


namespace i2d
{


// static constants
static const iser::CArchiveTag s_orientatedOutsideTag("IsOrientedOutside", "Flag if this circle is oriented inside or outside", iser::CArchiveTag::TT_LEAF);


COrientedCircle::COrientedCircle()
:	m_orientedOutside(false)
{
}


COrientedCircle::COrientedCircle(double radius, const CVector2d& center, bool orientatedOutside)
:	BaseClass(radius, center), m_orientedOutside(orientatedOutside)
{
}


void COrientedCircle::SetOrientedOutside(bool orientatedOutside)
{
	if (orientatedOutside != m_orientedOutside){
		istd::CChangeNotifier notifier(this);

		m_orientedOutside = orientatedOutside;
	}
}


bool COrientedCircle::operator==(const COrientedCircle& ref) const
{
	return BaseClass::operator==(ref) && (ref.m_orientedOutside == m_orientedOutside);
}


bool COrientedCircle::operator!=(const COrientedCircle& ref) const
{
	return BaseClass::operator!=(ref) || (ref.m_orientedOutside != m_orientedOutside);
}


// reimplemented (i2d::CCircle)

bool COrientedCircle::ConvertToPolygon(i2d::CPolygon& result, int segmentsCount) const
{
	if (BaseClass::ConvertToPolygon(result, segmentsCount)){
		if (IsOrientedOutside()){
			result.ReverseNodes();
		}
		
		return true;
	}
	
	return false;
}


// reimplemented (istd::IChangeable)

int COrientedCircle::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool COrientedCircle::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const COrientedCircle* orientedCirclePtr = dynamic_cast<const COrientedCircle*>(&object);
	if (orientedCirclePtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetPosition(orientedCirclePtr->GetPosition());
		SetRadius(orientedCirclePtr->GetRadius());
		SetOrientedOutside(orientedCirclePtr->IsOrientedOutside());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	const CCircle* circlePtr = dynamic_cast<const CCircle*>(&object);
	if (circlePtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetPosition(circlePtr->GetPosition());
		SetRadius(circlePtr->GetRadius());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* COrientedCircle::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<COrientedCircle> clonePtr(new COrientedCircle);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool COrientedCircle::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(s_orientatedOutsideTag);
	retVal = retVal && archive.Process(m_orientedOutside);
	retVal = retVal && archive.EndTag(s_orientatedOutsideTag);

	return retVal;
}


} // namespace i2d


