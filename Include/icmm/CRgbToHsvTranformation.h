#pragma once


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of HSV-to-RGB color transformation.
*/
class CRgbToHsvTranformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;
};


} // namespace icmm


