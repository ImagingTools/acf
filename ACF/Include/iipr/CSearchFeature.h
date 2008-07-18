#ifndef iipr_CSearchFeature_included
#define iipr_CSearchFeature_included


#include "i2d/CVector2d.h"

#include "iipr/IFeature.h"


namespace iipr
{


/**
	Implementation of IFeature interface for pattern search features.
*/
class CSearchFeature: virtual public IFeature
{
public:
	CSearchFeature(const i2d::CVector2d& position, const i2d::CVector2d& scale, double angle, double score);

	virtual double GetAngle() const;
	virtual i2d::CVector2d GetScale() const;

	// reimplemented (iipr::IFeature)
	virtual double GetWeight() const;
	virtual imath::CVarVector GetPosition() const;
	virtual istd::CString GetDescription() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	i2d::CVector2d m_scale;
	i2d::CVector2d m_position;
	double m_angle;
	double m_score;
};


} // namespace iipr


#endif // !iipr_CSearchFeature_included


