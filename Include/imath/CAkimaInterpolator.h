#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imath/ISampledFunctionInterpolator.h>


namespace imath
{


/**
	1D interpolation using Akima method.
*/
class CAkimaInterpolator: virtual public imath::ISampledFunctionInterpolator
{
public:
	struct Node
	{
		double value;
		double derivative;
	};

	typedef QMap<double, Node> Nodes;

	CAkimaInterpolator();
	CAkimaInterpolator(double* positions, double* values, int nodesCount);

	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::ISampledFunctionInterpolator)
	virtual bool InitFromFunction(const ISampledFunction& function) override;

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const override;
	virtual double GetValueAt(const double& argument) const override;

protected:
	Nodes m_nodes;
};


} // namespace imath


