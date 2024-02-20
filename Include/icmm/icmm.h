#pragma once


// ACF includes
#include <imath/imath.h>


/**
	Contains color management classes.
	This package is system undependent.
	This package use following ACF packages:
	\li	istd
	\li	iser
	\li	imath

	\ingroup Color
*/
namespace icmm
{


enum ColorantType
{
	CT_NONE,
	CT_PROCESS,
	CT_SPOT
};

I_DECLARE_ENUM(ColorantType, CT_NONE, CT_PROCESS, CT_SPOT);


/**
	Usage of a process colorant.
*/
enum ProcessColorantUsage
{
	/**
		No special usage of the process colorant.
	*/
	PCU_NONE,

	/**
		Colorant is used as a darker version of another process colorant.
	*/
	PCU_DARK,

	/**
		Colorant is used as a lighter version of another process colorant.
	*/
	PCU_LIGHT,

	/**
		Colorant is used for expanding the gamut of the printing device.
	*/
	PCU_ECG
};

I_DECLARE_ENUM(ProcessColorantUsage, PCU_NONE, PCU_DARK, PCU_LIGHT, PCU_ECG);



} // namespace icmm


