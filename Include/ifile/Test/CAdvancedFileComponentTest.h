// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>


class CAdvancedFileComponentTest : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	// TempFileManager advanced tests
	void testTempFileManagerNestedSessions();
	void testTempFileManagerSessionCleanup();
	void testTempFileManagerMultipleFilesInSession();
	void testTempFileManagerFileOperations();
	
	// FileNameParam advanced tests
	void testFileNameParamComplexPaths();
	void testFileNameParamRelativePaths();
	void testFileNameParamSymbolicLinks();
	
	// FileListProvider advanced tests
	void testFileListProviderRecursiveSearch();
	void testFileListProviderFiltering();
	void testFileListProviderSorting();
	
	// Integration tests
	void testFileArchiveWithTempFileManager();
	void testMultipleArchiveFormats();
};


