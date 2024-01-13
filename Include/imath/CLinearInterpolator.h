#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imath/ISampledFunctionInterpolator.h>


namespace imath
{


/**
	Implementation of a linear interpolator.
*/
class CLinearInterpolator: virtual public imath::ISampledFunctionInterpolator
{
public:
	CLinearInterpolator();

	/**
		\param isExtrapolationEnabled	If enabled, the values will be extrapolatied also outside of defined domain, otherwise returned value for argument outside[positionMin, positionMax] is the last value.
	*/
	CLinearInterpolator(double* positions, double* values, int nodesCount, bool isExtrapolationEnabled = false);

	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::ISampledFunctionInterpolator)
	virtual bool InitFromFunction(const ISampledFunction& function) override;

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const override;
	virtual double GetValueAt(const double& argument) const override;

private:
	typedef QMap<double, double> Nodes;
	Nodes m_nodes;
	bool m_isExtrapolationEnabled;
};


} // namespace imath


