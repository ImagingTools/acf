#ifndef istd_CChangeDelegator_included
#define istd_CChangeDelegator_included


// ACF includes
#include <istd/IChangeDelegator.h>


namespace istd
{


/**
	Delegates calls of IChangeable methods to the given slave.

	\ingroup DataModel
*/
class CChangeDelegator: virtual public IChangeDelegator 
{
public:
	typedef istd::IChangeable BaseClass;

	CChangeDelegator();
	explicit CChangeDelegator(istd::IChangeable* slavePtr);

	// reimplemented (istd::IChangeDelegator)
	virtual void SetSlavePtr(istd::IChangeable* slavePtr);
	virtual istd::IChangeable* GetSlavePtr() const;

	// reimplemented (istd::IChangeable)
	virtual void OnBeginChanges();
	virtual void OnEndChanges(const ChangeSet& changeSet);
	
private:
	istd::IChangeable* m_slavePtr;
};


// inline methods

inline void CChangeDelegator::SetSlavePtr(istd::IChangeable* slavePtr)
{
	m_slavePtr = slavePtr;
}


inline istd::IChangeable* CChangeDelegator::GetSlavePtr() const
{
	return m_slavePtr;
}


} // namespace istd


#endif // !istd_CChangeDelegator_included

