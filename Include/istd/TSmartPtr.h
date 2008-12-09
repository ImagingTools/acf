#ifndef istd_TSmartPtr_included
#define istd_TSmartPtr_included


#include "istd/istd.h"


namespace istd
{


/**
	Implementation of a smart pointer.
*/
template <class Type>
class TSmartPtr
{
public:
	TSmartPtr();
	explicit TSmartPtr(Type* pointer);
	TSmartPtr(const TSmartPtr& other);

	virtual ~TSmartPtr();

	void SetPtr(Type* pointer);
	const Type* GetPtr() const;
	Type* GetPtr();
	const Type* operator->() const;

	/**
		Get access to object pointed at.
	*/
	Type& operator*() const;
	
	/**
		Get access to pointer instance.
	*/
	Type* operator->();

	/**
		Assignment operator.
	*/
	TSmartPtr& operator = (const TSmartPtr& otherCounter);

	/**
		Checks, whether the object is in valid state.
	*/
	bool IsValid() const;

protected:
	void Detach();

protected:
	class Counter
	{
	public:
		Counter(Type* ptr)
		{
			objectPtr = ptr;
			count = 0;
		}

		~Counter()
		{
			if (objectPtr != NULL && count == 0){
				delete objectPtr;
			}
		}

		mutable int count;
		Type* objectPtr;
	};

	Counter* m_counter;
};


template <class Type>
TSmartPtr<Type>::TSmartPtr()
{
	m_counter = new Counter(NULL);
}


template <class Type>
TSmartPtr<Type>::TSmartPtr(Type* pointer)
{
	m_counter = NULL;

	SetPtr(pointer);
}


template <class Type>
TSmartPtr<Type>::TSmartPtr(const TSmartPtr<Type>& other)
{
	m_counter = other.m_counter;
	m_counter->count++;
}


template <class Type>
TSmartPtr<Type>::~TSmartPtr()
{
	Detach();
}


template <class Type>
inline void TSmartPtr<Type>::SetPtr(Type* pointer)
{
	Detach();

	m_counter = new Counter(pointer);
}


template <class Type>
inline const Type* TSmartPtr<Type>::GetPtr() const
{
	I_ASSERT(m_counter != NULL);

	return m_counter->objectPtr;
}


template <class Type>
inline Type* TSmartPtr<Type>::GetPtr()
{
	I_ASSERT(m_counter != NULL);

	return m_counter->objectPtr;
}


template <class Type>
inline const Type* TSmartPtr<Type>::operator->() const
{
	I_ASSERT(m_counter != NULL);
	I_ASSERT(m_counter->objectPtr != NULL);

	return GetPtr();
}


template <class Type>
inline Type* TSmartPtr<Type>::operator->()
{
	I_ASSERT(m_counter != NULL);
	I_ASSERT(m_counter->objectPtr != NULL);

	return GetPtr();
}


template <class Type>
TSmartPtr<Type>& TSmartPtr<Type>::operator=(const TSmartPtr<Type>& other)
{
	Detach();

	if (other.IsValid()){
		other.m_counter->count++;

		m_counter = other.m_counter;
	}

	return *this;
}


template <class Type>
bool TSmartPtr<Type>::IsValid() const
{
	if (m_counter != NULL){
		return (m_counter->objectPtr != NULL);
	}

	return false;
}


template <class Type>
void TSmartPtr<Type>::Detach()
{
	if (m_counter != NULL){
		if(m_counter->count == 0){
			delete m_counter;

			m_counter = NULL;
		}
		else{
			m_counter->count--;

			I_ASSERT(m_counter->count >= 0);
		}
	}
}


} // namespace istd


#endif // !istd_TSmartPtr_included

