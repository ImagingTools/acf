#ifndef icmm_ICieColorValuesProviderr_included
#define icmm_ICieColorValuesProviderr_included


// ACF includes
#include "istd/IChangeable.h"
#include "icmm/CLab.h"
#include "icmm/CVarColor.h"


namespace icmm
{


class ICieColorValuesProvider: virtual public istd::IChangeable
{
public:
	typedef QVector<icmm::CLab> LabValues;
	typedef QVector<icmm::CVarColor> XyzValues;

	virtual const LabValues& GetLabValues() const = 0;
	virtual const XyzValues& GetXyzValues() const = 0;
};


} // namespace icmm


#endif // !icmm_ICieColorValuesProviderr_included


