#include <icmm/CXyzToCieLabTransformation.h>


// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include <icmm/CLab.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorTransformation)

bool CXyzToCieLabTransformation::GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const
{
	if (argument.GetElementsCount() != 3){
		return false;
	}

	if (result.GetElementsCount() != icmm::CLab::GetElementsCount()){
		return false;
	}

	const double refX = 95.047;
	const double refY = 100.000;
	const double refZ = 108.883;

	double X = argument[0];
	double Y = argument[1];
	double Z = argument[2];

	double normX = X / refX;
	double normY = Y / refY;
	double normZ = Z / refZ;

	if ( normX > 0.008856 ){
		normX = pow(normX, 1/3.0);
	}
	else{
		normX = (7.787 * normX) + (16.0 / 116.0);
	}
	
	if (normY > 0.008856){
		normY = pow(normY, 1/3.0);
	}
	else{
		normY = (7.787 * normY) + (16.0 / 116.0);
	}
	
	if (normZ > 0.008856){
		normZ = pow(normZ, 1/3.0);
	}
	else{
		normZ = (7.787 * normZ) + (16.0 / 116.0);
	}

	double L = (116 * normY) - 16;
	double a = 500 * (normX - normY);
	double b = 200 * (normY - normZ);

	result[icmm::CLab::CI_L] = L;
	result[icmm::CLab::CI_A] = a;
	result[icmm::CLab::CI_B] = b;

	return true;
}


icmm::CVarColor CXyzToCieLabTransformation::GetValueAt(const icmm::CVarColor& argument) const
{
	icmm::CVarColor result(3);

	GetValueAt(argument, result);

	return result;
}


} // namespace iccm


