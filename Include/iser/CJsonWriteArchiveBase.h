#pragma once


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVector>
#include <QtCore/QDataStream>
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CTextWriteArchiveBase.h>
#include <istd/TDelPtr.h>


namespace iser
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonWriteArchiveBase: public iser::CTextWriteArchiveBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	~CJsonWriteArchiveBase();

	void SetFormat(QJsonDocument::JsonFormat jsonFormat);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;
	virtual bool Process(QByteArray& value) override;
	virtual bool ProcessData(void* dataPtr, int size) override;

	using BaseClass::Process;

protected:
	CJsonWriteArchiveBase(
				const iser::IVersionInfo* versionInfoPtr,
				bool serializeHeader,
				const iser::CArchiveTag& rootTag);

	bool InitStream(bool serializeHeader);
	bool InitArchive(QIODevice* devicePtr);
	bool InitArchive(QByteArray& inputString);
	bool WriteTag(const iser::CArchiveTag& tag, QString separator);
	bool WriteJsonHeader();
	bool Flush();

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;

protected:
	QTextStream m_stream;
	QBuffer m_buffer;
	bool m_firstTag;
	QJsonDocument::JsonFormat m_jsonFormat;
	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;	// idicate that separator must be added before something is outputted
	bool m_allowAttribute;		// indicate if attribute outputting is allowed now

	struct TagsStackItem
	{
		const iser::CArchiveTag* m_tagPtr;
		bool m_isMultiTag;
	};

	bool m_quotationMarksRequired = false;

	QList<TagsStackItem> m_tagsStack;
};


} // namespace iser


