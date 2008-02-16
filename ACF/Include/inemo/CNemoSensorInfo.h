#ifndef inemo_CNemoSensorInfo_included
#define inemo_CNemoSensorInfo_included


#include "inemo/INemoSensorInfo.h"


namespace inemo
{


/**
	\ingroup inemo

	Implementation of sensor info data record.
*/
class CNemoSensorInfo: public inemo::INemoSensorInfo
{
public:
	CNemoSensorInfo();

	virtual void SetFirstLevelLocation();
	virtual void SetSecondLevelLocation();
	virtual void SetThirdLevelLocation();
	virtual void SetFourthLevelLocation();

	// reimplemented (inemo::INemoSensorInfo)
	virtual ::std::string GetFirstLevelLocation() const;
	virtual ::std::string GetSecondLevelLocation() const;
	virtual ::std::string GetThirdLevelLocation() const;
	virtual ::std::string GetFourthLevelLocation() const;
};


} // namespace inemo


#endif // !inemo_CNemoSensorInfo_included
