#ifndef ibase_CMessage_included
#define ibase_CMessage_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IInformation.h"

#include "iser/ISerializable.h"


namespace ibase
{


/**
	Basic implementation of the istd::IInformation interface
*/
class CMessage:
			virtual public istd::IInformation,
			virtual public iser::ISerializable
{
public:
	CMessage();
	CMessage(	istd::IInformation::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0);

	virtual void SetCategory(istd::IInformation::InformationCategory category);
	virtual void SetText(const QString& text);
	virtual void SetSource(const QString& source);

	// reimplemented (istd::IInformation)
	virtual QDateTime GetTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetSource() const;
	virtual int GetFlags() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	istd::IInformation::InformationCategory m_category;
	int m_id;
	QString m_text;
	QString m_source;
	int m_flags;

	QDateTime m_timeStamp;
};


// inline methods

// reimplemented (istd::IInformation)

inline QDateTime CMessage::GetTimeStamp() const
{
	return m_timeStamp;
}


inline istd::IInformation::InformationCategory CMessage::GetInformationCategory() const
{
	return m_category;
}


inline int CMessage::GetInformationId() const
{
	return m_id;
}


inline QString CMessage::GetInformationDescription() const
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


