#ifndef inemo_CNemoSensorSpecification_included
#define inemo_CNemoSensorSpecification_included


#include "inemo/INemoSensorSpecification.h"


namespace inemo
{


/**
	\ingroup inemo

	Implementation of sensor specification data record.
*/
class CNemoSensorSpecification: public inemo::INemoSensorSpecification
{
public:
	CNemoSensorSpecification();

	// reimplemented (inemo::INemoSensorSpecification)
	virtual istd::CRange GetRange() const;
	virtual std::string GetUnit() const;
	virtual int GetType() const;

private:
	istd::CRange m_range;
	std::string m_unit;
	int m_type;
};


} // namespace inemo


#endif // !inemo_CNemoSensorSpecification_included
