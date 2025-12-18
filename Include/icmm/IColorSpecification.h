#pragma once


// STL includes
#include <memory>

// ACF includes
#include <istd/IChangeable.h>
#include <icmm/icmm.h>


namespace icmm
{


class IColorSpecification: virtual public istd::IChangeable
{
public:
	typedef std::shared_ptr<const IColorSpecification> ConstColorSpecPtr;
	typedef std::shared_ptr<IColorSpecification> ColorSpecPtr;

	enum class SpecType
	{
		Tristimulus,
		Spectral
	};

	/**
		Get the logical type of the specification. Can be tristimulus or spectral-based.
		\sa SpecType
	*/
	virtual SpecType GetSpecificationType() const = 0;
};


} // namespace icmm


