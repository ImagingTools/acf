#ifndef iipr_CCaliperFeature_included
#define iipr_CCaliperFeature_included


#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for caliper-extracted features.
*/
class CCaliperFeature: virtual public IFeature
{
public:
	CCaliperFeature(double weight, double position);

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;
	virtual imath::CVarVector GetPosition() const;
	virtual istd::CString GetDescription() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_weight;
	imath::CVarVector m_position;
};


} // namespace iipr


#endif // !iipr_CCaliperFeature_included


