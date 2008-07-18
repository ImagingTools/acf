#ifndef iipr_CSearchResultSet_included
#define iipr_CSearchResultSet_included


#include "iipr/ISearchResultSet.h"
#include "iipr/TFeaturesContainerWrap.h"
#include "iipr/CSearchFeature.h"


namespace iipr
{


class CSearchResultSet: public iipr::TFeaturesContainerWrap<iipr::ISearchResultSet, iipr::CSearchFeature>
{
public:
	void SetTime(double time);

	// reimplemented (iipr::ISearchResultSet)
	virtual double GetTime() const;

private:
	double m_time;
};


} // namespace iipr


#endif //!iipr_CSearchResultSet_included

