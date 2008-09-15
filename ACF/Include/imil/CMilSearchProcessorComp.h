#ifndef imil_CMilSearchProcessorComp_included
#define imil_CMilSearchProcessorComp_included


#include "ibase/TMessageProducerWrap.h"

#include "iipr/IFeaturesConsumer.h"

#include "iproc/TSyncProcessorCompBase.h"

#include "iimg/IBitmap.h"

#include "imil/CMilSearchParams.h"
#include "imil/CMilSearchModel.h"
#include "imil/CMilEngine.h"


namespace imil
{


class CMilSearchProcessorComp: public ibase::TMessageProducerWrap<iproc::CSyncProcessorCompBase>
{
public:
	typedef ibase::TMessageProducerWrap<iproc::CSyncProcessorCompBase> BaseClass;

	I_BEGIN_COMPONENT(CMilSearchProcessorComp)
	I_END_COMPONENT

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	virtual bool DoSearch(
				const CMilSearchParams& params,
				const iimg::IBitmap& bitmap,
				iipr::IFeaturesConsumer& result);

private:
	imil::CMilEngine m_engine;
};


} // namespace iipr


#endif // !imil_CMilSearchProcessorComp_included


