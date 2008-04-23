#ifndef icmm_CColorTransformationProvider_included
#define icmm_CColorTransformationProvider_included


#include "icmm/CHsvToRgbTransformation.h"
#include "icmm/CRgbToCmykTransformation.h"
#include "icmm/CCmykToRgbTransformation.h"


namespace icmm
{


/**
*/
class CColorTransformationProvider: public istd::IPolymorphic
{
public:
	enum ColorType
	{
		HsvColor = 0x0001,
		CmykColor = 0x0010,
		RgbColor = 0x0020,
		LabColor = 0x0040
	};

	CColorTransformationProvider();

	static IColorTransformation* GetColorTransformation(int inputColorType, int outputColorType);

private:
	static icmm::CHsvToRgbTransformation s_hsvToRgbTransform;
	static icmm::CRgbToCmykTransformation s_rgbToCmykTransform;
	static icmm::CCmykToRgbTransformation s_cmykToRgbTransform;
};

} // namespace icmm


#endif // !icmm_CColorTransformationProvider_included


