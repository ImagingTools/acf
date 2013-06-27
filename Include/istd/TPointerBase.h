#ifndef istd_TPointerBase_included
#define istd_TPointerBase_included


#include "istd/istd.h"


namespace istd
{


/**
	\internal
	Internal data accessor using standard memory management with new and delete.
*/
template <class Type>
class DefaultAccessor
{
public:
	static void Delete(Type* ptr)
	{
		Q_ASSERT(ptr != NULL);
		if (ptr != NULL){
			delete ptr;
		}
	}
};


/**
	\internal
	Internal data accessor using standard memory management with new and delete for arrays.
*/
template <class Type>
class ArrayAccessor
{
public:
	static void Delete(Type* ptr)
	{
		Q_ASSERT(ptr != NULL);
		if (ptr != NULL){
			delete[] ptr;
		}
	}
};


/**
	Implementation of pointer wrapper.
*/
template <class Type>
class TPointerBase
{
public:
	/**
		Construct and assign internal pointer.
	*/
	TPointerBase(Type* ptr = NULL);

	/**
		Set value of internal stored pointer.
	 */
	void SetPtr(Type* ptr);

	/**
		Set internal pointer value to NULL.
	*/
	void Reset();

	/**
		Return access to internal stored pointer.
	*/
	Type* GetPtr() const;
	/**
		Check if internal pointer not NULL.
		\return	true, if internal pointer not NULL and can be used to access pointed members.
	*/
	bool IsValid() const;

	/**
		Swap two pointers.
	*/
	void Swap(TPointerBase& ptr);

	/**
		Get an access to object pointed at.
	*/
	Type& operator*() const;

	template <class CastedType>
	CastedType Cast() const
	{
		return dynamic_cast<CastedType>(GetPtr());
	}

	// operators
	Type* operator->() const;
	bool operator==(const TPointerBase<Type>& ptr) const;
	bool operator!=(const TPointerBase<Type>& ptr) const;
	bool operator<(const TPointerBase<Type>& ptr) const;
	bool operator>(const TPointerBase<Type>& ptr) const;
	bool operator<=(const TPointerBase<Type>& ptr) const;
	bool operator>=(const TPointerBase<Type>& ptr) const;
	bool operator==(const Type* ptr) const;
	bool operator!=(const Type* ptr) const;
	bool operator<(const Type* ptr) const;
	bool operator>(const Type* ptr) const;
	bool operator<=(const Type* ptr) const;
	bool operator>=(const Type* ptr) const;

protected:
	// blocked operators
	TPointerBase<Type>& operator=(const TPointerBase<Type> ptr);

	Type*& GetPtrRef();

private:
	Type* m_ptr;
};


// inline methods

template <class Type>
inline void TPointerBase<Type>::Reset()
{
	m_ptr = NULL;
}


template <class Type>
inline Type* TPointerBase<Type>::GetPtr() const
{
	return m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::IsValid() const
{
	return (m_ptr != NULL);
}


template <class Type>
void TPointerBase<Type>::Swap(TPointerBase& ptr)
{
	qSwap(m_ptr, ptr.m_ptr);
}


template <class Type>
inline Type& TPointerBase<Type>::operator*() const
{
	Q_ASSERT(m_ptr != NULL);
	
	return *m_ptr;
}


template <class Type>
inline Type* TPointerBase<Type>::operator->() const
{
	return m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::operator==(const TPointerBase<Type>& ptr) const
{
	return (m_ptr == ptr.m_ptr);
}


template <class Type>
inline bool TPointerBase<Type>::operator!=(const TPointerBase<Type>& ptr) const
{
	return (m_ptr != ptr.m_ptr);
}


template <class Type>
inline bool TPointerBase<Type>::operator<(const TPointerBase<Type>& ptr) const
{
	return m_ptr < ptr.m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::operator>(const TPointerBase<Type>& ptr) const
{
	return m_ptr > ptr.m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::operator<=(const TPointerBase<Type>& ptr) const
{
	return m_ptr <= ptr.m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::operator>=(const TPointerBase<Type>& ptr) const
{
	return m_ptr >= ptr.m_ptr;
}


template <class Type>
inline bool TPointerBase<Type>::operator==(const Type* ptr) const
{
	return (m_ptr == ptr);
}


template <class Type>
inline bool TPointerBase<Type>::operator!=(const Type* ptr) const
{
	return (m_ptr != ptr);
}


template <class Type>
bool TPointerBase<Type>::operator<(const Type* ptr) const
{
	return (m_ptr < ptr);
}


template <class Type>
bool TPointerBase<Type>::operator>(const Type* ptr) const
{
	return (m_ptr > ptr);
}


template <class Type>
bool TPointerBase<Type>::operator<=(const Type* ptr) const
{
	return (m_ptr <= ptr);
}


template <class Type>
bool TPointerBase<Type>::operator>=(const Type* ptr) const
{
	return (m_ptr >= ptr);
}


// protected inline methods

template <class Type>
inline TPointerBase<Type>::TPointerBase(Type* ptr)
{
	m_ptr = ptr;
}


template <class Type>
inline void TPointerBase<Type>::SetPtr(Type* ptr)
{
	m_ptr = ptr;
}


template <class Type>
inline TPointerBase<Type>& TPointerBase<Type>::operator=(const TPointerBase<Type> ptr)
{
	m_ptr = ptr.m_ptr;
	
	return *this;
}


template <class Type>
inline Type*& TPointerBase<Type>::GetPtrRef()
{
	return m_ptr;
}


} // namespace istd


#endif // !istd_TPointerBase_included


