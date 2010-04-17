#ifndef istd_TPointer_included
#define istd_TPointer_included


#include "istd/istd.h"


namespace istd
{


/**
	Implementation of pointer wrapper.
*/
template <class Type>
class TPointer
{
public:
	/**
		Construct and assign internal pointer.
	*/
	TPointer(Type* ptr = NULL);

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
	bool operator==(const TPointer<Type>& ptr) const;
	bool operator!=(const TPointer<Type>& ptr) const;
	bool operator<(const TPointer<Type>& ptr) const;
	bool operator>(const TPointer<Type>& ptr) const;
	bool operator<=(const TPointer<Type>& ptr) const;
	bool operator>=(const TPointer<Type>& ptr) const;
	bool operator==(const Type* ptr) const;
	bool operator!=(const Type* ptr) const;
	bool operator<(const Type* ptr) const;
	bool operator>(const Type* ptr) const;
	bool operator<=(const Type* ptr) const;
	bool operator>=(const Type* ptr) const;

protected:
	// blocked operators
	TPointer<Type>& operator=(const TPointer<Type> ptr);

	Type*& GetPtrRef();

private:
	Type* m_ptr;
};


// inline methods

template <class Type>
inline void TPointer<Type>::Reset()
{
	m_ptr = NULL;
}


template <class Type>
inline Type* TPointer<Type>::GetPtr() const
{
	return m_ptr;
}


template <class Type>
inline bool TPointer<Type>::IsValid() const
{
	return (m_ptr != NULL);
}


template <class Type>
inline Type& TPointer<Type>::operator*() const
{
	I_ASSERT(m_ptr != NULL);
	
	return *m_ptr;
}


template <class Type>
inline Type* TPointer<Type>::operator->() const
{
	return m_ptr;
}


template <class Type>
inline bool TPointer<Type>::operator==(const TPointer<Type>& ptr) const
{
	return (m_ptr == ptr.m_ptr);
}


template <class Type>
inline bool TPointer<Type>::operator!=(const TPointer<Type>& ptr) const
{
	return (m_ptr != ptr.m_ptr);
}


template <class Type>
inline bool TPointer<Type>::operator<(const TPointer<Type>& ptr) const
{
	return m_ptr < ptr.m_ptr;
}


template <class Type>
inline bool TPointer<Type>::operator>(const TPointer<Type>& ptr) const
{
	return m_ptr > ptr.m_ptr;
}


template <class Type>
inline bool TPointer<Type>::operator<=(const TPointer<Type>& ptr) const
{
	return m_ptr <= ptr.m_ptr;
}


template <class Type>
inline bool TPointer<Type>::operator>=(const TPointer<Type>& ptr) const
{
	return m_ptr >= ptr.m_ptr;
}


template <class Type>
inline bool TPointer<Type>::operator==(const Type* ptr) const
{
	return (m_ptr == ptr);
}


template <class Type>
inline bool TPointer<Type>::operator!=(const Type* ptr) const
{
	return (m_ptr != ptr);
}


template <class Type>
bool TPointer<Type>::operator<(const Type* ptr) const
{
	return (m_ptr < ptr);
}


template <class Type>
bool TPointer<Type>::operator>(const Type* ptr) const
{
	return (m_ptr > ptr);
}


template <class Type>
bool TPointer<Type>::operator<=(const Type* ptr) const
{
	return (m_ptr <= ptr);
}


template <class Type>
bool TPointer<Type>::operator>=(const Type* ptr) const
{
	return (m_ptr >= ptr);
}


// protected inline methods

template <class Type>
inline TPointer<Type>::TPointer(Type* ptr)
{
	m_ptr = ptr;
}


template <class Type>
inline void TPointer<Type>::SetPtr(Type* ptr)
{
	m_ptr = ptr;
}


template <class Type>
inline TPointer<Type>& TPointer<Type>::operator=(const TPointer<Type> ptr)
{
	m_ptr = ptr.m_ptr;
	
	return *this;
}


template <class Type>
inline Type*& TPointer<Type>::GetPtrRef()
{
	return m_ptr;
}


} // namespace istd


#endif // !istd_TPointer_included


