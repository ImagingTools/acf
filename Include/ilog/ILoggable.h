#ifndef ilog_ILoggable_included
#define ilog_ILoggable_included


#include <ilog/IMessageConsumer.h>


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
};


} // namespace ilog


#endif // !ilog_ILoggable_included


