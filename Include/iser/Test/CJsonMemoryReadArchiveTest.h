#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <iser/CJsonMemReadArchive.h>
#include <iser/ISerializable.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


class CJsonMemoryReadArchiveTest: public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void DoTest();
	void DoArrayTest();

private:
	class Model: virtual public iser::ISerializable
	{
	public:
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag typeTag("Type", "Type of the meta information");
			bool retVal = archive.BeginTag(typeTag);
			retVal = retVal && archive.Process(value);
			retVal = retVal && archive.EndTag(typeTag);

			return retVal;
		}

		int value = 42;
	};

	class NestedArray : public iser::ISerializable
	{
	public:
		NestedArray() = default;

		NestedArray(QList<QList<double>> data)
			: m_data(data)
		{
		}

		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag outerListTag("Data", "DataDesc", iser::CArchiveTag::TT_MULTIPLE);
			static iser::CArchiveTag innerListTag("Element", "ElementDesc1", iser::CArchiveTag::TT_MULTIPLE, &outerListTag);
			static iser::CArchiveTag elementTag("Element1", "ElementDesc2", iser::CArchiveTag::TT_LEAF, &innerListTag);

			bool retVal = true;

			int outerCount = m_data.size();
			retVal = retVal && archive.BeginMultiTag(outerListTag, innerListTag, outerCount);
			if (!archive.IsStoring()) {
				m_data.resize(outerCount);
			}

			for (qsizetype i = 0; i < m_data.size(); ++i) {
				int innerCount = m_data[i].size();
				retVal = retVal && archive.BeginMultiTag(innerListTag, elementTag, innerCount);
				if (!archive.IsStoring()) {
					m_data[i].resize(innerCount);
				}

				for (qsizetype j = 0; j < m_data[i].size(); ++j) {
					retVal = retVal && archive.BeginTag(elementTag);
					retVal = retVal && archive.Process(m_data[i][j]);
					retVal = retVal && archive.EndTag(elementTag);
				}

				retVal = retVal && archive.EndTag(innerListTag);
			}

			retVal = retVal && archive.EndTag(outerListTag);

			return retVal;
		}

		bool operator==(const NestedArray& other) const
		{
			return m_data == other.m_data;
		}

	private:
		QList<QList<double>> m_data;
	};

};


