// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CIdocCompTest.h"


// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <idoc/IDocumentStateComparator.h>


// protected slots

void CIdocCompTest::initTestCase()
{
	m_testInstanceCompPtr.reset(new CDocumentManagementComponentTest);

	m_textDocumentPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocument");
	m_textDocumentWithDefaultPtr = m_testInstanceCompPtr->GetInterface<idoc::ITextDocument>("TextDocumentWithDefault");
	m_undoManagerPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManager");
	m_undoManagerSmallBufferPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManagerSmallBuffer");
	m_undoManagerWithVersionInfoPtr = m_testInstanceCompPtr->GetInterface<idoc::IUndoManager>("UndoManagerWithVersionInfo");
	m_documentTemplatePtr = m_testInstanceCompPtr->GetInterface<idoc::IDocumentTemplate>("DocumentTemplate");

	QVERIFY(m_textDocumentPtr != nullptr);
	QVERIFY(m_textDocumentWithDefaultPtr != nullptr);
	QVERIFY(m_undoManagerPtr != nullptr);
	QVERIFY(m_undoManagerSmallBufferPtr != nullptr);
	QVERIFY(m_undoManagerWithVersionInfoPtr != nullptr);
	QVERIFY(m_documentTemplatePtr != nullptr);
}


// Tests for CTextDocumentComp

void CIdocCompTest::testTextDocumentCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_textDocumentPtr != nullptr);
	
	// Check that the text is not null
	QString text = m_textDocumentPtr->GetText();
	QVERIFY(text.isEmpty());
}


void CIdocCompTest::testTextDocumentGetSet()
{
	// Set a specific text
	QString testText = "Test document content";
	m_textDocumentPtr->SetText(testText);
	
	// Verify the text was set correctly
	QCOMPARE(m_textDocumentPtr->GetText(), testText);
	
	// Set different text
	QString newText = "Updated content";
	m_textDocumentPtr->SetText(newText);
	QCOMPARE(m_textDocumentPtr->GetText(), newText);
	
	// Test with empty string
	m_textDocumentPtr->SetText("");
	QCOMPARE(m_textDocumentPtr->GetText(), QString(""));
}


void CIdocCompTest::testTextDocumentDefaultText()
{
	// Verify the document with default text has the configured value
	QString defaultText = m_textDocumentWithDefaultPtr->GetText();
	QCOMPARE(defaultText, QString("Custom Default Text"));
}


void CIdocCompTest::testTextDocumentSerialization()
{
	// Set a specific text
	QString testText = "Serialization test content";
	m_textDocumentPtr->SetText(testText);
	
	// Serialize to memory
	iser::ISerializable* serializable = dynamic_cast<iser::ISerializable*>(m_textDocumentPtr);
	QVERIFY(serializable != nullptr);
	
	iser::CMemoryWriteArchive writeArchive(nullptr);
	QVERIFY(serializable->Serialize(writeArchive));
	
	// Change the text
	m_textDocumentPtr->SetText("Different text");
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Different text"));
	
	// Deserialize from memory
	iser::CMemoryReadArchive readArchive(writeArchive);
	QVERIFY(serializable->Serialize(readArchive));
	
	// Verify the text was restored
	QCOMPARE(m_textDocumentPtr->GetText(), testText);
}


// Tests for CSerializedUndoManagerComp

void CIdocCompTest::testUndoManagerCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_undoManagerPtr != nullptr);
	
	// Check initial state - should have no undo/redo steps
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
}


void CIdocCompTest::testUndoManagerUndoRedo()
{
	// Get the document as IChangeable for undo manager attachment
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	model->AttachObserver(observer);
	
	// Set initial text
	m_textDocumentPtr->SetText("Initial text");
	
	// Make a change - this should create an undo step
	m_textDocumentPtr->SetText("Modified text");
	
	// Verify we have an undo step
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
	
	// Perform undo
	QVERIFY(m_undoManagerPtr->DoUndo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Initial text"));
	
	// After undo, we should have a redo step
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() > 0);
	
	// Perform redo
	QVERIFY(m_undoManagerPtr->DoRedo());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Modified text"));
	
	// Clean up
	model->DetachObserver(observer);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerMultipleSteps()
{
	// Get the document as IChangeable
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	model->AttachObserver(observer);
	
	// Reset undo to start fresh
	m_undoManagerPtr->ResetUndo();
	
	// Make multiple changes
	m_textDocumentPtr->SetText("Step 0");
	m_textDocumentPtr->SetText("Step 1");
	m_textDocumentPtr->SetText("Step 2");
	m_textDocumentPtr->SetText("Step 3");
	
	// Should have 3 undo steps (changes from step 0 to 1, 1 to 2, 2 to 3)
	int undoSteps = m_undoManagerPtr->GetAvailableUndoSteps();
	QVERIFY(undoSteps >= 3);
	
	// Undo twice
	QVERIFY(m_undoManagerPtr->DoUndo(2));
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Step 1"));
	
	// Should have 2 redo steps available
	QVERIFY(m_undoManagerPtr->GetAvailableRedoSteps() >= 2);
	
	// Clean up
	model->DetachObserver(observer);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerMaxBufferSize()
{
	// Verify that the small buffer undo manager was created
	QVERIFY(m_undoManagerSmallBufferPtr != nullptr);
	
	// The small buffer is configured with MaxBufferSize=1 (1 MB)
	// This test verifies the component creation with different buffer size
	QCOMPARE(m_undoManagerSmallBufferPtr->GetAvailableUndoSteps(), 0);
}


void CIdocCompTest::testUndoManagerReset()
{
	// Get the document as IChangeable
	istd::IChangeable* changeable = dynamic_cast<istd::IChangeable*>(m_textDocumentPtr);
	QVERIFY(changeable != nullptr);
	
	// Get undo manager as observer
	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);
	
	// Attach undo manager to document
	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	model->AttachObserver(observer);
	
	// Reset to start fresh
	m_undoManagerPtr->ResetUndo();
	
	// Make some changes
	m_textDocumentPtr->SetText("Change 1");
	m_textDocumentPtr->SetText("Change 2");
	
	// Verify we have undo steps
	QVERIFY(m_undoManagerPtr->GetAvailableUndoSteps() > 0);
	
	// Reset undo
	m_undoManagerPtr->ResetUndo();
	
	// Verify all undo/redo steps are cleared
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(m_undoManagerPtr->GetAvailableRedoSteps(), 0);
	
	// Clean up
	model->DetachObserver(observer);
}


void CIdocCompTest::testUndoManagerStateComparison()
{
	// Get state comparator interface
	idoc::IDocumentStateComparator* comparator = 
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr);
	QVERIFY(comparator != nullptr);

	imod::IObserver* observer = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(observer != nullptr);

	imod::IModel* model = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(model != nullptr);
	QVERIFY(model->AttachObserver(observer));
	m_undoManagerPtr->ResetUndo();

	QVERIFY(!comparator->HasStoredDocumentState());
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_UNKNOWN);

	m_textDocumentPtr->SetText("Stored document state");
	QVERIFY(comparator->StoreDocumentState());
	QVERIFY(comparator->HasStoredDocumentState());
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	m_textDocumentPtr->SetText("Changed document state");
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);

	QVERIFY(comparator->RestoreDocumentState());
	QCOMPARE(m_textDocumentPtr->GetText(), QString("Stored document state"));
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	model->DetachObserver(observer);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerObserverSeesChangedDocumentState()
{
	idoc::IDocumentStateComparator* comparator =
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr);
	QVERIFY(comparator != nullptr);

	imod::IObserver* undoManagerObserver = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(undoManagerObserver != nullptr);

	imod::IModel* documentModel = dynamic_cast<imod::IModel*>(m_textDocumentPtr);
	QVERIFY(documentModel != nullptr);
	QVERIFY(documentModel->AttachObserver(undoManagerObserver));

	CDocumentStateObserver stateObserver;
	imod::IModel* undoManagerModel = dynamic_cast<imod::IModel*>(m_undoManagerPtr);
	QVERIFY(undoManagerModel != nullptr);
	QVERIFY(undoManagerModel->AttachObserver(&stateObserver));

	QVERIFY(comparator->StoreDocumentState());
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	stateObserver.SetExpectedDocumentChangeFlag(idoc::IDocumentStateComparator::DCF_DIFFERENT);
	m_textDocumentPtr->SetText("Changed document state during observer update");
	QCOMPARE(stateObserver.GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);

	undoManagerModel->DetachObserver(&stateObserver);
	documentModel->DetachObserver(undoManagerObserver);
	m_undoManagerPtr->ResetUndo();
}


void CIdocCompTest::testUndoManagerVersionDependentDocument()
{
	idoc::IDocumentStateComparator* comparator =
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerWithVersionInfoPtr);
	QVERIFY(comparator != nullptr);

	imod::IObserver* undoManagerObserver = dynamic_cast<imod::IObserver*>(m_undoManagerWithVersionInfoPtr);
	QVERIFY(undoManagerObserver != nullptr);

	VersionDependentDocumentModel document;
	QVERIFY(document.AttachObserver(undoManagerObserver));

	m_undoManagerWithVersionInfoPtr->ResetUndo();

	document.SetVersionedValue(1);
	QVERIFY(comparator->StoreDocumentState());
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);
	m_undoManagerWithVersionInfoPtr->ResetUndo();

	// Change of the version dependent value is only visible in the serialized stream
	// if the version information is provided to the internal archives.
	document.SetVersionedValue(42);
	QCOMPARE(m_undoManagerWithVersionInfoPtr->GetAvailableUndoSteps(), 1);
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);

	QVERIFY(m_undoManagerWithVersionInfoPtr->DoUndo());
	QCOMPARE(document.GetVersionedValue(), 1);
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	QVERIFY(m_undoManagerWithVersionInfoPtr->DoRedo());
	QCOMPARE(document.GetVersionedValue(), 42);
	QCOMPARE(comparator->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_DIFFERENT);

	document.DetachObserver(undoManagerObserver);
	m_undoManagerWithVersionInfoPtr->ResetUndo();

	// Without configured version information the version dependent value is skipped
	// during the serialization, so no undo step and no state difference can be detected.
	imod::IObserver* undoManagerWithoutVersionObserver = dynamic_cast<imod::IObserver*>(m_undoManagerPtr);
	QVERIFY(undoManagerWithoutVersionObserver != nullptr);

	idoc::IDocumentStateComparator* comparatorWithoutVersion =
		dynamic_cast<idoc::IDocumentStateComparator*>(m_undoManagerPtr);
	QVERIFY(comparatorWithoutVersion != nullptr);

	QVERIFY(document.AttachObserver(undoManagerWithoutVersionObserver));
	m_undoManagerPtr->ResetUndo();

	QVERIFY(comparatorWithoutVersion->StoreDocumentState());

	document.SetVersionedValue(123);
	QCOMPARE(m_undoManagerPtr->GetAvailableUndoSteps(), 0);
	QCOMPARE(comparatorWithoutVersion->GetDocumentChangeFlag(), idoc::IDocumentStateComparator::DCF_EQUAL);

	document.DetachObserver(undoManagerWithoutVersionObserver);
	m_undoManagerPtr->ResetUndo();
}


// Tests for CSingleDocumentTemplateComp

void CIdocCompTest::testDocumentTemplateCreation()
{
	// Verify the component was created successfully
	QVERIFY(m_documentTemplatePtr != nullptr);
	
	// Check that template provides basic information
	QVERIFY(m_documentTemplatePtr->GetDocumentTypeIds().size() > 0);
}


void CIdocCompTest::testDocumentTemplateCreateDocument()
{
	// Get the document type ID
	QByteArray docTypeId = m_documentTemplatePtr->GetDocumentTypeIds().value(0);
	QVERIFY(!docTypeId.isEmpty());
	
	// Create a new document using the template
	istd::IChangeableUniquePtr documentPtr = m_documentTemplatePtr->CreateDocument(docTypeId);
	QVERIFY(documentPtr.GetPtr() != nullptr);
	
	// Verify the document implements ITextDocument
	idoc::ITextDocument* textDoc = dynamic_cast<idoc::ITextDocument*>(documentPtr.GetPtr());
	QVERIFY(textDoc != nullptr);
	
	// Verify the document has the default text
	QCOMPARE(textDoc->GetText(), QString("Template Default Text"));
}


void CIdocCompTest::testDocumentTemplateAttributes()
{
	// Verify document type ID
	QByteArray docTypeId = m_documentTemplatePtr->GetDocumentTypeIds().value(0);
	QCOMPARE(docTypeId, QByteArray("TestDocType"));
	
	// Check if new operation is supported
	QVERIFY(m_documentTemplatePtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, docTypeId));
	
	// Check if edit operation is supported
	QVERIFY(m_documentTemplatePtr->IsFeatureSupported(idoc::IDocumentTypesInfo::SF_EDIT_DOCUMENT, docTypeId));
	
	// Get document type name
	QString docTypeName = m_documentTemplatePtr->GetDocumentTypeName(docTypeId);
	QCOMPARE(docTypeName, QString("Test Document Type"));
}


void CIdocCompTest::cleanupTestCase()
{
	m_testInstanceCompPtr.reset();
}


I_ADD_TEST(CIdocCompTest);
