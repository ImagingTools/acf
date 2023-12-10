#ifndef idoc_CSerializedUndoManagerComp_included
#define idoc_CSerializedUndoManagerComp_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/TDelPtr.h>
#include <iser/ISerializable.h>
#include <iser/CMemoryWriteArchive.h>
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <idoc/IUndoManager.h>


namespace idoc
{


/**
	Implements multi level UNDO mechanism based on storing complete objects state at each step using serialization.
	To trace if UNDO is available you can use ACF model/observer.
*/
class CSerializedUndoManagerComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<iser::ISerializable>,
			virtual public IUndoManager
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iser::ISerializable> BaseClass2;

	I_BEGIN_COMPONENT(CSerializedUndoManagerComp);
		I_REGISTER_INTERFACE(idoc::IUndoManager);
		I_REGISTER_INTERFACE(idoc::IDocumentStateComparator);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_maxBufferSizeAttrPtr, "MaxBufferSize", "Maximal memory size of the Undo-buffer in MByte", false, 100);
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

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual bool OnModelDetached(imod::IModel* modelPtr) override;

protected:
	typedef istd::TDelPtr<iser::CMemoryWriteArchive> UndoArchivePtr;
	struct UndoStepInfo
	{
		UndoArchivePtr archivePtr;
		QString description;
	};
	typedef QList<UndoStepInfo> UndoList;

	bool DoListShift(int steps, UndoList& fromList, UndoList& toList);

	// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)
	virtual iser::ISerializable* CastFromModel(imod::IModel* modelPtr) const override;

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr) override;
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (idoc::IDocumentStateComparator)
	virtual bool HasStoredDocumentState() const override;
	virtual bool StoreDocumentState() override;
	virtual bool RestoreDocumentState() override;
	virtual DocumentChangeFlag GetDocumentChangeFlag() const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

private:
	qint64 GetUsedMemorySize() const;

	UndoList m_undoList;
	UndoList m_redoList;

	UndoArchivePtr m_beginStateArchivePtr;

	bool m_hasStoredDocumentState;
	bool m_isBlocked;

	iser::CMemoryWriteArchive m_storedStateArchive;

	mutable DocumentChangeFlag m_stateChangedFlag;
	mutable bool m_isStateChangedFlagValid;

	I_ATTR(int, m_maxBufferSizeAttrPtr);
};


} // namespace idoc


#endif // !idoc_CSerializedUndoManagerComp_included


