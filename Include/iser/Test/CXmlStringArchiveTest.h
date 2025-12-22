#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CXmlStringReadArchive.h>
#include <iser/CXmlStringWriteArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CXmlStringArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicXmlSerializationTest();
	void XmlPrimitiveTypesTest();
	void XmlArraySerializationTest();

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
			static iser::CArchiveTag boolTag("BoolValue", "Boolean value");

			bool retVal = archive.BeginTag(intTag);
			retVal = retVal && archive.Process(intValue);
			retVal = retVal && archive.EndTag(intTag);

			retVal = retVal && archive.BeginTag(doubleTag);
			retVal = retVal && archive.Process(doubleValue);
			retVal = retVal && archive.EndTag(doubleTag);

			retVal = retVal && archive.BeginTag(boolTag);
			retVal = retVal && archive.Process(boolValue);
			retVal = retVal && archive.EndTag(boolTag);

			return retVal;
		}

		int intValue = 0;
		double doubleValue = 0.0;
		bool boolValue = false;

		bool operator==(const ComplexModel& other) const
		{
			return intValue == other.intValue && 
			       qFuzzyCompare(doubleValue, other.doubleValue) && 
			       boolValue == other.boolValue;
		}
	};

	class ArrayModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag arrayTag("Items", "Items array", iser::CArchiveTag::TT_MULTIPLE);
			static iser::CArchiveTag itemTag("Item", "Single item", iser::CArchiveTag::TT_LEAF, &arrayTag);

			bool retVal = true;
			int count = items.size();

			retVal = retVal && archive.BeginMultiTag(arrayTag, itemTag, count);
			if (!archive.IsStoring()) {
				items.resize(count);
			}

			for (int i = 0; i < items.size(); ++i) {
				retVal = retVal && archive.BeginTag(itemTag);
				retVal = retVal && archive.Process(items[i]);
				retVal = retVal && archive.EndTag(itemTag);
			}

			retVal = retVal && archive.EndTag(arrayTag);
			return retVal;
		}

		QList<int> items;

		bool operator==(const ArrayModel& other) const
		{
			return items == other.items;
		}
	};
};


