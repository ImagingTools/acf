#ifndef iipr_IFeature_included
#define iipr_IFeature_included


#include "iser/ISerializable.h"

#include "imath/TVarVector.h"

#include "iipr/iipr.h"


namespace iipr
{


class IFeature: virtual public iser::ISerializable
{
public:
	/**
		Get weigth of this feature.
	*/
	virtual double GetWeight() const = 0;

	/**
		Get position of this feature.
		Interpretation of this value is depending on feture type.
	*/
	virtual imath::CVarVector GetPosition() const = 0;

	/**
		Get feature description.
	*/
	virtual istd::CString GetDescription() const = 0;
};


} // namespace iipr


#endif // !iipr_IFeature_included


