#include <icmm/CSpectralColorSpecificationBase.h>


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

// reimplemented (ISpectrumInfoProvider)
CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(const CSpectralColorSpecificationBase& spec)
{
	m_info.CopyFrom(*spec.GetSpectrumInfo());
	m_spectrumType = spec.GetSpectrumType();
}

const ISpectrumInfo* CSpectralColorSpecificationBase::GetSpectrumInfo() const
{
	return &m_info;
}

bool CSpectralColorSpecificationBase::IsEqual(const IChangeable& other) const
{
	const CSpectralColorSpecificationBase* objectPtr =
		dynamic_cast<const CSpectralColorSpecificationBase*>(&other);

	if (objectPtr == nullptr) {
		return false;
	}

	bool isTypeSame = m_spectrumType == objectPtr->GetSpectrumType();
	bool isInfoSame = m_info.IsEqual(*objectPtr->GetSpectrumInfo());

	return isTypeSame && isInfoSame;

}

ISpectralColorSpecification::SpectrumType CSpectralColorSpecificationBase::GetSpectrumType() const
{
	return m_spectrumType;
}

} // namespace icmm


