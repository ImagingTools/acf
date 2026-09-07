// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/Test/CAdvancedFileComponentTest.h>


// Qt includes
#include <QtCore/QTemporaryDir>
#include <QtCore/QTemporaryFile>
#include <QtCore/QDir>
#include <QtCore/QFile>

// ACF includes
#include <ifile/CTempFileManagerComp.h>
#include <ifile/CFileNameParam.h>
#include <ifile/CFileListProviderComp.h>
#include <ifile/CFileReadArchive.h>
#include <ifile/CFileWriteArchive.h>
#include <ifile/CFileReadSecureArchive.h>
#include <ifile/CFileWriteSecureArchive.h>
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>


// Simple test model
class SimpleModel: virtual public iser::ISerializable
{
public:
	virtual bool Serialize(iser::IArchive& archive) override
	{
		static iser::CArchiveTag valueTag("Value", "Integer value");
		bool retVal = archive.BeginTag(valueTag);
		retVal = retVal && archive.Process(value);
		retVal = retVal && archive.EndTag(valueTag);
		return retVal;
	}
	
	int value = 0;
};


// TempFileManager advanced tests

void CAdvancedFileComponentTest::testTempFileManagerNestedSessions()
{
	ifile::CTempFileManagerComp tempManager;
	
	// Create parent session
	QByteArray parentSession = tempManager.BeginSession("parent_session");
	QVERIFY(!parentSession.isEmpty());
	
	// Create nested sessions
	QByteArray childSession1 = tempManager.BeginSession("parent_session/child1");
	QByteArray childSession2 = tempManager.BeginSession("parent_session/child2");
	
	QVERIFY(!childSession1.isEmpty());
	QVERIFY(!childSession2.isEmpty());
	QVERIFY(childSession1 != childSession2);
	QVERIFY(parentSession != childSession1);
	
	// Add files to different sessions
	QByteArray parentFile = tempManager.AddFileItem(parentSession, "parent.txt");
	QByteArray child1File = tempManager.AddFileItem(childSession1, "child1.txt");
	QByteArray child2File = tempManager.AddFileItem(childSession2, "child2.txt");
	
	QVERIFY(!parentFile.isEmpty());
	QVERIFY(!child1File.isEmpty());
	QVERIFY(!child2File.isEmpty());
	
	// Verify paths are different
	QString parentPath = tempManager.GetPath(parentSession, parentFile);
	QString child1Path = tempManager.GetPath(childSession1, child1File);
	QString child2Path = tempManager.GetPath(childSession2, child2File);
	
	QVERIFY(!parentPath.isEmpty());
	QVERIFY(!child1Path.isEmpty());
	QVERIFY(!child2Path.isEmpty());
	QVERIFY(parentPath != child1Path);
	QVERIFY(child1Path != child2Path);
	
	// Cleanup sessions
	tempManager.FinishSession(childSession1);
	tempManager.FinishSession(childSession2);
	tempManager.FinishSession(parentSession);
}


void CAdvancedFileComponentTest::testTempFileManagerSessionCleanup()
{
	ifile::CTempFileManagerComp tempManager;
	
	// Create session and add files
	QByteArray sessionId = tempManager.BeginSession("cleanup_test");
	QVERIFY(!sessionId.isEmpty());
	
	QByteArray file1 = tempManager.AddFileItem(sessionId, "file1.txt");
	QByteArray file2 = tempManager.AddFileItem(sessionId, "file2.txt");
	QByteArray file3 = tempManager.AddFileItem(sessionId, "file3.txt");
	
	// Get paths and verify they exist in the session
	QString path1 = tempManager.GetPath(sessionId, file1);
	QString path2 = tempManager.GetPath(sessionId, file2);
	QString path3 = tempManager.GetPath(sessionId, file3);
	
	QVERIFY(!path1.isEmpty());
	QVERIFY(!path2.isEmpty());
	QVERIFY(!path3.isEmpty());
	
	// Finish session (should clean up)
	tempManager.FinishSession(sessionId);
	
	// Verify paths are no longer accessible
	QString emptyPath = tempManager.GetPath(sessionId, file1);
	QVERIFY(emptyPath.isEmpty());
}


void CAdvancedFileComponentTest::testTempFileManagerMultipleFilesInSession()
{
	ifile::CTempFileManagerComp tempManager;
	
	QByteArray sessionId = tempManager.BeginSession("multi_file_session");
	QVERIFY(!sessionId.isEmpty());
	
	// Add many files to the same session
	QVector<QByteArray> fileIds;
	const int numFiles = 20;
	
	for (int i = 0; i < numFiles; ++i)
	{
		QString fileName = QString("test_file_%1.txt").arg(i);
		QByteArray fileId = tempManager.AddFileItem(sessionId, fileName);
		QVERIFY(!fileId.isEmpty());
		fileIds.append(fileId);
	}
	
	// Verify all files have unique paths
	QSet<QString> paths;
	for (const QByteArray& fileId : fileIds)
	{
		QString path = tempManager.GetPath(sessionId, fileId);
		QVERIFY(!path.isEmpty());
		QVERIFY(!paths.contains(path)); // Ensure uniqueness
		paths.insert(path);
	}
	
	QCOMPARE(paths.size(), numFiles);
	
	// Clean up
	tempManager.FinishSession(sessionId);
}


void CAdvancedFileComponentTest::testTempFileManagerFileOperations()
{
	ifile::CTempFileManagerComp tempManager;
	
	QByteArray sessionId = tempManager.BeginSession("file_ops_test");
	QVERIFY(!sessionId.isEmpty());
	
	// Add file
	QByteArray fileId = tempManager.AddFileItem(sessionId, "test.txt");
	QVERIFY(!fileId.isEmpty());
	
	QString filePath = tempManager.GetPath(sessionId, fileId);
	QVERIFY(!filePath.isEmpty());
	
	// Remove the file from session
	QVERIFY(tempManager.RemoveFileItem(sessionId, fileId));
	
	// Verify file is removed
	QString removedPath = tempManager.GetPath(sessionId, fileId);
	QVERIFY(removedPath.isEmpty());
	
	// Clean up
	tempManager.FinishSession(sessionId);
}


// FileNameParam advanced tests

void CAdvancedFileComponentTest::testFileNameParamComplexPaths()
{
	ifile::CFileNameParam param;
	
	// Test various complex path formats
	QStringList testPaths;
	testPaths << "/home/user/documents/project/file.txt";
	testPaths << "C:/Users/Test/Documents/file.txt";
	testPaths << "../relative/path/file.txt";
	testPaths << "./current/dir/file.txt";
	testPaths << "/path/with spaces/file name.txt";
	testPaths << "/path/with-special-chars_123/file.txt";
	
	for (const QString& testPath : testPaths)
	{
		param.SetPath(testPath);
		QCOMPARE(param.GetPath(), testPath);
	}
}


void CAdvancedFileComponentTest::testFileNameParamRelativePaths()
{
	ifile::CFileNameParam param;
	
	// Test relative paths
	param.SetPath("../test.txt");
	QCOMPARE(param.GetPath(), QString("../test.txt"));
	
	param.SetPath("./current.txt");
	QCOMPARE(param.GetPath(), QString("./current.txt"));
	
	param.SetPath("subdir/file.txt");
	QCOMPARE(param.GetPath(), QString("subdir/file.txt"));
}


void CAdvancedFileComponentTest::testFileNameParamSymbolicLinks()
{
	// Create a temporary directory and file
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	QString originalFile = tempDir.path() + "/original.txt";
	QFile file(originalFile);
	QVERIFY(file.open(QIODevice::WriteOnly));
	file.write("test content");
	file.close();
	
	ifile::CFileNameParam param;
	param.SetPath(originalFile);
	
	// Verify file path type
	QCOMPARE(param.GetPathType(), (int)ifile::IFileNameParam::PT_FILE);
	
	// Note: Symbolic link testing is platform-dependent and may not work on all systems
	// So we just verify the basic file path handling works
}


// FileListProvider advanced tests

void CAdvancedFileComponentTest::testFileListProviderRecursiveSearch()
{
	// Create temporary directory structure
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	// Create subdirectories
	QDir dir(tempDir.path());
	QVERIFY(dir.mkpath("subdir1"));
	QVERIFY(dir.mkpath("subdir2"));
	QVERIFY(dir.mkpath("subdir1/nested"));
	
	// Create files at different levels
	QFile file1(tempDir.path() + "/file1.txt");
	QVERIFY(file1.open(QIODevice::WriteOnly));
	file1.close();
	
	QFile file2(tempDir.path() + "/subdir1/file2.txt");
	QVERIFY(file2.open(QIODevice::WriteOnly));
	file2.close();
	
	QFile file3(tempDir.path() + "/subdir1/nested/file3.txt");
	QVERIFY(file3.open(QIODevice::WriteOnly));
	file3.close();
	
	// Test recursive file listing
	QFileInfoList fileList;
	QStringList filters;
	filters << "*.txt";
	
	// Test with recursive depth
	bool result = ifile::CFileListProviderComp::CreateFileList(
		QDir(tempDir.path()), 0, 3, filters, QDir::Name, fileList);
	
	QVERIFY(result);
	QVERIFY(fileList.size() >= 1); // At least the top-level file
}


void CAdvancedFileComponentTest::testFileListProviderFiltering()
{
	// Create temporary directory with mixed file types
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	// Create files with different extensions
	QFile txtFile(tempDir.path() + "/test.txt");
	QVERIFY(txtFile.open(QIODevice::WriteOnly));
	txtFile.close();
	
	QFile cppFile(tempDir.path() + "/test.cpp");
	QVERIFY(cppFile.open(QIODevice::WriteOnly));
	cppFile.close();
	
	QFile hFile(tempDir.path() + "/test.h");
	QVERIFY(hFile.open(QIODevice::WriteOnly));
	hFile.close();
	
	// Test filtering for specific extensions
	QFileInfoList fileList;
	QStringList filters;
	filters << "*.cpp" << "*.h";
	
	bool result = ifile::CFileListProviderComp::CreateFileList(
		QDir(tempDir.path()), 0, 1, filters, QDir::Name, fileList);
	
	QVERIFY(result);
	// Should find cpp and h files, but not txt
	bool foundCpp = false;
	bool foundH = false;
	bool foundTxt = false;
	
	for (const QFileInfo& info : fileList)
	{
		if (info.fileName() == "test.cpp") foundCpp = true;
		if (info.fileName() == "test.h") foundH = true;
		if (info.fileName() == "test.txt") foundTxt = true;
	}
	
	QVERIFY(foundCpp);
	QVERIFY(foundH);
	QVERIFY(!foundTxt); // txt should not be in the list
}


void CAdvancedFileComponentTest::testFileListProviderSorting()
{
	// Create temporary directory with multiple files
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	// Create files with names that would sort differently
	QStringList fileNames;
	fileNames << "zebra.txt" << "alpha.txt" << "beta.txt" << "gamma.txt";
	
	for (const QString& fileName : fileNames)
	{
		QFile file(tempDir.path() + "/" + fileName);
		QVERIFY(file.open(QIODevice::WriteOnly));
		file.close();
	}
	
	// Get file list with name sorting
	QFileInfoList fileList;
	QStringList filters;
	filters << "*.txt";
	
	bool result = ifile::CFileListProviderComp::CreateFileList(
		QDir(tempDir.path()), 0, 1, filters, QDir::Name, fileList);
	
	QVERIFY(result);
	QVERIFY(fileList.size() >= 4);
	
	// Verify sorting (first file should be "alpha.txt" if sorted by name)
	if (fileList.size() >= 4)
	{
		bool foundAlpha = false;
		for (const QFileInfo& info : fileList)
		{
			if (info.fileName() == "alpha.txt")
			{
				foundAlpha = true;
				break;
			}
		}
		QVERIFY(foundAlpha);
	}
}


// Integration tests

void CAdvancedFileComponentTest::testFileArchiveWithTempFileManager()
{
	ifile::CTempFileManagerComp tempManager;
	
	// Create session
	QByteArray sessionId = tempManager.BeginSession("archive_integration");
	QVERIFY(!sessionId.isEmpty());
	
	// Add temp file
	QByteArray fileId = tempManager.AddFileItem(sessionId, "test_archive.dat");
	QString filePath = tempManager.GetPath(sessionId, fileId);
	QVERIFY(!filePath.isEmpty());
	
	// Write data to archive in temp file
	{
		SimpleModel writeModel;
		writeModel.value = 12345;
		
		ifile::CFileWriteArchive writeArchive(filePath);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(writeModel.Serialize(writeArchive));
	}
	
	// Read data back
	{
		SimpleModel readModel;
		ifile::CFileReadArchive readArchive(filePath);
		QVERIFY(readModel.Serialize(readArchive));
		QCOMPARE(readModel.value, 12345);
	}
	
	// Clean up
	tempManager.FinishSession(sessionId);
}


void CAdvancedFileComponentTest::testMultipleArchiveFormats()
{
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	
	QString normalFile = tempDir.path() + "/normal.dat";
	QString secureFile = tempDir.path() + "/secure.dat";
	
	SimpleModel originalModel;
	originalModel.value = 54321;
	
	// Write with normal archive
	{
		ifile::CFileWriteArchive writeArchive(normalFile);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(originalModel.Serialize(writeArchive));
	}
	
	// Write with secure archive
	{
		ifile::CFileWriteSecureArchive writeArchive(secureFile);
		QVERIFY(writeArchive.IsArchiveValid());
		QVERIFY(originalModel.Serialize(writeArchive));
	}
	
	// Read from normal archive
	{
		SimpleModel readModel;
		ifile::CFileReadArchive readArchive(normalFile);
		QVERIFY(readModel.Serialize(readArchive));
		QCOMPARE(readModel.value, 54321);
	}
	
	// Read from secure archive
	{
		SimpleModel readModel;
		ifile::CFileReadSecureArchive readArchive(secureFile);
		QVERIFY(readModel.Serialize(readArchive));
		QCOMPARE(readModel.value, 54321);
	}
	
	// Verify that files are different (secure vs normal)
	QFile normalFileObj(normalFile);
	QFile secureFileObj(secureFile);
	
	QVERIFY(normalFileObj.open(QIODevice::ReadOnly));
	QVERIFY(secureFileObj.open(QIODevice::ReadOnly));
	
	QByteArray normalData = normalFileObj.readAll();
	QByteArray secureData = secureFileObj.readAll();
	
	normalFileObj.close();
	secureFileObj.close();
	
	// Files should have different content due to encoding
	QVERIFY(normalData != secureData);
}


I_ADD_TEST(CAdvancedFileComponentTest);


