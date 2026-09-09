// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>


class CIlluminantTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void StandardIlluminantConstructorTest();
	void CustomIlluminantConstructorTest();
	void CopyConstructorTest();
	void SettersTest();
	void FactoryIdTest();
	void SerializeTest();
	void SerializeLegacyWhitePointTest();
	void IsEqualTest();
	void CopyFromTest();
	void CloneMeTest();
};
