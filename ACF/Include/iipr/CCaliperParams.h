#ifndef iipr_CCaliperParams_included
#define iipr_CCaliperParams_included


#include "iipr/ICaliperParams.h"


namespace iipr
{


class CCaliperParams: virtual public ICaliperParams
{
public:
	CCaliperParams();

	// reimplemented (iipr::ICaliperParams)
	virtual double GetWeightThreshold() const;
	virtual void SetWeightThreshold(double threshold);
	virtual int GetPolarityMode() const;
	virtual void SetPolarityMode(int mode);
	virtual int GetDirectionMode() const;
	virtual void SetDirectionMode(int mode);

private:
	double m_weightThreshold;
	int m_polarityMode;
	int m_directionMode;
};


} // namespace iipr


#endif // !iipr_CCaliperParams_included


