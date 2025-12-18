#include <ifile/Test/CCompactXmlDeviceArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CCompactXmlDeviceReadArchive.h>
#include <ifile/CCompactXmlDeviceWriteArchive.h>
#include <ifile/TDeviceBasedSerializerComp.h>


void CCompactXmlDeviceArchiveTest::DoFileTest()
{
	QFile file("./Output.xml");

	QString path = "./Test/Blablablabla.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CCompactXmlDeviceWriteArchive writeArchive(file);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CCompactXmlDeviceReadArchive readArchive(file);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	file.remove();
}


void CCompactXmlDeviceArchiveTest::DoMemoryTest()
{
	QBuffer inputData;
	QString path = "./Test/Blablablabla.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CCompactXmlDeviceWriteArchive writeArchive(inputData);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CCompactXmlDeviceReadArchive readArchive(inputData);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);
}


void CCompactXmlDeviceArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TDeviceBasedSerializerComp<
							ifile::CCompactXmlDeviceReadArchive,
							ifile::CCompactXmlDeviceWriteArchive>> CompactXmlDeviceSerializer;

	CompactXmlDeviceSerializer component;
	component.InitComponent();

	QFile file("./PersistenceOutput.xml");
	QString path = "./Test/Blablablabla.test";
	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	{
		int state = component.WriteToDevice(filePathParam, file);
		QVERIFY(state == ifile::IDeviceBasedPersistence::Successful);
	}

	imod::TModelWrap<ifile::CFileNameParam> filePathParam2;
	int state = component.ReadFromDevice(filePathParam2, file);
	QVERIFY(state == ifile::IDeviceBasedPersistence::Successful);
	QVERIFY(filePathParam2.IsEqual(filePathParam));
	QVERIFY(filePathParam2.GetPath() == path);

	file.remove();
}


I_ADD_TEST(CCompactXmlDeviceArchiveTest);


