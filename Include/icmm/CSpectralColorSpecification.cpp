#include <icmm/CSpectralColorSpecification.h>


namespace icmm
{



CSpectralColorSpecification::CSpectralColorSpecification(const ISpectrumInfo& spectrumInfo)
{
	m_info.CopyFrom(spectrumInfo);
}

CSpectralColorSpecification::CSpectralColorSpecification(istd::CIntRange range, int step)
{
	m_info.SetRange(range);
	m_info.SetStep(step);
}

// reimplemented (ISpectrumInfoProvider)

const ISpectrumInfo* CSpectralColorSpecification::GetSpectrumInfo() const
{
	return &m_info;
}


} // namespace icmm


