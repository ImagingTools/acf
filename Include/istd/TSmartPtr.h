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
template <class Type>
class TSmartPtr: public TRetSmartPtr<Type>
{
public:
	typedef TRetSmartPtr<Type> BaseClass;

	TSmartPtr();
	TSmartPtr(const TRetSmartPtr<Type>& pointer);
	explicit TSmartPtr(Type* pointer);
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
	/**
		Detach counter object without changing of internal counter pointer.
	*/
	void Detach();
};


template <class Type>
TSmartPtr<Type>::TSmartPtr()
{
	m_counterPtr = NULL;
}


template <class Type>
TSmartPtr<Type>::TSmartPtr(const TRetSmartPtr<Type>& pointer)
:	BaseClass(pointer)
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}
}


template <class Type>
TSmartPtr<Type>::TSmartPtr(Type* pointer)
{
	m_counterPtr = new Counter(pointer);
}


template <class Type>
TSmartPtr<Type>::~TSmartPtr()
{
	Detach();
}


template <class Type>
void TSmartPtr<Type>::Reset()
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnDetached();

		m_counterPtr = NULL;
	}
}


template <class Type>
inline void TSmartPtr<Type>::SetPtr(Type* pointer)
{
	Detach();

	m_counterPtr = new Counter(pointer);
}


template <class Type>
TSmartPtr<Type>& TSmartPtr<Type>::operator=(const TSmartPtr<Type>& other)
{
	Detach();

	m_counterPtr = other.m_counterPtr;

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}

	return *this;
}


// protected methods

template <class Type>
void TSmartPtr<Type>::Detach()
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnDetached();
	}
}


} // namespace istd


#endif // !istd_TSmartPtr_included


