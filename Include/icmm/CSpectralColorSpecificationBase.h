#pragma once


// ACF includes
#include <icmm/ISpectralColorSpecification.h>
#include <icmm/CSpectrumInfo.h>


namespace icmm
{


class CSpectralColorSpecificationBase: virtual public ISpectralColorSpecification
{
public:
	virtual ~CSpectralColorSpecificationBase();

	CSpectralColorSpecificationBase(const CSpectralColorSpecificationBase& spec);

	// reimplemented (ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;
	virtual SpectrumType GetSpectrumType() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const istd::IChangeable& other) const override;

protected:
	CSpectralColorSpecificationBase(const ISpectrumInfo& spectrumInfo);
	CSpectralColorSpecificationBase(istd::CIntRange range, int step);

	SpectrumType m_spectrumType;

private:
	CSpectrumInfo m_info;
};


} // namespace icmm


