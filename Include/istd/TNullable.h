#ifndef istd_TNullable_included
#define istd_TNullable_included


// STL includes
#include <algorithm>


namespace istd
{


/**
	\brief This class is designed to represent three possible states of a given element: "Value set", "Value set to NULL", and "Value not set". In other words, it can be described as a pointer to a pointer.
	\example "Value set" is when the user explicitly sets the value for. In other words, the pointer points to another pointer with a specific value.
	\example "Value set to NULL" is when the variable is initialized, it has not been assigned a value. (JS analog value === null). In other words, the pointer points a null pointer.
	\example "Value not set" is when the variable has not been initialized. (JS analog value === undefined). In other words, the pointer is null
*/
template <class T>
class TNullable
{
public:
	TNullable();
	TNullable(const T& value);
	TNullable(T&& value);
	~TNullable();

	/**
		\brief Checks whether the container has been initialized with a value, even if it is a NULL value.
		\returns true if a value has been initialized or a NULL value was set and false otherwise
		\code{.cpp} IsNull() || HasValue() \endcode
		\sa IsNull
		\sa HasValue
	*/
	bool IsValid() const;

	/**
		\brief Checks whether the container has been explicitly initialized with a NULL value.
		\returns true if a NULL value was set, and false if a non-NULL value was set or if no value was set.
		\sa SetNull
		\code{.cpp}
			TNullable<int> value;
			value.IsNull(); // false
			value = 50;
			value.IsNull(); // false
			value.SetNull();
			value.IsNull(); // true
		\endcode
	*/
	bool IsNull() const;

	/**
		\brief Checks whether the container has been initialized with a specific value.
		\returns true if a non-NULL value was set, and false if a null value was set or if no value was set.
		\code{.cpp}
			TNullable<int> value;
			value.HasValue(); // false
			value = 50;
			value.HasValue(); // true
			value.SetNull();
			value.HasValue(); // false
		\endcode
	*/
	bool HasValue() const;

	/**
		\brief access the pointer of value
		\returns a pointer to a stored value
	*/
	const T* GetPtr() const;
	T* GetPtr();

	/**
		\brief access the value
		\returns a reference to a value
		\warning before call this method you MUST ensure, the container was initialized with a non-NULL value! Call \c HasValue() before.
	*/
	T& GetValue();
	const T& GetValue() const;

	/**
		\brief Resets a container and destroys a value
	*/
	void Reset();

	/**
		\brief Initializes a container with a default value
		\returns a reference to a created value
		\note for primitive types don't forget to initialize with a value
		\code{.cpp} 
			TNullable<int> value;
			value.Emplace() = 50; // initialize
		\endcode
	*/
	T& Emplace();

	/**
		\brief Destroys a containing value (if was set), and sets state of a container to NULL
		\code{.cpp}
			TNullable<int> value;
			value.HasValue(); // false
			value.IsNull();   // false
			value.IsValid();  // false

			value = 50;
			value.HasValue(); // false
			value.IsNull();   // false
			value.IsValid();  // true

			value.SetNull();
			value.HasValue(); // false
			value.IsNull();   // true
			value.IsValid();  // true
		\endcode
	*/
	void SetNull();

	// operators
	TNullable& operator=(const T& value);
	TNullable& operator=(T&& value);

	// STL-compatibility methods
	// access operators
	const T* operator->() const;
	T* operator->();
	const T& operator*() const &;
	T& operator*() &;
	const T&& operator*() const &&;
	T&& operator*() &&;

	// set operators
	TNullable& operator=( std::nullptr_t );
	TNullable& operator=(const TNullable& other);

	explicit operator bool() const;
	bool has_value() const;

	// value access
	T& value() &;
	const T& value() const &;
	T&& value() &&;
	const T&& value() const &&;
	T value_or(const T& default_value) const;

	void swap(TNullable& other );
	void reset();
	T& emplace();
	T& emplace(T&& value)&&;

	//Compare two TNullable objects
	bool operator==(const TNullable& other) const;
	bool operator!=(const TNullable& other) const;

	//Compare a TNullable object with a nullptr
	bool operator==(std::nullptr_t) const;
	bool operator!=(std::nullptr_t) const;

	//Compare a TNullable object with a value
	bool operator==(const T& value) const;
	bool operator!=(const T& value) const;

private:
	bool m_hasValue;
	bool m_isNull;
	bool m_isValid;
	T* m_dataPtr;
};


// public methods

template <class T>
TNullable<T>::TNullable()
	:m_hasValue(false),
	m_isNull(false),
	m_isValid(false),
	m_dataPtr(nullptr)
{
}


template<class T>
inline TNullable<T>::TNullable(const T& value)
	:m_hasValue(true),
	m_isNull(false),
	m_isValid(true)
{
	m_dataPtr = new T;
	*m_dataPtr = value;
}


template<class T>
inline TNullable<T>::TNullable(T&& value)
	:m_hasValue(true),
	m_isNull(false),
	m_isValid(true)
{
	m_dataPtr = new T;
	*m_dataPtr = std::move(value);
}


template <class T>
TNullable<T>::~TNullable()
{
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
	}
}


template<class T>
inline bool TNullable<T>::IsValid() const
{
	return m_isValid;
}


template<class T>
bool TNullable<T>::IsNull() const
{
	return m_isNull;
}


template<class T>
bool TNullable<T>::HasValue() const
{
	return m_hasValue;
}


template<class T>
const T* TNullable<T>::GetPtr() const
{
	return m_dataPtr;
}


template<class T>
T* TNullable<T>::GetPtr()
{
	return m_dataPtr;
}


template<class T>
T& TNullable<T>::GetValue()
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}


template<class T>
const T& TNullable<T>::GetValue() const
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}

template<class T>
void TNullable<T>::Reset()
{
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
	}

	m_isNull = false;
	m_hasValue = false;
	m_isValid = false;
	m_dataPtr = nullptr;
}

template<class T>
T& TNullable<T>::Emplace()
{
	m_isNull = false;
	m_hasValue = true;
	m_isValid = true;
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
	}

	m_dataPtr = new T;

	return *m_dataPtr;
}


template<class T>
inline void TNullable<T>::SetNull()
{
	Reset();
	m_isNull = true;
	m_isValid = true;
}


// operators

template<class T>
TNullable<T>& TNullable<T>::operator=(const T& value)
{
	m_hasValue = true;
	m_isNull = false;
	m_isValid = true;
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
	}

	m_dataPtr = new T;
	*m_dataPtr = value;

	return *this;
}


template<class T>
TNullable<T>& TNullable<T>::operator=(T&& value)
{
	m_hasValue = true;
	m_isNull = false;
	m_isValid = true;
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
	}

	m_dataPtr = new T;
	*m_dataPtr = std::move(value);

	return *this;
}


// STL-compatibility methods
// access operators

template<class T>
const T* TNullable<T>::operator->() const
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return m_dataPtr;
}


template<class T>
T* TNullable<T>::operator->()
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return m_dataPtr;
}


template<class T>
const T& TNullable<T>::operator*() const &
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}


template<class T>
T& TNullable<T>::operator*() &
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}


template<class T>
const T&& TNullable<T>::operator*() const &&
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return std::move(*m_dataPtr);
}


template<class T>
T&& TNullable<T>::operator*() &&
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return std::move(*m_dataPtr);
}


template<class T>
TNullable<T>& TNullable<T>::operator=( std::nullptr_t )
{
	Reset();
	m_isNull = true;
	m_isValid = true;

	return *this;
}


template<class T>
TNullable<T>& TNullable<T>::operator=(const TNullable& other)
{
	if (m_dataPtr != nullptr){
		delete m_dataPtr;
		m_dataPtr = nullptr;
	}
	if (other.m_dataPtr != nullptr){
		m_dataPtr = new T;
		*m_dataPtr = *other.m_dataPtr;
	}

	m_hasValue = other.m_hasValue;
	m_isNull = other.m_isNull;
	m_isValid = other.m_isValid;

	return *this;
}


template<class T>
TNullable<T>::operator bool() const
{
	return m_hasValue;
}


template<class T>
bool TNullable<T>::has_value() const
{
	return m_hasValue;
}


template<class T>
T& TNullable<T>::value() &
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}


template<class T>
const T& TNullable<T>::value() const &
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return *m_dataPtr;
}


template<class T>
T&& TNullable<T>::value() &&
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return std::move(*m_dataPtr);
}


template<class T>
const T&& TNullable<T>::value() const &&
{
	Q_ASSERT_X(!m_isNull && m_hasValue, __func__, "Access to uninitialized data");

	return std::move(*m_dataPtr);
}


template<class T>
T TNullable<T>::value_or(const T& default_value) const
{
	if (m_hasValue && !m_isNull){
		return *m_dataPtr;
	}

	return default_value;
}


template<class T>
void TNullable<T>::swap(TNullable& other )
{
	std::swap(m_dataPtr, other.m_dataPtr);
	std::swap(m_hasValue, other.m_hasValue);
	std::swap(m_isNull, other.m_isNull);
	std::swap(m_isValid, other.m_isValid);
}


template<class T>
void TNullable<T>::reset()
{
	Reset();
}


template<class T>
T& TNullable<T>::emplace()
{
	return Emplace();
}


template<class T>
T& TNullable<T>::emplace(T&& value)&&
{
	m_dataPtr = std::move(value);
	m_hasValue = true;
	m_isNull = false;
	m_isValid = true;

	return *m_dataPtr;
}


template<class T>
bool TNullable<T>::operator==(const TNullable& other) const
{
	bool retVal = m_hasValue == other.m_hasValue && m_isNull == other.m_isNull && m_isValid == other.m_isValid;

	if (retVal && (m_dataPtr != nullptr && other.m_dataPtr != nullptr)){
		retVal = *m_dataPtr == *other.m_dataPtr;
	}

	return retVal;
}


template<class T>
bool TNullable<T>::operator!=(const TNullable& other) const
{
	return (!operator==(other));
}


template<class T>
bool TNullable<T>::operator==(std::nullptr_t) const
{
	return m_dataPtr == nullptr;
}


template<class T>
bool TNullable<T>::operator!=(std::nullptr_t) const
{
	return m_dataPtr != nullptr;
}


template<class T>
bool TNullable<T>::operator==(const T& value) const
{
	if (!m_hasValue || m_isNull){
		Q_ASSERT_X(false, __func__, "Comnpare with uninitialized data");

		return false;
	}

	return *m_dataPtr == value;
}


template<class T>
bool TNullable<T>::operator!=(const T& value) const
{
	if (!m_hasValue || m_isNull){
		Q_ASSERT_X(false, __func__, "Comnpare with uninitialized data");

		return false;
	}

	return *m_dataPtr != value;
}


} // namespace istd



#endif // !istd_TNullable_included



