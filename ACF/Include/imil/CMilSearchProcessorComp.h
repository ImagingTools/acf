#ifndef imil_CMilSearchProcessorComp_included
#define imil_CMilSearchProcessorComp_included


#include "imil/imil.h"

#include "ibase/TMessageProducerWrap.h"

#include "icomp/CComponentBase.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iimg/IBitmap.h"

#include "iipr/ISearchProcessor.h"

#include "imil/CMilSearchParams.h"
#include "imil/CMilSearchModel.h"
#include "imil/CMilEngine.h"


namespace imil
{


class CMilSearchProcessorComp:	public ibase::TMessageProducerWrap<icomp::CComponentBase>,
								public iproc::TSyncProcessorWrap<iipr::ISearchProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMilSearchProcessorComp)
		I_REGISTER_INTERFACE(iipr::ISearchProcessor);
		I_ASSIGN(m_searchParamsIdAttrPtr, "ParamsId", "Search params id", false, "ParamsId");
	I_END_COMPONENT

	// reimplemented (iproc::TSyncProcessorWrap<iipr::ISearchProcessor>)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iipr::IFeaturesConsumer* outputPtr);

private:
	imil::CMilEngine m_engine;

	I_ATTR(istd::CString, m_searchParamsIdAttrPtr);
};


} // namespace iipr


#endif // !imil_CMilSearchProcessorComp_included


