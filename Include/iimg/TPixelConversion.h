#ifndef iimg_TPixelConversion_included
#define iimg_TPixelConversion_included


// ACF includes
#include <iimg/iimg.h>


namespace iimg
{

	
/**
	A generic pixel conversion.

	\tparam	SourcePixel	Type of the source pixel.
	\tparam	DestPixel  	Type of the destination pixel.
	\tparam	DestPixel  	Type of the destination pixel.
*/
template <typename SourcePixel, typename DestPixel, typename CalcPixel = DestPixel>
class TPixelConversion
{
public:
	typedef SourcePixel SourcePixelType;
	typedef DestPixel DestPixelType;
	typedef CalcPixel CalcPixelType;


	/**
		Default constructor.
	*/
	TPixelConversion();

	/**
		 Casting operator.
		
		\return	The result of the pixel conversion.
	*/
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
inline DestPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::operator()(const SourcePixel& pixel) const
{
	return DestPixel(pixel);
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline CalcPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetCalc(const SourcePixel& pixel) const
{
	return CalcPixel(pixel);
}


template <typename SourcePixel, typename DestPixel, typename CalcPixel>
inline DestPixel TPixelConversion<SourcePixel, DestPixel, CalcPixel>::GetDest(const CalcPixel& pixel) const
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
inline TPixelConversion<quint8, quint8, quint8>::TPixelConversion()
:	whiteIntensity(255)
{
}


template <>
inline float TPixelConversion<quint8, float, float>::operator()(const quint8& pixel) const
{
	return pixel / 255.0f;
}


template <>
inline float TPixelConversion<quint8, float, float>::GetCalc(const quint8& pixel) const
{
	return pixel / 255.0f;
}


template <>
inline quint8 TPixelConversion<float, quint8, float>::operator()(const float& pixel) const
{
	return quint8(pixel * 255);
}


template <>
inline quint8 TPixelConversion<float, quint8, float>::GetDest(const float& pixel) const
{
	return quint8(pixel * 255);
}


typedef TPixelConversion<quint8, quint8> CGrayGrayPixelConversion;
typedef TPixelConversion<float, quint8, float> CFloatGrayPixelConversion;
typedef TPixelConversion<quint8, float, float> CGrayFloatPixelConversion;
typedef TPixelConversion<float, float> CFloatFloatPixelConversion;


} // namespace iimg


#endif // !iimg_TPixelConversion_included


