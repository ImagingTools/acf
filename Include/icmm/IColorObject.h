#pragma once


// ACF includes
#include <icmm/IColorModel.h>
#include <icmm/CVarColor.h>
#include <iser/IObject.h>


namespace icmm
{


class IColorObject: virtual public iser::IObject
{
public:
	/**
		Get a generic color value.
	*/
	virtual icmm::CVarColor GetColor() const = 0;

	/**
		Get related color model.
	*/
	virtual ConstColorModelPtr GetColorModel() const = 0;

	/**
		Get a generic color value.
	*/
	virtual std::unique_ptr<IColorObject> CloneIntoUniquePtr() const = 0;
};


} // namespace icmm


