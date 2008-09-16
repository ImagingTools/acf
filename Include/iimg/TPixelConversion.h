#ifndef iimg_TPixelConversion_included
#define iimg_TPixelConversion_included


#include "iimg/iimg.h"


namespace iimg
{


template <typename SourcePixel, typename DestPixel, typename CalcPixel = DestPixel>
class TPixelConversion
{
public:
	typedef SourcePixel SourcePixelType;
	typedef DestPixel DestPixelType;
	typedef CalcPixel CalcPixelType;

	TPixelConversion();

	DestPixel operator()(const SourcePixel& pixel) const;

	CalcPixel GetCalc(const SourcePixel& pixel) const;
	DestPixel GetDest(const CalcPixel& pixel) const;

	/**
		Get weight of pixel value.
		Weight is value, typically between 0 and 1, describing importance of pixel.
		For normal pixels it is always 1.
		For alpha-channeled pixels or weight pixels it can differ.
		Values above 1 and negative are also possible for some algorithmical reasons.
	*/
	double GetWeight(const CalcPixel& pixel) const;

	/**
		Get intensity of pixel value.
		This value is used for grayscale algorithms to operate with pixels with additional than intensity information.
	*/
	double GetIntensity(const CalcPixel& pixel) const;

	double whiteIntensity;
};


// inline methods

template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline TPixelConversion<SourcePixel, DestPixel, CalcPixel>::TPixelConversion()
:	whiteIntensity(1)
{
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline typename DestPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::operator()(const SourcePixel& pixel) const
{
	return DestPixel(pixel);
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline typename CalcPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetCalc(const SourcePixel& pixel) const
{
	return CalcPixel(pixel);
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline typename DestPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetDest(const CalcPixel& pixel) const
{
	return DestPixel(pixel);
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline double TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetWeight(const CalcPixel& /*pixel*/) const
{
	return 1.0;
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline double TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetIntensity(const CalcPixel& pixel) const
{
	return double(pixel);
}


template <>
inline TPixelConversion<I_BYTE, I_BYTE, I_BYTE>::TPixelConversion()
:	whiteIntensity(255)
{
}


template <>
inline typename float TPixelConversion<I_BYTE, float, float>::operator()(const I_BYTE& pixel) const
{
	return pixel / 255.0f;
}


template <>
inline typename float TPixelConversion<I_BYTE, float, float>::GetCalc(const I_BYTE& pixel) const
{
	return pixel / 255.0f;
}


template <>
inline typename I_BYTE TPixelConversion<float, I_BYTE, float>::operator()(const float& pixel) const
{
	return I_BYTE(pixel * 255);
}


template <>
inline typename I_BYTE TPixelConversion<float, I_BYTE, float>::GetDest(const float& pixel) const
{
	return I_BYTE(pixel * 255);
}


typedef TPixelConversion<I_BYTE, I_BYTE> CGrayGrayPixelConversion;
typedef TPixelConversion<float, I_BYTE, float> CFloatGrayPixelConversion;
typedef TPixelConversion<I_BYTE, float, float> CGrayFloatPixelConversion;
typedef TPixelConversion<float, float> CFloatFloatPixelConversion;


} // namespace iimg


#endif // !iimg_TPixelConversion_included


