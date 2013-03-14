#ifndef imath_CAkimaInterpolator_included
#define imath_CAkimaInterpolator_included


// Qt includes
#include <QtCOre/QMap>

// ACF includes
#include "imath/TIMathFunction.h"


namespace imath
{


/**
	1D interpolation using Akima method.
*/
class CAkimaInterpolator: public virtual IDoubleFunction
{
public:
	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const;
	virtual double GetValueAt(const double& argument) const;

private:
	struct Node
	{
		double value;
		double derivative;
	};
	typedef QMap<double, Node> Nodes;

	Nodes m_nodes;
};


} // namespace imath


#endif // !imath_CAkimaInterpolator_included


