#include "CI3dTest.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtTest/QtTest>


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
	CI3dTest testInstance;
	return QTest::qExec(&testInstance, argc, argv);
}
