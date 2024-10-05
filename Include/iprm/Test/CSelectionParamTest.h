#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>
#include <imod/TModelWrap.h>
#include <iprm/CSelectionParam.h>


class CSelectionParamTest: public QObject
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
	imod::TModelWrap<iprm::CSelectionParam> m_param;
};


