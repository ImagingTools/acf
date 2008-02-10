#ifndef inemo_CNemoSensorData_included
#define inemo_CNemoSensorData_included


#include "inemo/INemoSensorData.h"


#include "Base/Sequence.h"


namespace inemo
{


/**
	\ingroup inemo

	Implementation of sensor info data record.
*/
class CNemoSensorData: public inemo::INemoSensorData
{
public:
	CNemoSensorData();

	virtual void SetMeasurementData(const acf::Sequence& sequence);
	virtual void SetPredictedDataA(const acf::Sequence& sequence);
	virtual void SetPredictedDataB(const acf::Sequence& sequence);

	// reimplemented (inemo::INemoSensorData)
	virtual const acf::SequenceInterface& GetMeasurementData() const;
	virtual const acf::SequenceInterface& GetPredictedDataA() const;
	virtual const acf::SequenceInterface& GetPredictedDataB() const;

private:
	acf::Sequence m_measuredData;
	acf::Sequence m_predictedDataA;
	acf::Sequence m_predictedDataB;
};


} // namespace inemo


#endif // !inemo_CNemoSensorData_included
