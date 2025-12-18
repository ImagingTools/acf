#include <icmm/CIlluminant.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


// public methods

CIlluminant::CIlluminant(StandardIlluminant illuminantType)
	:m_illuminantType(illuminantType)
{
	InitFromStandardIlluminant(illuminantType);
}


CIlluminant::CIlluminant(const QString& illuminantName, const icmm::CVarColor& whitePoint)
	:m_illuminantType(icmm::StandardIlluminant::Custom),
	m_illuminantName(illuminantName),
	m_whitePoint(whitePoint)
{
}


CIlluminant::CIlluminant(const CIlluminant & other)
	:m_whitePoint(other.m_whitePoint),
	m_illuminantName(other.m_illuminantName),
	m_illuminantType(other.m_illuminantType)
{
}


// reimplemented (IWhitePointProvider)

icmm::CVarColor icmm::CIlluminant::GetWhitePoint() const
{
	return m_whitePoint;
}


// reimplemented (IIluminant)

void CIlluminant::SetWhitePoint(const icmm::CVarColor& whitePoint)
{
	if (m_whitePoint != whitePoint){
		istd::CChangeNotifier changeNotifier(this);

		m_whitePoint = whitePoint;
	}
}


QString CIlluminant::GetIlluminantName() const
{
	return m_illuminantName;
}


void CIlluminant::SetIlluminantName(const QString& illuminantName)
{
	if (m_illuminantName != illuminantName){
		istd::CChangeNotifier changeNotifier(this);

		m_illuminantName = illuminantName;
	}
}


StandardIlluminant CIlluminant::GetIlluminantType() const
{
	return m_illuminantType;
}


void CIlluminant::SetIlluminantType(const StandardIlluminant& illuminantType)
{
	if (m_illuminantType != illuminantType){
		istd::CChangeNotifier changeNotifier(this);

		m_illuminantType = illuminantType;
	}
}


// reimplemented (istd::IChangeable)

bool CIlluminant::IsEqual(const IChangeable& other) const
{
	const IIlluminant* objectPtr = dynamic_cast<const IIlluminant*>(&other);

	if (objectPtr == nullptr){
		return false;
	}

	return
				m_illuminantType == objectPtr->GetIlluminantType() &&
				m_illuminantName == objectPtr->GetIlluminantName() &&
				m_whitePoint == objectPtr->GetWhitePoint();
}



// reimplemented (iser::ISerializable)

bool CIlluminant::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	iser::CArchiveTag illuminantTypeTag("IlluminantType", "Illuminant", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(illuminantTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_illuminantType, &icmm::staticMetaObject);
	retVal = retVal && archive.EndTag(illuminantTypeTag);

	iser::CArchiveTag illuminantNameTag("IlluminantName", "Name of the illuminant", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(illuminantNameTag);
	retVal = retVal && archive.Process(m_illuminantName);
	retVal = retVal && archive.EndTag(illuminantNameTag);

	iser::CArchiveTag whitePointTag("WhitePoint", "White point of the illuminant", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(whitePointTag);
	retVal = retVal && m_whitePoint.Serialize(archive);
	retVal = retVal && archive.EndTag(whitePointTag);

	return retVal;
}


// protected methods

void CIlluminant::InitFromStandardIlluminant(StandardIlluminant illuminantType)
{
	/// \todo Implement white point for each standard illuminant types.

	switch (illuminantType){
	case icmm::StandardIlluminant::A:
		m_illuminantName = "A";
		break;
	case icmm::StandardIlluminant::B:
		m_illuminantName = "B";
		break;
	case icmm::StandardIlluminant::C:
		m_illuminantName = "C";
		break;
	case icmm::StandardIlluminant::D50:
		m_illuminantName = "D50";
		break;
	case icmm::StandardIlluminant::D55:
		m_illuminantName = "D55";
		break;
	case icmm::StandardIlluminant::D60:
		m_illuminantName = "D60";
		break;
	case icmm::StandardIlluminant::D65:
		m_illuminantName = "D65";
		break;
	case icmm::StandardIlluminant::D75:
		m_illuminantName = "D75";
		break;
	case icmm::StandardIlluminant::E:
		m_illuminantName = "E";
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}


} // namespace icmm


