#pragma once

// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <idoc/CStandardDocumentMetaInfo.h>
#include <iser/CJsonMemWriteArchive.h>
#include <iser/CJsonMemReadArchive.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class Model: virtual public iser::ISerializable
{
public:
	bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag typeTag("Type", "Type of the meta information");
		bool retVal = archive.BeginTag(typeTag);
		retVal = retVal && archive.Process(value);
		retVal = retVal && archive.EndTag(typeTag);

		return retVal;
	}

	int value = 0;
};

class CJsonMemoryWriteArchiveTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void WriteTest();
	void ReadTest();
	void _Test();

	void cleanupTestCase();

private:
	idoc::CStandardDocumentMetaInfo m_metaInfo;
	QByteArray m_buffer = "";
	const QByteArray m_id = "12345678";
};


