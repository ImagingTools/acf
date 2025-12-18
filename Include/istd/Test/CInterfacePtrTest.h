#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CInterfacePtrTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoSharedInterfaceTest();
	void DoUniqueInterfaceTest();
	void DoOptionalInterfaceTest();
};


