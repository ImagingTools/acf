#ifndef ibase_CMessage_included
#define ibase_CMessage_included


#include "ibase/IMessage.h"

#include "isys/CSimpleDateTime.h"


namespace ibase
{		


class CMessage: virtual public ibase::IMessage
{
public:
	CMessage();
	CMessage(MessageCategory category, int id, const istd::CString& text, const istd::CString& source);

	virtual void SetCategory(MessageCategory category);
	virtual void SetText(const istd::CString& text);
	virtual void SetSource(const istd::CString& source);

	// reimplemented (ibase::IMessage)
	virtual const isys::IDateTime& GetTimeStamp() const;
	virtual MessageCategory GetCategory() const;
	virtual int GetId() const;
	virtual istd::CString GetText() const;
	virtual istd::CString GetSource() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	IMessage::MessageCategory m_category;
	int m_id;
	istd::CString m_text;
	istd::CString m_source;

	isys::CSimpleDateTime m_time;
};


} // namespace ibase


#endif // !ibase_CMessage_included