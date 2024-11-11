#include "CJsonMemoryWriteArchiveTest.h"


// ACF includes
#include <istd/TDelPtr.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>

#include <iprm/CParamsSet.h>
#include <iprm/CTextParam.h>
#include <iprm/CIdParam.h>
#include <iprm/CNameParam.h>


void CJsonMemoryWriteArchiveTest::initTestCase()
{
	m_metaInfo.SetMetaInfo(0, QByteArray("Test ByteArray"));
	m_metaInfo.SetMetaInfo(1, QString("Test String"));
	m_metaInfo.SetMetaInfo(2, int(10));
	m_metaInfo.SetMetaInfo(3, double(10.0));
}


void CJsonMemoryWriteArchiveTest::WriteTest()
{
	{
		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(m_metaInfo.Serialize(writeArchive));
	}
}

void CJsonMemoryWriteArchiveTest::ReadTest()
{
	idoc::CStandardDocumentMetaInfo metaInfo;
	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(metaInfo.Serialize(readArchive));
	QVERIFY(m_metaInfo.IsEqual(metaInfo));
}


void CJsonMemoryWriteArchiveTest::ParamsSetSerializeTest()
{
	// create reference params
	iprm::CParamsSet params;

	iprm::CNameParam nameParam;
	nameParam.SetName("Sam");
	params.SetEditableParameter("Name", &nameParam);

	iprm::CTextParam textParam;
	textParam.SetText("Qt Test");
	params.SetEditableParameter("Text", &textParam);

	iprm::CIdParam idParam;
	idParam.SetId("{GV5-E4YTGO5IY6T0EG45-45-TG54-G-F}");
	params.SetEditableParameter("ID", &idParam);

	QByteArray buffer;
	{
		iser::CJsonMemWriteArchive writeArchive;

		// serialize
		const bool written = params.Serialize(writeArchive);
		QVERIFY2(written, "Unable to save params");
	}

	QJsonParseError jsonParserror;
	QJsonObject jsonObject = QJsonDocument::fromJson(buffer, &jsonParserror).object();

	QVERIFY2(jsonParserror.error == QJsonParseError::NoError,
			 QString("Saved JSON is NOT valid. Error: '%1' at '%2'. \n DATA: \n %3").arg(jsonParserror.errorString(), QString::number(jsonParserror.offset), qPrintable(buffer)).toLocal8Bit());

	// create new params
	iprm::CParamsSet newParams;
	iprm::CNameParam newNameParam;
	newParams.SetEditableParameter("Name", &newNameParam);
	iprm::CTextParam newTextParam;
	newParams.SetEditableParameter("Text", &newTextParam);
	iprm::CIdParam newIdParam;
	newParams.SetEditableParameter("ID", &newIdParam);


	iser::CJsonMemReadArchive readArchive(buffer);
	// restore serialized data in iew params
	const bool restored = newParams.Serialize(readArchive);
	QVERIFY2(restored, "Unable to load params");

	QByteArray newBuffer;
	{
		iser::CJsonMemWriteArchive writeArchive;

		// serialize
		const bool written = newParams.Serialize(writeArchive);
		QVERIFY2(written, "Unable to save new params");
	}

	// compare saved and restored
	// bool isEqual = newParams.IsEqual(params);
	bool isEqual = (buffer == newBuffer);
	QVERIFY2(isEqual, "Restored params is not equal to original");
}


void CJsonMemoryWriteArchiveTest::_Test()
{
	m_buffer.clear();

	Model model;

	model.value = 10;

	{
		iser::CJsonMemWriteArchive writeArchive;
		QVERIFY(model.Serialize(writeArchive));
	}

	model.value = 0;

	iser::CJsonMemReadArchive readArchive(m_buffer);
	QVERIFY(model.Serialize(readArchive));
}


void CJsonMemoryWriteArchiveTest::cleanupTestCase()
{
}


I_ADD_TEST(CJsonMemoryWriteArchiveTest);
