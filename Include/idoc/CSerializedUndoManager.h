#ifndef idoc_CSerializedUndoManager_included
#define idoc_CSerializedUndoManager_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"
#include "iser/CMemoryWriteArchive.h"

#include "imod/TSingleModelObserverBase.h"

#include "idoc/IUndoManager.h"


namespace idoc
{


/**
	Implements multi level UNDO mechanism based on storing complete objects state at each step using serialization.
	To trace if UNDO is available you can use ACF model/observer.
*/
class CSerializedUndoManager: public imod::TSingleModelObserverBase<iser::ISerializable>, virtual public IUndoManager
{
public:
	/**
		Default constructor with optional model pointer to set managed model.
		You can also use standard connection to observed model.
	*/
	CSerializedUndoManager();

	// reimplemented (idoc::IUndoManager)
	virtual bool IsUndoAvailable() const;
	virtual bool IsRedoAvailable() const;
	virtual void ResetUndo();
	virtual bool DoUndo();
	virtual bool DoRedo();

protected:
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef istd::TDelPtr<iser::CMemoryWriteArchive> UndoArchivePtr;
	typedef QList<UndoArchivePtr> UndoList;
	UndoList m_undoList;
	UndoList m_redoList;

	UndoArchivePtr m_beginStateArchivePtr;
};


} // namespace idoc


#endif // !idoc_CSerializedUndoManager_included


