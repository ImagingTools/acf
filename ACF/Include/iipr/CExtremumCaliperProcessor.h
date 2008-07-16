#ifndef iipr_CExtremumCaliperProcessor_included
#define iipr_CExtremumCaliperProcessor_included


#include "iproc/TSyncProcessorWrap.h"

#include "iipr/ICaliperProcessor.h"


namespace iipr
{


/**
	Calculate extremum of projection data.
	Using this tool together with derivative calculation it is possible to create caliper tool (extremums of first derivative).
*/
class CExtremumCaliperProcessor: public iproc::TSyncProcessorWrap<ICaliperProcessor>
{
public:
	typedef iproc::TSyncProcessorWrap<ICaliperProcessor> BaseClass;

	/**
		Do extremum features analyze.
	*/
	virtual bool DoExtremumCaliper(const CProjectionData& source, const ICaliperParams& params, IFeaturesConsumer& results);

	/**
		Get parameter ID used to extract caliper parameter object from parameter set.
	*/
	const std::string& GetCaliperParamId() const;

	/**
		Set parameter ID used to extract caliper parameter object from parameter set.
		It is only needed while using general processing interface iproc::TIProcessor.
	*/
	void SetCaliperParamId(const std::string& id);

	// reimplemented (iipr::ICaliperProcessor)
	virtual bool DoCaliper(
				const CProjectionData& projection,
				const ICaliperParams* paramsPtr,
				IFeaturesConsumer& results);

	// reimplemented (iproc::TIProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const CProjectionData* inputPtr,
				IFeaturesConsumer* outputPtr);

private:
	std::string m_caliperParamsId;
};


// inline methods

inline const std::string& CExtremumCaliperProcessor::GetCaliperParamId() const
{
	return m_caliperParamsId;
}


inline void CExtremumCaliperProcessor::SetCaliperParamId(const std::string& id)
{
	m_caliperParamsId = id;
}


} // namespace iipr


#endif // !iipr_CExtremumCaliperProcessor_included


