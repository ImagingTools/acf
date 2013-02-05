#ifndef icmm_TColorGradient_included
#define icmm_TColorGradient_included


#include "imath/TIMathFunction.h"

#include "icmm/CVarColor.h"


namespace icmm
{


class LinearGradientFunction
{
public:
	static double GetValue(double x)
	{
		return x;
	}
};


/**
	Simple implementation of gradient based on interpolation beetween the start und end colors.
*/
template <class GradientFunction>
class TColorGradient: virtual public IColorTransformation
{
public:
	typedef IColorTransformation::ResultType GradientColor;

	TColorGradient(const GradientColor& startColor, const GradientColor& endColor);

	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

private:
	GradientColor m_startColor;
	GradientColor m_endColor;
};


template <class GradientFunction>
TColorGradient<GradientFunction>::TColorGradient(const GradientColor& startColor, const GradientColor& endColor)
	:m_startColor(startColor),
	m_endColor(endColor)
{
}


// reimplemented (icmm::IColorTransformation)

template <class GradientFunction>
bool TColorGradient<GradientFunction>::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	Q_ASSERT(argument.GetElementsCount() == 1);

	static istd::CRange normRange(0.0, 1.0);

	double argumentValue = normRange.GetClipped(argument[0]);
	
	for (int componentIndex = 0; componentIndex < result.GetElementsCount(); componentIndex++){
		double startValue = m_startColor[componentIndex];
		double endValue = m_endColor[componentIndex];

		double gradientValue = 0.0; 
		if (startValue < endValue){
			gradientValue = (endValue - startValue) * argumentValue + startValue;
		}
		else{
			gradientValue = (startValue - endValue) * (1.0 - argumentValue) + endValue;
		}

		result[componentIndex] = GradientFunction::GetValue(gradientValue);
	}

	return true;
}


template <class GradientFunction>
typename TColorGradient<GradientFunction>::ResultType TColorGradient<GradientFunction>::GetValueAt(const ArgumentType& argument) const
{
	ResultType result(m_startColor.GetElementsCount());

	GetValueAt(argument, result);

	return result;
}


typedef TColorGradient<LinearGradientFunction> CLinearColorGradient;


} // namespace icmm


#endif // !icmm_TColorGradient_included


