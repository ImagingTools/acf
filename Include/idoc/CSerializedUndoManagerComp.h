// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/TDelPtr.h>
#include <icomp/CComponentBase.h>
#include <iser/IVersionInfo.h>
#include <idoc/CDocumentStateComparator.h>
#include <idoc/IUndoManager.h>


namespace idoc
{


/**
	Implements multi-level UNDO mechanism based on storing complete object state at each step using serialization.
	
	This component provides a complete undo/redo implementation by serializing the entire
	document state before each change. It maintains separate undo and redo stacks and
	automatically manages memory usage by limiting the buffer size.
	
	The undo manager observes the document model and automatically captures state snapshots
	when changes occur. It integrates with the ACF model/observer pattern to track when
	undo/redo availability changes.
	
	\par Component Attributes
	- \b MaxBufferSize - Maximum memory size for undo buffer in megabytes (default: 100 MB)
	- \b VersionInfo - Optional reference to iser::IVersionInfo used to create the internal memory archives.
	If it is not set, all version dependent data will be skipped during serialization of the document state.
	
	\par Registered Interfaces
	- idoc::IUndoManager - Provides undo/redo operations
	- idoc::IDocumentStateComparator - Allows state comparison
	- imod::IObserver - Observes document changes
	
	\par Configuration Example
	\code
	CSerializedUndoManagerComp {
		MaxBufferSize = 50  // Limit undo buffer to 50 MB
		VersionInfo = "VersionInfo"  // Version info of the application
	}
	\endcode
	
	\par Usage with Document Template
	\code
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentFactory = CTextDocumentComp
		UndoManager = CSerializedUndoManagerComp {
			MaxBufferSize = 100
		}
	}
	\endcode
	
	\par How It Works
	The undo manager:
	1. Attaches to a serializable document as an observer
	2. Before each document change, serializes current state to memory
	3. Stores serialized state in undo stack with a description
	4. On undo, deserializes previous state from stack
	5. Manages memory by removing oldest undo steps when buffer is full
	
	\par Usage Example
	\code
	// Get undo manager for document
	idoc::IUndoManager* undoMgr = GetUndoManager();
	
	// Make changes to document
	document->SetText("New text");
	
	// Undo manager automatically captured state before change
	if (undoMgr->GetAvailableUndoSteps() > 0) {
		QString desc = undoMgr->GetUndoLevelDescription(0);
		qDebug() << "Can undo:" << desc;
		undoMgr->DoUndo();
	}
	
	// After undo, redo becomes available
	if (undoMgr->GetAvailableRedoSteps() > 0) {
		undoMgr->DoRedo();
	}
	
	// Clear all undo history
	undoMgr->ResetUndo();
	
	// Check if document has unsaved changes
	idoc::IDocumentStateComparator::DocumentChangeFlag flag = 
		undoMgr->GetDocumentChangeFlag();
	if (flag == idoc::IDocumentStateComparator::DCF_DIFFERENT) {
		// Document has been modified
	}
	\endcode
	
	\note The document must implement iser::ISerializable for this undo manager to work.
	\note Memory usage is automatically managed based on MaxBufferSize setting.
	\note Each undo step stores the complete document state, so large documents may use significant memory.
	
	\sa IUndoManager, IDocumentStateComparator
	\ingroup DocumentBasedFramework
*/
class CSerializedUndoManagerComp:
			public icomp::CComponentBase,
			public CDocumentStateComparator,
			virtual public IUndoManager
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CDocumentStateComparator BaseClass2;

	I_BEGIN_COMPONENT(CSerializedUndoManagerComp);
		I_REGISTER_INTERFACE(idoc::IUndoManager);
		I_REGISTER_INTERFACE(idoc::IDocumentStateComparator);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_maxBufferSizeAttrPtr, "MaxBufferSize", "Maximal memory size of the Undo-buffer in MByte", false, 100);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions used to serialize the document state", false, "VersionInfo");
	I_END_COMPONENT;

	CSerializedUndoManagerComp();

	// reimplemented (idoc::IUndoManager)
	virtual int GetAvailableUndoSteps() const override;
	virtual int GetAvailableRedoSteps() const override;
	virtual QString GetUndoLevelDescription(int stepIndex) const override;
	virtual QString GetRedoLevelDescription(int stepIndex) const override;
	virtual void ResetUndo() override;
	virtual bool DoUndo(int steps = 1) override;
	virtual bool DoRedo(int steps = 1) override;

protected:
	typedef istd::TDelPtr<iser::CMemoryWriteArchive> UndoArchivePtr;
	struct UndoStepInfo
	{
		UndoArchivePtr archivePtr;
		QString description;
	};
	typedef QList<UndoStepInfo> UndoList;

	bool DoListShift(int steps, UndoList& fromList, UndoList& toList);

	// reimplemented (idoc::IDocumentStateComparator)
	virtual bool StoreDocumentState() override;
	virtual bool RestoreDocumentState() override;

	// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)
	virtual iser::ISerializable* CastFromModel(imod::IModel* modelPtr) const override;

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr) override;
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	qint64 GetUsedMemorySize() const;

	UndoList m_undoList;
	UndoList m_redoList;

	UndoArchivePtr m_beginStateArchivePtr;

	bool m_isBlocked;

	I_ATTR(int, m_maxBufferSizeAttrPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace idoc

