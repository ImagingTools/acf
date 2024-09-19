#pragma once

// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <itest/CStandardTestExecutor.h>

// ImtCore includes
#include <iprm/CIdParam.h>


class CIdParamTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
};


