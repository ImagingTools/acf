#ifndef istd_CEventBasedNotifier_included
#define istd_CEventBasedNotifier_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TOptDelPtr.h>


namespace istd
{


class CAssyncNotifier;


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

	CAssyncNotifier* m_assyncNotifierPtr;
};


/**
	\internal

	Help class to realize the post-processing of \c istd::IChangeable::EndChanges
*/
class CAssyncNotifier: protected QObject
{
	Q_OBJECT

	friend class CEventBasedNotifier;

public:
	~CAssyncNotifier();

protected:
	CAssyncNotifier(istd::IChangeable* slavePtr, const IChangeable::ChangeSet& changeSet);

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


