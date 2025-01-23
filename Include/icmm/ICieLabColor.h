#pragma once


// ACF includes
#include <icmm/IColorObject.h>
#include <icmm/CLab.h>


namespace icmm
{


class ITristimulusSpecification;


class ICieLabColor: virtual public IColorObject
{
public:
	/**
		Initialize object with color values.
	*/
	virtual bool Initialize(const icmm::CLab& value, const ITristimulusSpecification& spec) = 0;

	/**
		Get Lab-color value.
	*/
	virtual const icmm::CLab& GetLab() const = 0;

	/**
		Get color-related specification.
	*/
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const = 0;
};


} // namespace icmm


