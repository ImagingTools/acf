#pragma once


// ACF includes
#include <imath/CSampledFunction.h>
#include <iser/ISerializable.h>
#include <icmm/ISpectrum.h>
#include <icmm/ISpectrumInfo.h>


namespace icmm
{


/**
	Standard spectrum implementation.
*/
class CSpectrum:
			public imath::CSampledFunction,
			virtual public ISpectrum,
			virtual protected ISpectrumInfo,
			virtual public iser::ISerializable
{
public:
	typedef imath::CSampledFunction BaseClass;

	CSpectrum();
	CSpectrum(const CSpectrum& other);
	CSpectrum(int startWavelength, int endWavelength, int step, const std::vector<double>& spectrumSamples);
	CSpectrum(const istd::CIntRange& wavelengthRange, int step, const std::vector<double>& spectrumSamples);

	// reimplemented (icmm::ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;

	// reimplemented (icmm::ISpectrumInfo)
	virtual istd::CIntRange GetSpectralRange() const override;
	virtual int GetStep() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	int m_step;
};


} // namespace icmm


