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


enum class AstmTableType
{
	E308Table5,
	E308Table6,
	E3081nm,
	Unknown
};


enum class SphereType
{
	SpecularIncluded,
	SpecularExcluded,
	Diffuse
};


enum class StandardIlluminant
{
	A,
	B,
	C,
	D50,
	D55,
	D60,
	D65,
	D75,
	E,
	F2,
	F3,
	F7,
	F9,
	F10,
	F11,
	F12,
	Illuminant9300,
	TL83,
	YL84,
	UL30,
	UL35,
	UL50,
	Custom
};


enum class ObserverType
{
	TwoDegree,
	TenDegree
};


/**
	Density filter type as defined by CxF standard
 */
enum class DensityFilterType
{
	Visual,
	Cyan,
	Magenta,
	Yellow,
	Black,
	Red,
	Green,
	Blue,
	A,
	B
};


enum class SpectrumType
{
	Reflectance,
	Transmittance,
	TotalTransmittance,
	Emissive,
	ColorimetricReflectance,
	ColorimetricTransmittance,
	ColorimetricEmissive
};


/**
	Density status type as defined by CxF standard
*/
enum class DensityStatusType
{ 
	A,
	E,
	I,
	M,
	T,
	SpectralX,
	Spectral,
	HiFi,
	Hex,
	Txp,
	Ex,
	DIN,
	DIN_NB,
	PD,
	APD
};


// Device-related

enum class DeviceTypeFlags
{
	Spot = 1,
	Scanning = 2
};


enum class FilterType
{
	None,
	UvExcluded,
	UvD65,
	Partial,
	Custom
};


enum class DeviceIlluminationType
{
	Incandescent, // M0
	Daylight, // M1
	UvExculded, //M2
	Polarized, //M3
	Flash,
	Led,
	Ambient,
	Custom
};


enum class EmmisiveModelType
{
	Diffuser,
	Reflected,
	Other
};


enum class SingleAngleConfigurationType
{
	Annular,
	Uniplanar
};


/**
	Usage of a colorant.
*/
enum ColorantUsage
{
	/**
		No special usage of the colorant.
	*/
	CU_NONE,
	CU_CYAN,
	CU_DARK_CYAN,
	CU_LIGHT_CYAN,
	CU_MAGENTA,
	CU_DARK_MAGENTA,
	CU_LIGHT_MAGENTA,
	CU_YELLOW,
	CU_DARK_YELLOW,
	CU_LIGHT_YELLOW,
	CU_BLACK,
	CU_LIGHT_BLACK,
	CU_LIGHT_LIGHT_BLACK,

	/**
		Colorant is used for expanding the gamut of the printing device.
	*/
	CU_ECG,

	/**
		Colorant is used as a spot color.
	*/
	CU_SPOT
};

I_DECLARE_ENUM(
			ColorantUsage,
			CU_NONE,
			CU_CYAN,
			CU_DARK_CYAN,
			CU_LIGHT_CYAN,
			CU_MAGENTA,
			CU_DARK_MAGENTA,
			CU_LIGHT_MAGENTA,
			CU_YELLOW,
			CU_DARK_YELLOW,
			CU_LIGHT_YELLOW,
			CU_BLACK,
			CU_LIGHT_BLACK,
			CU_LIGHT_LIGHT_BLACK,
			CU_ECG,
			CU_SPOT);


} // namespace icmm


