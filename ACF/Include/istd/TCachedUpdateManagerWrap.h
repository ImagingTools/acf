#ifndef istd_TCachedUpdateManagerWrap_included
#define istd_TCachedUpdateManagerWrap_included


#include "istd/TUpdateManagerWrap.h"


namespace istd
{


/**
	Help wrapper class supporting of cached parts.
	If nested update is started, base class implementation of BeginChanges and EndChanges will be blocked.
*/
template <class Base>
class TCachedUpdateManagerWrap: public TUpdateManagerWrap<Base>
{
public:
	typedef TUpdateManagerWrap<Base> BaseClass;

	TCachedUpdateManagerWrap();

	// reimplemented (istd::IChangeable)
	virtual void EndChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);

protected:
	/**
		Ensures cache is valid.
		If cache is not valid method CalculateCache will be called to update it.
	*/
	bool EnsureCacheValid() const;
	/**
		Make cache invalid.
		This is automatically done when model/observer mechanism is used.
	*/
	void InvalidateCache(int changeFlags = 0);
	/**
		Calculate cache.
		It will be called, if cache was not valid and EnsureCacheValid() is called.
		Default implementation does nothing and is provided to ensure
		that base class for all derived classes implement this method.
	*/
	virtual bool CalculateCache(int changeFlags);

private:
	mutable bool m_isCacheValid;
	mutable int m_cumulatedFlags;
};


// protected inline methods

template <class Base>
inline bool TCachedUpdateManagerWrap<Base>::EnsureCacheValid() const
{
	if (!m_isCacheValid){
		m_isCacheValid = true;	// set to avoid recursion
		m_isCacheValid = const_cast<TCachedUpdateManagerWrap<Base>*>(this)->CalculateCache(m_cumulatedFlags);
		m_cumulatedFlags = 0;
	}

	return m_isCacheValid;
}


template <class Base>
void TCachedUpdateManagerWrap<Base>::InvalidateCache(int changeFlags)
{
	m_cumulatedFlags |= changeFlags;
	m_isCacheValid = false;
}


template <class Base>
inline bool TCachedUpdateManagerWrap<Base>::CalculateCache(int /*changeFlags*/)
{
	return true;
}


// public methods

template <class Base>
TCachedUpdateManagerWrap<Base>::TCachedUpdateManagerWrap()
:	m_isCacheValid(false), m_cumulatedFlags(0)
{
}


// reimplemented (istd::IChangeable)

template <class Base>
void TCachedUpdateManagerWrap<Base>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	InvalidateCache(changeFlags);

	BaseClass::EndChanges(changeFlags, changeParamsPtr);
}


} // namespace istd


#endif // !istd_TCachedUpdateManagerWrap_included


