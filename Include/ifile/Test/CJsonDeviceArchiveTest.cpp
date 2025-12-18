#include <ifile/Test/CJsonDeviceArchiveTest.h>


// Qt includes
#include <QtCore/QFile>

// ACF includes
#include <icomp/TSimComponentWrap.h>
#include <ifile/CFileNameParam.h>
#include <imod/TModelWrap.h>
#include <ifile/CJsonDeviceReadArchive.h>
#include <ifile/CJsonDeviceWriteArchive.h>
#include <ifile/TDeviceBasedSerializerComp.h>


void CJsonDeviceArchiveTest::DoFileTest()
{
	QFile file("./Output.json");

	QString path = "./Test/Blablablabla.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CJsonDeviceWriteArchive writeArchive(file);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CJsonDeviceReadArchive readArchive(file);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);

	file.remove();
}


void CJsonDeviceArchiveTest::DoMemoryTest()
{
	QBuffer inputData;
	QString path = "./Test/Blablablabla.test";

	imod::TModelWrap<ifile::CFileNameParam> filePathParam;
	filePathParam.SetPath(path);

	// Write data:
	{
		ifile::CJsonDeviceWriteArchive writeArchive(inputData);
		bool retVal = filePathParam.Serialize(writeArchive);
		QVERIFY(retVal);
	}

	// Read data:
	ifile::CFileNameParam filePathParam2;
	ifile::CJsonDeviceReadArchive readArchive(inputData);
	bool retVal = filePathParam2.Serialize(readArchive);
	QVERIFY(retVal);

	// Compare data
	QVERIFY(filePathParam.IsEqual(filePathParam2));
	QVERIFY(filePathParam2.GetPath() == path);
}


void CJsonDeviceArchiveTest::DoPersistenceComponentTest()
{
	typedef icomp::TSimComponentWrap<
				ifile::TDeviceBasedSerializerComp<
							ifile::CJsonDeviceReadArchive,
							ifile::CJsonDeviceWriteArchive>> JsonDeviceSerializer;

	JsonDeviceSerializer component;
	component.InitComponent();

	QFile file("./PersistenceOutput.json");
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


I_ADD_TEST(CJsonDeviceArchiveTest);


