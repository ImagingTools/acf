#ifndef icmm_TComposedColorGradient_included
#define icmm_TComposedColorGradient_included	


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <istd/TRange.h>
#include <icmm/TColorGradient.h>


namespace icmm
{


/**
	Implementation of color gradient based 
	on set of defined gradients for each subintervall of the full range.
*/
template <class Gradient>
class TComposedColorGradient: virtual public IColorTransformation
{
public:
	void AddGradient(const istd::CRange& intervall, const Gradient& gradient);

	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const Argument& argument, Result& result) const;
	virtual Result GetValueAt(const Argument& argument) const;

private:
	typedef QPair<istd::CRange, Gradient> IntervallGradient;
	typedef QVector<IntervallGradient> Gradients;
	
	Gradients m_gradients;
};


// public methods

template <class GradientFunction>
void TColorGradient<GradientFunction>::AddGradient(const istd::CRange& intervall, const Gradient& gradient)
{
	m_gradients.push_back(std::make_pair<istd::CRange, Gradient>(intervall, gradient));
}


// reimplemented (icmm::IColorTransformation)

template <class GradientFunction>
bool TColorGradient<GradientFunction>::GetValueAt(const Argument& argument, Result& result) const
{
	for (int gradientIndex = 0; gradientIndex < int(m_gradients.size()); gradientIndex++){
		IntervallGradient gradient = m_gradients[gradientIndex];

		if (gradient.first.Contains(argument)){
			return gradient.second.GetValueAt(argument, result);
		}
	}
	
	return false;
}


template <class GradientFunction>
TColorGradient<GradientFunction>::Result TColorGradient<GradientFunction>::GetValueAt(const Argument& argument) const
{
	ResultType result;

	GetValueAt(argument, result);

	return result;
}


typedef TComposedColorGradient<CLinearGradient> CComposedLinearGradient;


} // namespace icmm


#endif // !icmm_TComposedColorGradient_included


