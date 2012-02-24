#ifndef ibase_IMessage_included
#define ibase_IMessage_included


#include "istd/ILogger.h"
#include <QString>

#include "iser/ISerializable.h"

#include "ibase/ibase.h"


namespace isys
{
	class IDateTime;
}


namespace ibase
{		


/**
	Common interface for an message.
	\sa istd::ILogger
*/
class IMessage: virtual public iser::ISerializable
{
public:
	/**
		Get time stamp of the message.
	*/
	virtual const isys::IDateTime& GetTimeStamp() const = 0;

	/**
		Get category of the message.
		\sa MessageCategory
	*/
	virtual istd::ILogger::MessageCategory GetCategory() const = 0;

	/**
		Get binary ID of the message using to automatical identification of this message type.
	*/
	virtual int GetId() const = 0;

	/**
		Get the text of the message.
	*/
	virtual QString GetText() const = 0;

	/**
		Get the source of the message.
	*/
	virtual QString GetSource() const = 0;

	/**
		Get flags of the message.
		\sa istd::ILogger::MessageFlags
	*/
	virtual int GetFlags() const = 0;
};


} // namespace ibase


#endif // !ibase_IMessage_included

