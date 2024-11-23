#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CSubstraactiveColorModelTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoSerializeTest();
	void DoAppendModelTest();
	void CheckColorantsTest();
	void RemoveColorantTest();
	void EqualTest();
};


