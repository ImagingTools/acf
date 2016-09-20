#ifndef imath_CLinearInterpolator_included
#define imath_CLinearInterpolator_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imath/TIMathFunction.h>


namespace imath
{


/**
	Implementation of a linear interpolator.
*/
class CLinearInterpolator: public virtual IDoubleFunction
{
public:
	CLinearInterpolator();
	CLinearInterpolator(double* positions, double* values, int nodesCount);

	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const;
	virtual double GetValueAt(const double& argument) const;

private:
	typedef QMap<double, double> Nodes;
	Nodes m_nodes;
};


} // namespace imath


#endif // !imath_CLinearInterpolator_included


