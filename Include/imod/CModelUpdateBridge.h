#ifndef imod_CModelUpdateBridge_included
#define imod_CModelUpdateBridge_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>


namespace imod
{


/**
	Reflects the changes of observed objects as changes of some other object (over istd::IChangeable), typically delegated to itself.
	In other words: it allows to connect as observer to multiple models and call \c BeginChanges and \c EndChanges on some its changes.
	\sa imod::CMultiModelDispatcherBase.

	\ingroup ModelObserver

*/
class CModelUpdateBridge: virtual public IObserver
{
public:
	/**
		Flags to control the change set using for the change notification of the target object.
	*/
	enum UpdateFlags
	{
		/**
			Change notification of the target object will be done using change set containing CF_DELEGATED flag only.
		*/
		UF_DELEGATED = 0x1,

		/**
			Change notification of the target object will be done using change set of observered object.
		*/
		UF_SOURCE = 0x2
	};

	explicit CModelUpdateBridge(istd::IChangeable* changeablePtr, int updateFlags = UF_DELEGATED);

	virtual ~CModelUpdateBridge();

	/**
		Get access to connected model with the index \c index.
	*/
	imod::IModel* GetObservedModel(int modelIndex) const;

	/**
		Gets the number of connected models.	
	*/
	int GetModelCount() const;

	/**
		Remove all observed models from this bridge.
	*/
	void EnsureModelsDetached();

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const imod::IModel* modelPtr) const;
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

private:
	typedef QVector<imod::IModel*> Models;
	Models m_models;

	istd::IChangeable* m_changeablePtr;

	int m_updateFlags;
};


} // namespace imod


#endif // !imod_CModelUpdateBridge_included

