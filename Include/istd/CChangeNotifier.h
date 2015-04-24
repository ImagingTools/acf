#ifndef istd_CChangeNotifier_included
#define istd_CChangeNotifier_included


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


/**
	Help class which provides the automatic update mechanism of the model.
	\note It should not be overriden.

	\ingroup ModelObserver
	\ingroup Main
	\ingroup DataModel
*/
class CChangeNotifier
{
public:
	/**
		Constructs the notifier.
		\param	changeablePtr	Your object prepared to change. If it is \c NULL than this notifier has no function.
		\param	changeSetPtr	Pointer to set of change flags (its IDs).
								It cannot be \c NULL.
	*/
	explicit CChangeNotifier(IChangeable* changeablePtr, const IChangeable::ChangeSet* changeSetPtr = &IChangeable::GetAnyChange());
	~CChangeNotifier();

	/**
		Check if this pointer is valid.
		It will be invalid after \c Reset() and \c Abort() only.
	*/
	bool IsValid() const;

	/**
		Call update on attached object and detach it.
	*/
	void Reset();
	/**
		Similar like method Reset(), but it tries to avoid the update.
		It should be used only if you are sure, that no changes was done, or result will be ignored.
	*/
	void Abort();

private:
	// blocked copy constructor
	CChangeNotifier(const CChangeNotifier& notifier);

	IChangeable* m_changeablePtr;
	const IChangeable::ChangeSet& m_changeSet;
};


inline CChangeNotifier::CChangeNotifier(IChangeable* changeablePtr, const IChangeable::ChangeSet* changeSetPtr)
:	m_changeablePtr(changeablePtr),
	m_changeSet(*changeSetPtr)
{
	Q_ASSERT(changeSetPtr != NULL);

	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChanges(m_changeSet);
	}
}


inline CChangeNotifier::~CChangeNotifier()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeSet);
	}
}


} // namespace istd


#endif // !istd_CChangeNotifier_included


