#ifndef ilog_CMessage_included
#define ilog_CMessage_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IInformationProvider.h"
#include "iser/IObject.h"


namespace ilog
{


/**
	Basic implementation of the istd::IInformationProvider interface

	\ingroup Logging
*/
class CMessage:
			virtual public istd::IInformationProvider,
			virtual public iser::IObject
{
public:
	CMessage();
	CMessage(	istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

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

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	static QByteArray GetMessageTypeId();

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


} // namespace ilog


#endif // !ilog_CMessage_included


