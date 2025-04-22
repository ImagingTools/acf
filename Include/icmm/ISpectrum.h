#pragma once


// ACF includes
#include <imath/TISampledFunction.h>
#include <icmm/ISpectrumInfoProvider.h>
#include <iser/ISerializable.h>


namespace icmm
{


/**
	Common interface for a measured/sampled spectrum.
	Spectrum is described as container of samples in the regular 1D-grid.
	Sampling grid information can be retrieved via ISpectrumInfoProvider interface.
	Sample values of the spectrum are accessable via imath::ISampledFunction interface
	For getting the "real" spectral values use imath::IMathfunction interface.
	Function domain is defined by the spectral range (\sa ISpectralInfo)
	Function range is normally [0, 1],
	but can also be acquired by \c GetResultValueRange method of imath::TISampledFunction.
*/
class ISpectrum:
			virtual public imath::ISampledFunction,
			virtual public ISpectrumInfoProvider,
			virtual public iser::ISerializable
{
};


} // namespace icmm


