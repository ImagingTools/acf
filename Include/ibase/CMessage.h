#ifndef ibase_CMessage_included
#define ibase_CMessage_included


#include "istd/ILogger.h"
#include "istd/TSmartPtr.h"

#include <QString>

#include "isys/CSimpleDateTime.h"

#include "ibase/IMessage.h"


namespace ibase
{


/**
	Basic implementation of the ibase::IMessage interface
*/
class CMessage: virtual public ibase::IMessage
{
public:
	CMessage();
	CMessage(	istd::ILogger::MessageCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0);

	virtual void SetCategory(istd::ILogger::MessageCategory category);
	virtual void SetText(const QString& text);
	virtual void SetSource(const QString& source);

	// reimplemented (ibase::IMessage)
	virtual const isys::IDateTime& GetTimeStamp() const;
	virtual istd::ILogger::MessageCategory GetCategory() const;
	virtual int GetId() const;
	virtual QString GetText() const;
	virtual QString GetSource() const;
	virtual int GetFlags() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	void InitializeMessageTime();

protected:
	typedef istd::TSmartPtr<isys::IDateTime> TimePtr;

	istd::ILogger::MessageCategory m_category;
	int m_id;
	QString m_text;
	QString m_source;
	int m_flags;

	TimePtr m_timePtr;
};


// inline methods

// reimplemented (ibase::IMessage)

inline const isys::IDateTime& CMessage::GetTimeStamp() const
{
	return *m_timePtr;
}


inline istd::ILogger::MessageCategory CMessage::GetCategory() const
{
	return m_category;
}


inline int CMessage::GetId() const
{
	return m_id;
}


inline QString CMessage::GetText() const
{
	return m_text;
}


inline QString CMessage::GetSource() const
{
	return m_source;
}


inline int CMessage::GetFlags() const
{
	return m_flags;
}


} // namespace ibase


#endif // !ibase_CMessage_included


