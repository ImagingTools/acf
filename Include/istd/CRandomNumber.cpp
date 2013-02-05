#include "istd/CRandomNumber.h"


// STD includes
#include <stdlib.h>
#include <time.h>

// ACF includes
#include "istd/istd.h"


namespace istd
{


CRandomNumber::CRandomNumber()
{
	::srand((unsigned)::time(NULL));
}


int CRandomNumber::GetNextInt(int minValue, int maxValue) const
{
	double unitary = GetNextUnitary();

	return minValue + int(unitary * (maxValue - minValue));
}


double CRandomNumber::GetNextDouble(double minValue, double maxValue) const
{
	double unitary = GetNextUnitary();

	return minValue + unitary * (maxValue - minValue);
}


// protected methods

double CRandomNumber::GetNextUnitary() const
{
	double maxRand(RAND_MAX);

	double retVal = 0;
	double factor = 1;

	for (int i = 0; i < 10; ++i){
		factor *= 0.5;
		retVal += factor * 0.5 * ::rand() / maxRand;
	}

	Q_ASSERT(retVal >= 0.0);
	Q_ASSERT(retVal <= 1.0);

	return retVal;
}


} // namespace istd


