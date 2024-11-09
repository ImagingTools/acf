#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QList>
#include <QtCore/QIODevice>

// ACF includes
#include <iser/CTextReadArchiveBase.h>


namespace iser
{


/**
	Implementation of an ACF archive deserializing from a JSON string

	\note	As to simplify decoding of some more complicated data structures support
			for special annotation tags was added. They are used for guiding the 
			deserialization algorithm in some special cases.
*/
class CJsonReadArchiveBase : public iser::CTextReadArchiveBase
{
public:
	typedef CTextReadArchiveBase BaseClass;

	CJsonReadArchiveBase(
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;

protected:
	bool SetContent(QIODevice* devicePtr);

	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text) override;

protected:
	QJsonDocument m_document;

	class HelperIterator
	{
	public:
		HelperIterator();
		void SetKey(const QString& key);
		const QString GetKey();
		QString GetValue();
		QJsonObject GetObject();
		bool NextElementArray();
		void SetValue(const QJsonValue value);
		bool isObject();
		bool isArray();
	private:
		QJsonArray m_array;
		QJsonArray::ConstIterator m_arrayIterator;
		QJsonValue m_value;
		QString m_key;
		int activeArrayIndex;
	};

	QList<HelperIterator> m_iterators;
	QList<const iser::CArchiveTag*> m_tags;
	iser::CArchiveTag m_rootTag;
	bool m_rootTagEnabled;
	bool m_serializeHeader;
};


} // namespace iser


