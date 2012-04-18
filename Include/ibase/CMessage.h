#ifndef ibase_CMessage_included
#define ibase_CMessage_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IInformationProvider.h"

#include "iser/ISerializable.h"


namespace ibase
{


/**
	Basic implementation of the istd::IInformationProvider interface
*/
class CMessage:
			virtual public istd::IInformationProvider,
			virtual public iser::ISerializable
{
public:
	CMessage();
	CMessage(	istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0);

	virtual void SetCategory(istd::IInformationProvider::InformationCategory category);
	virtual void SetText(const QString& text);
	virtual void SetSource(const QString& source);

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	istd::IInformationProvider::InformationCategory m_category;
	int m_id;
	QString m_text;
	QString m_source;
	int m_flags;

	QDateTime m_timeStamp;
};


// inline methods

// reimplemented (istd::IInformationProvider)

inline QDateTime CMessage::GetInformationTimeStamp() const
{
	return m_timeStamp;
}


inline istd::IInformationProvider::InformationCategory CMessage::GetInformationCategory() const
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


inline QString CMessage::GetInformationSource() const
{
	return m_source;
}


inline int CMessage::GetInformationFlags() const
{
	return m_flags;
}


} // namespace ibase


#endif // !ibase_CMessage_included


