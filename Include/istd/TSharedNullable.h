#pragma once


// STL includes
#include <algorithm>
#include <memory>

// ACF include
#include <istd/TNullable.h>


namespace istd
{


/**
	This is a comfort wrap for \c TNullable that allows several \c TSharedNullable objects to own the same object.
	In other words this is equivalent for \c std::shared_ptr<TNullable<T>>
*/
template <class T>
class TSharedNullable
{
public:
	TSharedNullable();
	TSharedNullable(const T& value);
	TSharedNullable(T&& value);
	~TSharedNullable();
	bool IsValid() const;
	bool IsNull() const;
	bool HasValue() const;
	const T* GetPtr() const;
	T* GetPtr();
	T& GetValue();
	const T& GetValue() const;
	void Reset();
	T& Emplace();
	void SetNull();

	bool isValid() const
	{
		return false;
	}
	bool isNull() const
	{
		return true;
	}

	// operators
	TSharedNullable& operator=(const T& value);
	TSharedNullable& operator=(T&& value);

	// STL-compatibility methods
	// access operators
	const T* operator->() const;
	T* operator->();
	const T& operator*() const &;
	T& operator*() &;
	const T&& operator*() const &&;
	T&& operator*() &&;

	// set operators
	TSharedNullable& operator=( std::nullptr_t );
	TSharedNullable& operator=(const TSharedNullable& other);

	explicit operator bool() const;
	bool has_value() const;

	// value access
	T& value() &;
	const T& value() const &;
	T&& value() &&;
	const T&& value() const &&;
	T value_or(const T& default_value) const;

	void swap(TSharedNullable& other );
	void reset();
	T& emplace();
	T& emplace(T&& value)&&;

	//Compare two TSharedNullable objects
	bool operator==(const TSharedNullable& other) const;
	bool operator!=(const TSharedNullable& other) const;

	//Compare a TSharedNullable object with a nullptr
	bool operator==(std::nullptr_t) const;
	bool operator!=(std::nullptr_t) const;

	//Compare a TSharedNullable object with a value
	bool operator==(const T& value) const;
	bool operator!=(const T& value) const;

private:
	std::shared_ptr<TNullable<T>> m_dataPtr;
};

template <class T>
bool Less(const istd::TSharedNullable<T> &lhs, const istd::TSharedNullable<T> &rhs)
{
	return *lhs < *rhs;
}


// public methods

template <class T>
TSharedNullable<T>::TSharedNullable()
	:m_dataPtr(new TNullable<T>())
{
}


template<class T>
inline TSharedNullable<T>::TSharedNullable(const T& value)
	:m_dataPtr(new TNullable<T>())
{
	*m_dataPtr = value;
}


template<class T>
inline TSharedNullable<T>::TSharedNullable(T&& value)
	:m_dataPtr(new TNullable<T>())
{
	*m_dataPtr = std::move(value);
}


template <class T>
TSharedNullable<T>::~TSharedNullable()
{
}


template<class T>
inline bool TSharedNullable<T>::IsValid() const
{
	return m_dataPtr->IsValid();
}


template<class T>
bool TSharedNullable<T>::IsNull() const
{
	return m_dataPtr->IsNull();
}


template<class T>
bool TSharedNullable<T>::HasValue() const
{
	return m_dataPtr->HasValue();
}


template<class T>
const T* TSharedNullable<T>::GetPtr() const
{
	return m_dataPtr->GetPtr();
}


template<class T>
T* TSharedNullable<T>::GetPtr()
{
	return m_dataPtr->GetPtr();
}


template<class T>
T& TSharedNullable<T>::GetValue()
{
	return m_dataPtr->GetValue();
}


template<class T>
const T& TSharedNullable<T>::GetValue() const
{
	return m_dataPtr->GetValue();
}


template<class T>
void TSharedNullable<T>::Reset()
{
	return m_dataPtr.reset(new TNullable<T>);
}


template<class T>
T& TSharedNullable<T>::Emplace()
{
	return m_dataPtr->Emplace();
}


template<class T>
inline void TSharedNullable<T>::SetNull()
{
	m_dataPtr->SetNull();
}


// operators

template<class T>
TSharedNullable<T>& TSharedNullable<T>::operator=(const T& value)
{
	m_dataPtr->operator=(value);

	return *this;
}


template<class T>
TSharedNullable<T>& TSharedNullable<T>::operator=(T&& value)
{
	m_dataPtr->operator=(std::move(value));

	return *this;
}


// STL-compatibility methods
// access operators

template<class T>
const T* TSharedNullable<T>::operator->() const
{
	return m_dataPtr->operator->();
}


template<class T>
T* TSharedNullable<T>::operator->()
{
	return m_dataPtr->operator->();
}


template<class T>
const T& TSharedNullable<T>::operator*() const &
{
	return m_dataPtr->operator*();
}


template<class T>
T& TSharedNullable<T>::operator*() &
{
	return m_dataPtr->operator*();
}


template<class T>
const T&& TSharedNullable<T>::operator*() const &&
{
	return m_dataPtr->operator*();
}


template<class T>
T&& TSharedNullable<T>::operator*() &&
{
	return m_dataPtr->operator*();
}


template<class T>
TSharedNullable<T>& TSharedNullable<T>::operator=( std::nullptr_t )
{
	m_dataPtr->operator=(nullptr);

	return *this;
}


template<class T>
TSharedNullable<T>& TSharedNullable<T>::operator=(const TSharedNullable& other)
{
	m_dataPtr = other.m_dataPtr;

	return *this;
}


template<class T>
TSharedNullable<T>::operator bool() const
{
	return m_dataPtr->operator bool();
}


template<class T>
bool TSharedNullable<T>::has_value() const
{
	return m_dataPtr->has_value();
}


template<class T>
T& TSharedNullable<T>::value() &
{
	return m_dataPtr->value();
}


template<class T>
const T& TSharedNullable<T>::value() const &
{
	return m_dataPtr->value();
}


template<class T>
T&& TSharedNullable<T>::value() &&
{
	return m_dataPtr->value();
}


template<class T>
const T&& TSharedNullable<T>::value() const &&
{
	return m_dataPtr->value();
}


template<class T>
T TSharedNullable<T>::value_or(const T& default_value) const
{
	return m_dataPtr->value_or(default_value);
}


template<class T>
void TSharedNullable<T>::swap(TSharedNullable& other )
{
	m_dataPtr.swap(other.m_dataPtr);
}


template<class T>
void TSharedNullable<T>::reset()
{
	Reset();
}


template<class T>
T& TSharedNullable<T>::emplace()
{
	return Emplace();
}


template<class T>
T& TSharedNullable<T>::emplace(T&& value)&&
{
	return m_dataPtr->emplace();
}


template<class T>
bool TSharedNullable<T>::operator==(const TSharedNullable& other) const
{
	return m_dataPtr.get() == other.m_dataPtr.get();
}


template<class T>
bool TSharedNullable<T>::operator!=(const TSharedNullable& other) const
{
	return (!operator==(other));
}


template<class T>
bool TSharedNullable<T>::operator==(std::nullptr_t) const
{
	return m_dataPtr->operator==(nullptr);
}


template<class T>
bool TSharedNullable<T>::operator!=(std::nullptr_t) const
{
	return m_dataPtr->operator!=(nullptr);
}


template<class T>
bool TSharedNullable<T>::operator==(const T& value) const
{
	return m_dataPtr->operator==(value);
}


template<class T>
bool TSharedNullable<T>::operator!=(const T& value) const
{
	return m_dataPtr->operator!=(value);
}


} // namespace istd


template <class T>
inline unsigned int qHash(const istd::TSharedNullable<T> &key, unsigned int seed){
	return qHash(*key, seed);
}

