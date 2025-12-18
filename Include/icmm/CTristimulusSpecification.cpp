#include <icmm/CTristimulusSpecification.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


CTristimulusSpecification::CTristimulusSpecification(
			ObserverType observerType,
			AstmTableType method,
			std::shared_ptr<IIlluminant> illuminantPtr,
			std::shared_ptr<ISpectralColorSpecification> baseSpec)
:	m_observerType(observerType),
	m_method(method),
	m_illuminantPtr(illuminantPtr),
	m_baseSpecPtr(baseSpec)
{
}


CTristimulusSpecification::CTristimulusSpecification(const ITristimulusSpecification& other)
:	m_observerType(other.GetObserverType()),
	m_method(other.GetMethod()),
	m_illuminantPtr(other.GetIlluminant()),
	m_baseSpecPtr(other.GetBaseSpecification())
{
}


// reimplemented (ITristimulusSpecification)

std::shared_ptr<IIlluminant> icmm::CTristimulusSpecification::GetIlluminant() const
{
	return m_illuminantPtr;
}


ObserverType CTristimulusSpecification::GetObserverType() const
{
	return m_observerType;
}


AstmTableType CTristimulusSpecification::GetMethod() const
{
	return m_method;
}


std::shared_ptr<ISpectralColorSpecification> CTristimulusSpecification::GetBaseSpecification() const
{
	return m_baseSpecPtr;
}


// reimplemented (istd::IChangeable)

bool CTristimulusSpecification::IsEqual(const IChangeable& other) const
{
	const CTristimulusSpecification* objectPtr = dynamic_cast<const CTristimulusSpecification*>(&other);
	if (objectPtr == nullptr){
		return false;
	}

	return *this == *objectPtr;
}


// reimplemented (iser::ISerializable)

bool CTristimulusSpecification::Serialize(iser::IArchive& archive)
{
	if (m_illuminantPtr == nullptr) {
		return false;
	}

	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	iser::CArchiveTag illuminantTag("IlluminantType", "Illuminant", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(illuminantTag);
	retVal = retVal && m_illuminantPtr->Serialize(archive);
	retVal = retVal && archive.EndTag(illuminantTag);

	iser::CArchiveTag observerTypeTag("ObserverType", "Observer Type", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(observerTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_observerType, &icmm::staticMetaObject);
	retVal = retVal && archive.EndTag(observerTypeTag);

	iser::CArchiveTag astmTableTypeTag("AstmTableType", "ASTM table entry", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(astmTableTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_method, &icmm::staticMetaObject);
	retVal = retVal && archive.EndTag(astmTableTypeTag);

	iser::CArchiveTag baseSpecTag("BaseSpec", "Base specification", iser::CArchiveTag::TT_WEAK, nullptr, true);
	retVal = retVal && archive.BeginTag(baseSpecTag);
	if (m_baseSpecPtr != nullptr) {
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_method, &icmm::staticMetaObject);
	}
	retVal = retVal && archive.EndTag(baseSpecTag);

	return retVal;
}


bool CTristimulusSpecification::operator==(const CTristimulusSpecification& other) const
{
	return m_observerType == other.m_observerType
		&& m_method == other.m_method
		&& istd::AreObjectsEqual(m_illuminantPtr.get(), other.m_illuminantPtr.get())
		&& istd::AreObjectsEqual(m_baseSpecPtr.get(), other.m_baseSpecPtr.get());
}


const icmm::CTristimulusSpecification& CTristimulusSpecification::GetD50TwoDegree()
{
	static const icmm::CTristimulusSpecification spec(
		icmm::ObserverType::TwoDegree,
		icmm::AstmTableType::Unknown,
		std::make_shared<icmm::CIlluminant>(icmm::StandardIlluminant::D50));

	return spec;
}


} // namespace icmm


