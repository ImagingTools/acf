#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CBitMemoryReadArchive.h>
#include <iser/CBitMemoryWriteArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CBitMemoryArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void BasicBitSerializationTest();
	void MultipleBoolsTest();
	void MixedDataTest();

private:
	class BoolModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag tag("BoolValue", "Boolean value");
			bool retVal = archive.BeginTag(tag);
			retVal = retVal && archive.Process(value);
			retVal = retVal && archive.EndTag(tag);
			return retVal;
		}

		bool value = false;
	};

	class MultiBoolModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag tag1("Bool1", "First boolean");
			static iser::CArchiveTag tag2("Bool2", "Second boolean");
			static iser::CArchiveTag tag3("Bool3", "Third boolean");

			bool retVal = archive.BeginTag(tag1);
			retVal = retVal && archive.Process(value1);
			retVal = retVal && archive.EndTag(tag1);

			retVal = retVal && archive.BeginTag(tag2);
			retVal = retVal && archive.Process(value2);
			retVal = retVal && archive.EndTag(tag2);

			retVal = retVal && archive.BeginTag(tag3);
			retVal = retVal && archive.Process(value3);
			retVal = retVal && archive.EndTag(tag3);

			return retVal;
		}

		bool value1 = false;
		bool value2 = false;
		bool value3 = false;

		bool operator==(const MultiBoolModel& other) const
		{
			return value1 == other.value1 && 
			       value2 == other.value2 && 
			       value3 == other.value3;
		}
	};

	class MixedModel: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag boolTag("BoolValue", "Boolean value");
			static iser::CArchiveTag intTag("IntValue", "Integer value");

			bool retVal = archive.BeginTag(boolTag);
			retVal = retVal && archive.Process(boolValue);
			retVal = retVal && archive.EndTag(boolTag);

			retVal = retVal && archive.BeginTag(intTag);
			retVal = retVal && archive.Process(intValue);
			retVal = retVal && archive.EndTag(intTag);

			return retVal;
		}

		bool boolValue = false;
		int intValue = 0;

		bool operator==(const MixedModel& other) const
		{
			return boolValue == other.boolValue && intValue == other.intValue;
		}
	};
};


