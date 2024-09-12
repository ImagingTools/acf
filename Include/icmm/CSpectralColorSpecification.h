#pragma once


// ACF includes
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CSpectrumInfo.h>


namespace icmm
{


class CSpectralColorSpecification: virtual public ISpectralColorSpecification
{
public:
	CSpectralColorSpecification(const ISpectrumInfo& spectrumInfo);
	CSpectralColorSpecification(istd::CIntRange range, int step);

	// reimplemented (ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;

private:
	CSpectrumInfo m_info;
};


} // namespace icmm


