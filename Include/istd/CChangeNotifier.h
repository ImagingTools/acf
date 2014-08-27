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
		\param	changeablePtr	Your object prepared to change. It cannot be \c NULL.
		\param	changeSet		Set of change flags (its IDs).
	*/
	explicit CChangeNotifier(IChangeable* changeablePtr, const IChangeable::ChangeSet& changeSet = IChangeable::GetAnyChange());
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
		Similiar like method Reset(), but it tries to avoid the update.
		It should be used only if you are sure, that no changes was done, or result will be ignored.
	*/
	void Abort();

private:
	// blocked copy constructor
	CChangeNotifier(const CChangeNotifier& notifier);

	IChangeable* m_changeablePtr;
	IChangeable::ChangeSet m_changeIds;
};


inline CChangeNotifier::CChangeNotifier(IChangeable* changeablePtr, const IChangeable::ChangeSet& changeSet)
:	m_changeablePtr(changeablePtr),
	m_changeIds(changeSet)
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->BeginChanges(changeSet);
	}
}


inline CChangeNotifier::~CChangeNotifier()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeIds);
	}
}


} // namespace istd


#endif // !istd_CChangeNotifier_included


