#ifndef iipr_CSearchParamsBase_included
#define iipr_CSearchParamsBase_included


#include "iipr/ISearchParams.h"


namespace iipr
{


class CSearchParamsBase: virtual public iipr::ISearchParams
{
public:
	CSearchParamsBase();
	
	// reimplemented (iipr::ISearchParams)
	virtual void SetCalibrationPtr(const i2d::CTransform* calibrationPtr);
	virtual i2d::CRectangle GetSearchRegion(const i2d::CTransform* calibrationPtr = NULL) const;
	virtual i2d::CRectangle GetModelRegion(const i2d::CTransform* calibrationPtr = NULL) const;
	virtual double GetMinScore() const;
	virtual void SetMinScore(double minScore);
	virtual double GetStartAngle() const;
	virtual void SetStartAngle(double minAngle);
	virtual double GetEndAngle() const;
	virtual void SetEndAngle(double maxAngle);
	virtual double GetMinScale() const;
	virtual void SetMinScale(double minScale);
	virtual double GetMaxScale() const;
	virtual void SetMaxScale(double maxScale);
	virtual int GetMatchesCount() const;
	virtual void SetMatchesCount(int matchesCount);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive);

protected:
	static i2d::CRectangle GetCalibrated(const i2d::CTransform& calibration, const i2d::CRectangle& region);

protected:
	i2d::CRectangle m_searchRegion;
	i2d::CRectangle m_modelRegion;
	const i2d::CTransform* m_calibrationPtr;
	double m_minScore;
	double m_startAngle;
	double m_endAngle;
	double m_minScale;
	double m_maxScale;
	int m_matchesCount;
	double m_timeout;
};


} // namespace iipr


#endif //!iipr_CSearchParamsBase_included

