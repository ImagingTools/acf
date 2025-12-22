#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <iprm/CIdParam.h>


class CIdParamTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void initTestCase();

	void GetSetTest();
	void SetSameIdTest();
	void SetEmptyIdTest();
	void GetSupportedOperationsTest();
	void ResetTest();
	void ResetEmptyTest();
	void SerializeTest();
	void CopyFromTest();
	void CopyFromIncompatibleTest();
	void CloneTest();
	void CloneWithModeTest();
	void IsEqualTest();
	void IsEqualDifferentIdTest();
	void IsEqualIncompatibleTest();

	void cleanupTestCase();

private:
	iprm::CIdParam m_param;
	const QByteArray m_id = "12345678";
};


