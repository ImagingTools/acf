#ifndef istd_TRetSmartPtr_included
#define istd_TRetSmartPtr_included


#include "istd/istd.h"


namespace istd
{


/**
	Implementation of reduced a smart pointer used as return parameters.
	This is a part of smart pointer concept.
	\sa istd::TSmartPtr
*/
template <class Type>
class TRetSmartPtr
{
public:
	TRetSmartPtr(const TRetSmartPtr& other);

	/**
		Check, whether the object is in valid state.
	*/
	bool IsValid() const;

	/**
		Get access to pointed object.
	*/
	const Type* GetPtr() const;

	/**
		Get access to pointed object.
	*/
	Type* GetPtr();

	// operators
	Type& operator*() const;
	const Type* operator->() const;
	Type* operator->();

protected:
	TRetSmartPtr();

	class Counter
	{
	public:
		Counter(Type* ptr)
		{
			I_ASSERT(ptr != NULL);

			m_objectPtr = ptr;
			m_count = 1;
		}

		void OnAttached()
		{
			I_ASSERT(m_objectPtr != NULL);
			I_ASSERT(m_count > 0);

			++m_count;
		}

		void OnDetached()
		{
			I_ASSERT(m_objectPtr != NULL);
			I_ASSERT(m_count > 0);

			if (--m_count <= 0){
				delete m_objectPtr;

				delete this;
			}
		}

		Type* GetPtr()
		{
			return m_objectPtr;
		}

	private:
		int m_count;
		Type* m_objectPtr;
	};

	Counter* m_counterPtr;
};


// public methods

template <class Type>
TRetSmartPtr<Type>::TRetSmartPtr(const TRetSmartPtr<Type>& other)
:	m_counterPtr(other.m_counterPtr)
{
}


template <class Type>
bool TRetSmartPtr<Type>::IsValid() const
{
	return (m_counterPtr != NULL) && (m_counterPtr->m_objectPtr != NULL);
}


template <class Type>
inline const Type* TRetSmartPtr<Type>::GetPtr() const
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type>
inline Type* TRetSmartPtr<Type>::GetPtr()
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type>
inline const Type* TRetSmartPtr<Type>::operator->() const
{
	I_ASSERT(m_counterPtr != NULL);
	I_ASSERT(m_counterPtr->m_objectPtr != NULL);

	return m_counterPtr->m_objectPtr;
}


template <class Type>
inline Type* TRetSmartPtr<Type>::operator->()
{
	I_ASSERT(m_counterPtr != NULL);
	I_ASSERT(m_counterPtr->m_objectPtr != NULL);

	return m_counterPtr->m_objectPtr;
}


// protected methods

template <class Type>
TRetSmartPtr<Type>::TRetSmartPtr()
{
}


} // namespace istd


#endif // !istd_TRetSmartPtr_included


