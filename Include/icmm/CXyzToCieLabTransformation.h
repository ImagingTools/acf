#pragma once


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of XYZ-to-CIE-Lab color transformation.
	2D Observer and D65-illumination are used.
*/
class CXyzToCieLabTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const override;
	virtual icmm::CVarColor GetValueAt(const icmm::CVarColor& argument) const override;
};


} // namespace icmm


