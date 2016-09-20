#ifndef ilog_ILoggable_included
#define ilog_ILoggable_included


#include <ilog/IMessageConsumer.h>

#include <istd/IPolymorphic.h>


namespace ilog
{


/**
	Inteface for object, which support the 
*/
class ILoggable: virtual public istd::IPolymorphic
{
public:
	/**
		Install log.
	*/
	virtual void SetLogPtr(IMessageConsumer* logPtr) = 0;
	
	/**
		Get log.
	*/
	virtual IMessageConsumer* GetLogPtr() const = 0;

	/**
		Enable/disable tracing.
	*/
	virtual void SetTracingEnabled(bool trace) = 0;

	/**
		Returns true if tracing enabled.
	*/
	virtual bool IsTracingEnabled() const = 0;
};


} // namespace ilog


#endif // !ilog_ILoggable_included


