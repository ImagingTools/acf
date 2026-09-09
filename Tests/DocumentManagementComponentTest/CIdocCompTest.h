// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <optional>

// Qt includes
#include <QtCore/QObject>
#include <QtTest/QtTest>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>
#include <iser/IArchive.h>
#include <iser/ISerializable.h>
#include <iser/IVersionInfo.h>
#include <imod/CSingleModelObserverBase.h>
#include <imod/TModelWrap.h>
#include <idoc/ITextDocument.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentTemplate.h>
#include <itest/CStandardTestExecutor.h>
#include <GeneratedFiles/DocumentManagementComponentTest/CDocumentManagementComponentTest.h>

class CIdocCompTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();

	// Tests for CTextDocumentComp
	void testTextDocumentCreation();
	void testTextDocumentGetSet();
	void testTextDocumentDefaultText();
	void testTextDocumentSerialization();

	// Tests for CSerializedUndoManagerComp
	void testUndoManagerCreation();
	void testUndoManagerUndoRedo();
	void testUndoManagerMultipleSteps();
	void testUndoManagerMaxBufferSize();
	void testUndoManagerReset();
	void testUndoManagerStateComparison();
	void testUndoManagerObserverSeesChangedDocumentState();
	void testUndoManagerVersionDependentDocument();

	// Tests for CSingleDocumentTemplateComp
	void testDocumentTemplateCreation();
	void testDocumentTemplateCreateDocument();
	void testDocumentTemplateAttributes();

	void cleanupTestCase();

private:
	class CDocumentStateObserver: public imod::CSingleModelObserverBase
	{
	public:
		CDocumentStateObserver()
			: m_documentChangeFlag(idoc::IDocumentStateComparator::DCF_UNKNOWN)
		{
		}

		idoc::IDocumentStateComparator::DocumentChangeFlag GetDocumentChangeFlag() const
		{
			return m_documentChangeFlag;
		}

		// Sets the document change flag expected to be seen while handling the next
		// model update notification. The check is performed directly inside OnUpdate(),
		// so it verifies the state as observed at notification time, not afterwards.
		void SetExpectedDocumentChangeFlag(idoc::IDocumentStateComparator::DocumentChangeFlag expectedFlag)
		{
			m_expectedDocumentChangeFlag = expectedFlag;
		}

	protected:
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/) override
		{
			idoc::IDocumentStateComparator* comparator = dynamic_cast<idoc::IDocumentStateComparator*>(GetObservedModel());
			QVERIFY(comparator != nullptr);

			m_documentChangeFlag = comparator->GetDocumentChangeFlag();

			if (m_expectedDocumentChangeFlag.has_value()){
				QCOMPARE(m_documentChangeFlag, *m_expectedDocumentChangeFlag);
			}
		}

	private:
		idoc::IDocumentStateComparator::DocumentChangeFlag m_documentChangeFlag;
		std::optional<idoc::IDocumentStateComparator::DocumentChangeFlag> m_expectedDocumentChangeFlag;
	};

	// Serializable fixture containing a data field which is only serialized if the archive
	// provides the user version information configured at the undo manager.
	class CVersionDependentDocument: public virtual iser::ISerializable
	{
	public:
		enum{
			VersionedValueVersion = 2
		};

		CVersionDependentDocument()
			: m_versionedValue(0)
		{
		}

		int GetVersionedValue() const
		{
			return m_versionedValue;
		}

		void SetVersionedValue(int value)
		{
			if (m_versionedValue != value){
				istd::CChangeNotifier notifier(this);
				Q_UNUSED(notifier);

				m_versionedValue = value;
			}
		}

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override
		{
			static iser::CArchiveTag versionedValueTag("VersionedValue", "Version dependent value", iser::CArchiveTag::TT_LEAF);

			quint32 versionNumber = 0;
			if (		!archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::UserVersionId, versionNumber) ||
						(versionNumber < VersionedValueVersion)){
				return true;
			}

			bool retVal = archive.BeginTag(versionedValueTag);
			retVal = retVal && archive.Process(m_versionedValue);
			retVal = retVal && archive.EndTag(versionedValueTag);

			return retVal;
		}

	private:
		int m_versionedValue;
	};

	typedef imod::TModelWrap<CVersionDependentDocument> VersionDependentDocumentModel;

	std::shared_ptr<CDocumentManagementComponentTest> m_testInstanceCompPtr;

	idoc::ITextDocument* m_textDocumentPtr = nullptr;
	idoc::ITextDocument* m_textDocumentWithDefaultPtr = nullptr;
	idoc::IUndoManager* m_undoManagerPtr = nullptr;
	idoc::IUndoManager* m_undoManagerSmallBufferPtr = nullptr;
	idoc::IUndoManager* m_undoManagerWithVersionInfoPtr = nullptr;
	idoc::IDocumentTemplate* m_documentTemplatePtr = nullptr;
};
