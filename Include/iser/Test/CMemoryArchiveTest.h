#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CMemoryArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicSerializationTest();
	void PrimitiveTypesTest();
	void StringSerializationTest();
	void ResetPositionTest();

private:
	class SimpleModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag tag("Value", "Integer value");
			bool retVal = archive.BeginTag(tag);
			retVal = retVal && archive.Process(value);
			retVal = retVal && archive.EndTag(tag);
			return retVal;
		}

		int value = 0;
	};

	class ComplexModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag intTag("IntValue", "Integer value");
			static iser::CArchiveTag doubleTag("DoubleValue", "Double value");
			static iser::CArchiveTag stringTag("StringValue", "String value");

			bool retVal = archive.BeginTag(intTag);
			retVal = retVal && archive.Process(intValue);
			retVal = retVal && archive.EndTag(intTag);

			retVal = retVal && archive.BeginTag(doubleTag);
			retVal = retVal && archive.Process(doubleValue);
			retVal = retVal && archive.EndTag(doubleTag);

			retVal = retVal && archive.BeginTag(stringTag);
			retVal = retVal && archive.Process(stringValue);
			retVal = retVal && archive.EndTag(stringTag);

			return retVal;
		}

		int intValue = 0;
		double doubleValue = 0.0;
		QString stringValue;

		bool operator==(const ComplexModel& other) const
		{
			return intValue == other.intValue && 
			       qFuzzyCompare(doubleValue, other.doubleValue) && 
			       stringValue == other.stringValue;
		}
	};
};


