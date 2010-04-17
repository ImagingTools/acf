#ifndef istd_TRetSmartPtr_included
#define istd_TRetSmartPtr_included


#include "istd/istd.h"


namespace istd
{


/**
	\internal
	Internal data accessor.
*/
template <class Type>
class DefaultAccessor
{
public:
	static void Delete(Type* ptr)
	{
		I_ASSERT(ptr != NULL);
		if (ptr != NULL){
			delete ptr;
		}
	}
};


/**
	Implementation of reduced a smart pointer used as return parameters.
	This is a part of smart pointer concept.
	\sa istd::TSmartPtr
*/
template <class Type, class AccessAdapter = DefaultAccessor<Type> >
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

	template <class CastedType>
	CastedType Cast() const
	{
		return dynamic_cast<CastedType>(GetPtr());
	}

	// operators
	Type& operator*() const;
	Type* operator->() const;

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
				AccessAdapter::Delete(m_objectPtr);

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

template <class Type, class AccessAdapter>
TRetSmartPtr<Type, AccessAdapter>::TRetSmartPtr(const TRetSmartPtr& other)
:	m_counterPtr(other.m_counterPtr)
{
}


template <class Type, class AccessAdapter>
bool TRetSmartPtr<Type, AccessAdapter>::IsValid() const
{
	return (m_counterPtr != NULL) && (m_counterPtr->GetPtr() != NULL);
}


template <class Type, class AccessAdapter>
inline const Type* TRetSmartPtr<Type, AccessAdapter>::GetPtr() const
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type, class AccessAdapter>
inline Type* TRetSmartPtr<Type, AccessAdapter>::GetPtr()
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type, class AccessAdapter>
inline Type& TRetSmartPtr<Type, AccessAdapter>::operator*() const
{
	I_ASSERT(m_counterPtr != NULL);
	I_ASSERT(m_counterPtr->GetPtr() != NULL);

	return *m_counterPtr->GetPtr();
}


template <class Type, class AccessAdapter>
inline Type* TRetSmartPtr<Type, AccessAdapter>::operator->() const
{
	I_ASSERT(m_counterPtr != NULL);
	I_ASSERT(m_counterPtr->GetPtr() != NULL);

	return m_counterPtr->GetPtr();
}


// protected methods

template <class Type, class AccessAdapter>
TRetSmartPtr<Type, AccessAdapter>::TRetSmartPtr()
{
}


} // namespace istd


#endif // !istd_TRetSmartPtr_included


