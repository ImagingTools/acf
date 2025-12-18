#include <icmm/CSpectralColorSpecificationBase.h>


// ACF includes
#include <iser/IArchive.h>
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{



CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(const ISpectrumInfo& spectrumInfo)
{
	m_info.CopyFrom(spectrumInfo);
}


CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(istd::CIntRange range, int step)
{
	m_info.SetRange(range);
	m_info.SetStep(step);
}


CSpectralColorSpecificationBase::~CSpectralColorSpecificationBase()
{
}


CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(const CSpectralColorSpecificationBase& spec)
{
	m_info.CopyFrom(*spec.GetSpectrumInfo());
	m_spectrumType = spec.GetSpectrumType();
}


// reimplemented (ISpectrumInfoProvider)

const ISpectrumInfo* CSpectralColorSpecificationBase::GetSpectrumInfo() const
{
	return &m_info;
}



ISpectralColorSpecification::SpectrumType CSpectralColorSpecificationBase::GetSpectrumType() const
{
	return m_spectrumType;
}



// reimplemented (iser::ISerializable)

bool CSpectralColorSpecificationBase::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring() ? NULL : this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	iser::CArchiveTag spectrumTypeTag("SpectrumType", "Type if used spectrum", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(spectrumTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_spectrumType, &icmm::staticMetaObject);
	retVal = retVal && archive.EndTag(spectrumTypeTag);

	iser::CArchiveTag spectrumInfoTag("SpectrumInfo", "Information about spectrum properties", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(spectrumInfoTag);
	retVal = retVal && m_info.Serialize(archive);
	retVal = retVal && archive.EndTag(spectrumInfoTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CSpectralColorSpecificationBase::IsEqual(const IChangeable& other) const
{
	const CSpectralColorSpecificationBase* objectPtr = dynamic_cast<const CSpectralColorSpecificationBase*>(&other);
	if (objectPtr == nullptr){
		return false;
	}

	bool isTypeSame = m_spectrumType == objectPtr->GetSpectrumType();
	bool isInfoSame = m_info.IsEqual(*objectPtr->GetSpectrumInfo());

	return isTypeSame && isInfoSame;
}


} // namespace icmm


