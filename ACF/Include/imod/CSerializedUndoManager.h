#ifndef imod_CSerializedUndoManager_included
#define imod_CSerializedUndoManager_included


#include <list>

#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"
#include "iser/CMemoryWriteArchive.h"

#include "imod/IUndoManager.h"
#include "imod/TSingleModelObserverBase.h"


namespace imod
{


/**
	Implements multi level UNDO mechanism based on storing complete objects state at each step using serialization.
	To trace if UNDO is available you can use ACF model/observer.
*/
class CSerializedUndoManager: public TSingleModelObserverBase<iser::ISerializable>, virtual public IUndoManager
{
public:
	enum UpdateFlags{
		UF_UNDO_MANAGER = 0x400
	};

	/**
		Default constructor with optional model pointer to set managed model.
		You can also use standard connection to observed model.
	*/
	CSerializedUndoManager(imod::IModel* persistentModelPtr = NULL);

	// reimplemented (imod::IUndoManager)
	virtual bool IsUndoAvailable() const;
	virtual bool IsRedoAvailable() const;
	virtual void ResetUndo();
	virtual bool DoUndo();
	virtual bool DoRedo();

protected:
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

private:
	typedef istd::TDelPtr<iser::CMemoryWriteArchive> UndoArchivePtr;
	typedef std::list<UndoArchivePtr> UndoList;
	UndoList m_undoList;
	UndoList m_redoList;

	UndoArchivePtr m_beginStateArchivePtr;
};


} // namespace imod


#endif // !imod_CSerializedUndoManager_included


