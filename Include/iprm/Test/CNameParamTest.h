#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/CNameParam.h>


class CNameParamTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void GetSetTest();
	void ResetTest();

	void SerializeTest();
	void CopyFromTest();
	void CloneTest();
	void IsEqualTest();

	void cleanupTestCase();

private:
	iprm::CNameParam m_param;
	const QByteArray m_refValue = "12345678";
};


