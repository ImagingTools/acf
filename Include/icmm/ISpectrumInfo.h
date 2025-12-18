#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <istd/TRange.h>


namespace icmm
{


/**
	Common interface for describing the properties or meta-informations of a measured/sampled spectrum.
*/
class ISpectrumInfo: virtual public istd::IChangeable
{
public:
	/**
		Get the spectral range of the measured values given in nm.
	*/
	virtual istd::CIntRange GetSpectralRange() const = 0;

	/**
		Get distance/step between the samples in the spectrum definition (in nm).
		We assume that we have regular sampling grid.
	*/
	virtual int GetStep() const = 0;

	/**
		Return number of samples
	 */
	virtual int GetSamplesCount() const = 0;
};


} // namespace icmm
