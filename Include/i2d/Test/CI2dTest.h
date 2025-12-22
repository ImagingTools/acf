#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CI2dTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoVector2dTest();
	void DoPosition2dTest();
	void DoCircleTest();
	void DoRectangleTest();
	void DoLine2dTest();
	void DoMatrix2dTest();
};


