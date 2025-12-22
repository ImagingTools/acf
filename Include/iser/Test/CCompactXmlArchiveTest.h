#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CCompactXmlMemReadArchive.h>
#include <iser/CCompactXmlMemWriteArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CCompactXmlArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicCompactXmlSerializationTest();
	void CompactXmlComplexDataTest();
	void CompactXmlArrayTest();

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

	class ArrayModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag arrayTag("Numbers", "Numbers array", iser::CArchiveTag::TT_MULTIPLE);
			static iser::CArchiveTag itemTag("Number", "Single number", iser::CArchiveTag::TT_LEAF, &arrayTag);

			bool retVal = true;
			int count = numbers.size();

			retVal = retVal && archive.BeginMultiTag(arrayTag, itemTag, count);
			if (!archive.IsStoring()) {
				numbers.resize(count);
			}

			for (int i = 0; i < numbers.size(); ++i) {
				retVal = retVal && archive.BeginTag(itemTag);
				retVal = retVal && archive.Process(numbers[i]);
				retVal = retVal && archive.EndTag(itemTag);
			}

			retVal = retVal && archive.EndTag(arrayTag);
			return retVal;
		}

		QList<double> numbers;

		bool operator==(const ArrayModel& other) const
		{
			if (numbers.size() != other.numbers.size())
				return false;
			for (int i = 0; i < numbers.size(); ++i) {
				if (!qFuzzyCompare(numbers[i], other.numbers[i]))
					return false;
			}
			return true;
		}
	};
};


