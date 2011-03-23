#ifndef istd_CChangeDelegator_included
#define istd_CChangeDelegator_included


#include "istd/IChangeable.h"


namespace istd
{


/**
	Delegates calls of IChangeable methods to the given slave.
*/
class CChangeDelegator: virtual public istd::IChangeable
{
public:
	typedef istd::IChangeable BaseClass;

	CChangeDelegator();
	explicit CChangeDelegator(istd::IChangeable* slavePtr);

	/**
		Sets the slave object pointer. 
		Its IChangeable methods will be called, if this object is updated.
	*/
	virtual void SetSlavePtr(istd::IChangeable* slavePtr);

	/**
		Gets the slave object pointer. 
	*/
	virtual istd::IChangeable* GetSlavePtr() const;

	// reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
	
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

