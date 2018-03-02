#ifndef istd_CEventBasedNotifier_included
#define istd_CEventBasedNotifier_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TOptDelPtr.h>


namespace istd
{


class CAsyncNotifier;


/**
	Implementation of model changes notification between different threads.

	\ingroup DataModel
*/
class CEventBasedNotifier
{
public:
	explicit CEventBasedNotifier(istd::IChangeable* slavePtr, const IChangeable::ChangeSet* changeSetPtr = &IChangeable::GetAnyChange());
	virtual ~CEventBasedNotifier();

private:
	CEventBasedNotifier(const CEventBasedNotifier&){}

	CAsyncNotifier* m_asyncNotifierPtr;
};


/**
	\internal

	Help class to realize the post-processing of \c istd::IChangeable::EndChanges
*/
class CAsyncNotifier: protected QObject
{
	Q_OBJECT

	friend class CEventBasedNotifier;

public:
	~CAsyncNotifier();

protected:
	CAsyncNotifier(istd::IChangeable* slavePtr, const IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void DoBeginChanges();

Q_SIGNALS:
	void EmitBeginChanges();

private:
	/**
		Target object should be updated. Cannot be \c NULL.
	*/
	istd::IChangeable* m_slavePtr;

	/**
		Set of changes.
	*/
	IChangeable::ChangeSet m_changeIds;

	/**
		Inidicates that \c BeginChange was called on slave object.
	*/
	bool m_isBeginCalled;
};


} // namespace istd


#endif // !istd_CEventBasedNotifier_included


