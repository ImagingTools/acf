#pragma once

// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/CIdParam.h>


class CIdParamTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	void GetSetTest();
	void ResetTest();

	void SerializeTest();
	void CopyFromTest();
	void CloneTest();
	void IsEqualTest();

	void cleanupTestCase();

private:
	iprm::CIdParam m_param;
	const QByteArray m_id = "12345678";
};


