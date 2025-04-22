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

	/**
		Initialize with given samples
		First sample is at the start of the range, last sample is at the end
	*/
	CSpectrum(int startWavelength, int endWavelength, const std::vector<double>& spectrumSamples);
	CSpectrum(const istd::CIntRange& wavelengthRange, const std::vector<double>& spectrumSamples);

	/**
		Zero initialize samples to fill range with given step
	 */
	CSpectrum(int startWavelength, int endWavelength, int step);
	CSpectrum(const istd::CIntRange& wavelengthRange, int step);

	// reimplemented (icmm::ISpectrumInfoProvider)
	virtual const ISpectrumInfo* GetSpectrumInfo() const override;

	// reimplemented (icmm::ISpectrumInfo)
	virtual istd::CIntRange GetSpectralRange() const override;
	virtual int GetStep() const override;
	virtual int GetSamplesCount() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

};


} // namespace icmm


