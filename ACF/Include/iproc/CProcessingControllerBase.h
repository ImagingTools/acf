#ifndef CProcessingControllerBase_included
#define CProcessingControllerBase_included


#include "iproc/IProcessingController.h"
#include "iproc/IOperator.h"

#include "imod/TSingleModelObserverBase.h"


namespace iproc
{


class CProcessingControllerBase:	virtual public iproc::IProcessingController,
									public imod::TSingleModelObserverBase<iproc::IOperator>
{
public:
	typedef imod::TSingleModelObserverBase<iproc::IOperator> BaseClass2;

	// reimplemented (iproc::IProcessingController)
	virtual const ibase::IMessageContainer* GetLog() const; 
	virtual void AddProgressHandler(iproc::IProgressEventHandler* progressHandlerPtr);

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef std::set<iproc::IProgressEventHandler*> ProgressHandlers;

	ProgressHandlers m_progressHandlers;

};


} // namespace iproc


#endif // !CProcessingControllerBase_included
