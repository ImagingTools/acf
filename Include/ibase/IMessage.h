#ifndef ibase_IMessage_included
#define ibase_IMessage_included


#include "ibase/ibase.h"


#include "isys/IDateTime.h"

#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace ibase
{		


/**
	Common interface for an message
*/
class IMessage: virtual public iser::ISerializable
{
public:
	enum MessageCategory{
		Info,
		Warning,
		Error,
		Critical,
		DebugMask = 128,
		SystemMask = 256,
		UserMask = 512
	};

	/**
		Get time stamp of the message.
	*/
	virtual const isys::IDateTime& GetTimeStamp() const = 0;

	/**
		Get category of the message.
		\sa MessageCategory
	*/
	virtual MessageCategory GetCategory() const = 0;

	/**
		Get binary ID of the message using to automatical identification of this message type.
	*/
	virtual int GetId() const = 0;

	/**
		Get the text of the message.
	*/
	virtual istd::CString GetText() const = 0;

	/**
		Get the source of the message.
	*/
	virtual istd::CString GetSource() const = 0;
};


} // namespace ibase


#endif // !ibase_IMessage_included

