#include <icmm/CSpectralColorSpecification.h>


namespace icmm
{



CSpectralColorSpecification::CSpectralColorSpecification()
{
}


CSpectralColorSpecification::CSpectralColorSpecification(const ISpectrumInfo& spectrumInfo)
{
	m_info.CopyFrom(spectrumInfo);
}



// reimplemented (ISpectrumInfoProvider)

const ISpectrumInfo* CSpectralColorSpecification::GetSpectrumInfo() const
{
	return &m_info;
}


} // namespace icmm


