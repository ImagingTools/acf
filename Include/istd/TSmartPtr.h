#ifndef istd_TSmartPtr_included
#define istd_TSmartPtr_included


#include "istd/TRetSmartPtr.h"


namespace istd
{


/**
	Implementation of a smart pointer.
	This implementation uses internal counter object. Internal counter object is used to
	count of instances of smart pointers and will be shared by all its copies.
*/
template <class Type, class Accessor = DefaultAccessor<Type> >
class TSmartPtr: public TRetSmartPtr<Type, Accessor>
{
public:
	typedef TRetSmartPtr<Type, Accessor> BaseClass;

	TSmartPtr();
	explicit TSmartPtr(Type* pointer);
	TSmartPtr(const TRetSmartPtr<Type, Accessor>& pointer);
	TSmartPtr(const TSmartPtr& pointer);
	~TSmartPtr();

	/**
		Set this pointer to NULL.
	*/
	void Reset();

	/**
		Set pointed object.
	*/
	void SetPtr(Type* pointer);

	// operators
	TSmartPtr& operator=(const TSmartPtr& otherCounter);

protected:
	using TRetSmartPtr<Type, Accessor>::m_counterPtr;

	/**
		Detach counter object without changing of internal counter pointer.
	*/
	void Detach();
};


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr()
{
	m_counterPtr = NULL;
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(Type* pointer)
{
	m_counterPtr = new typename TRetSmartPtr<Type, Accessor>::Counter(pointer);
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(const TRetSmartPtr<Type, Accessor>& pointer)
:	BaseClass(pointer)
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(const TSmartPtr& pointer)
:	BaseClass(pointer)
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::~TSmartPtr()
{
	Detach();
}


template <class Type, class Accessor>
void TSmartPtr<Type, Accessor>::Reset()
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnDetached();

		m_counterPtr = NULL;
	}
}


template <class Type, class Accessor>
inline void TSmartPtr<Type, Accessor>::SetPtr(Type* pointer)
{
	Detach();

	m_counterPtr = new typename TRetSmartPtr<Type, Accessor>::Counter(pointer);
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>& TSmartPtr<Type, Accessor>::operator=(const TSmartPtr& other)
{
	Detach();

	m_counterPtr = other.m_counterPtr;

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}

	return *this;
}


// protected methods

template <class Type, class Accessor>
void TSmartPtr<Type, Accessor>::Detach()
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnDetached();
	}
}


} // namespace istd


#endif // !istd_TSmartPtr_included


