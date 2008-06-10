#ifndef iproc_IProcessingController_included
#define iproc_IProcessingController_included


#include "ibase/IMessageContainer.h"

#include "iproc/IProgressEventHandler.h"


namespace iproc
{


class IProcessingController: virtual public istd::IPolymorphic
{
public:
	virtual bool StartProcessing() = 0;
	virtual void PauseProcessing() = 0;
	virtual void ResumeProcessing() = 0;
	virtual void AbortProcessing() = 0;
	virtual bool IsRunning() const = 0;
	virtual bool IsPaused() const = 0;
	virtual bool IsFinished() const = 0;
	virtual double GetProcessingTime() const = 0;
	virtual void SetLogPtr(ibase::IMessageContainer* logPtr) = 0; 
	virtual void AddProgressHandler(iproc::IProgressEventHandler* progressHandlerPtr) = 0;
};


} // namespace iproc


#endif // iproc_IProcessingController_included


