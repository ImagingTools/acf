#ifndef isig_ISamplesProcessor_included
#define isig_ISamplesProcessor_included


#include "istd/CRange.h"

#include "iproc/TIAssyncProcessor.h"

#include "isig/ISamplesContainer.h"


namespace isig
{


/**
	Common interface for receiving, sending and processing sampled 1-D data.
*/
class ISamplesProcessor: virtual public iproc::TIAssyncProcessor<ISamplesContainer, ISamplesContainer>
{
public:
	/**
		Get range of possible/representable sample values.
	*/
	virtual istd::CRange GetValueRange(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const = 0;

	/**
		Get maximal number of samples can be received/send at one time.
	*/
	virtual int GetMaximalSamplesCount(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const = 0;
};


} // namespace isig


#endif // !isig_ISamplesProcessor_included


