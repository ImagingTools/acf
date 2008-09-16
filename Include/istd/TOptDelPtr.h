#ifndef istd_TOptDelPtr_included
#define istd_TOptDelPtr_included


#include "istd/TPointer.h"


namespace istd
{


/**
	Pointer wrapper providing automatic deleting pointed object during destruction.
*/
template <class Type, bool DelArray = false>
class TOptDelPtr: public TPointer<Type>
{
public:
	typedef TPointer<Type> BaseClass;

	/**
		Construct and init this pointer.
		\param	ptr		pointer to object.
	*/
	TOptDelPtr(Type* ptr = NULL, bool releaseFlag = false);

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TOptDelPtr(const TOptDelPtr& ptr);

	/**
		Destructor.
		It calls Reset() to delete pointed object.
	 */
	~TOptDelPtr();

	/**
		Remove object pointed by internal pointer and set this pointer to NULL.
	*/
	void Reset();

	/**
		Set new value of internal pointer.
		If internal pointer wasn't NULL, it will be deleted.
	 */
	void SetPtr(Type* ptr, bool releaseFlag = false);

	/**
		Reset internal pointer value without deleting instance and return previos value.
	 */
	Type* PopPtr();
	/**
		Take internal pointer over.
		It set pointer from other object and detach it from them.
	*/
	void TakeOver(TOptDelPtr& sourcePtr);

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TOptDelPtr& operator=(const TOptDelPtr& ptr);
	/**
		Assign operator.
		It removes refererenced object before new is assigned.
		\param	ptr		pointer to object.
	*/
	TOptDelPtr& operator=(Type* ptr);

protected:
	/**
		Remove referenced object.
		Warning: internal pointer will be not changed.
	*/
	virtual void Detach();

private:
	bool m_releaseFlag;
};


// inline methods

template <class Type, bool DelArray>
inline TOptDelPtr<Type, DelArray>::TOptDelPtr(Type* ptr, bool releaseFlag)
:	BaseClass(ptr), m_releaseFlag(releaseFlag)
{
}


template <class Type, bool DelArray>
inline TOptDelPtr<Type, DelArray>::TOptDelPtr(const TOptDelPtr<Type, DelArray>& ptr)
{
	I_ASSERT(ptr.GetPtr() == NULL);
}


template <class Type, bool DelArray>
inline TOptDelPtr<Type, DelArray>::~TOptDelPtr()
{
	Detach();
}


template <class Type, bool DelArray>
inline void TOptDelPtr<Type, DelArray>::Reset()
{
	SetPtr(NULL);
}


template <class Type, bool DelArray>
inline void TOptDelPtr<Type, DelArray>::SetPtr(Type* ptr, bool releaseFlag)
{
	Detach();

	BaseClass::SetPtr(ptr);

	m_releaseFlag = releaseFlag;
}


template <class Type, bool DelArray>
inline typename Type* TOptDelPtr<Type, DelArray>::PopPtr()
{
	Type* slavePtr = BaseClass::GetPtr();
	BaseClass::SetPtr(NULL);

	return slavePtr;
}


template <class Type, bool DelArray>
void TOptDelPtr<Type, DelArray>::TakeOver(TOptDelPtr& sourcePtr)
{
	SetPtr(sourcePtr.PopPtr());
}


// public methods

template <class Type, bool DelArray>
TOptDelPtr<Type, DelArray>& TOptDelPtr<Type, DelArray>::operator=(const TOptDelPtr& ptr)
{
	I_ASSERT(ptr.GetPtr() == NULL);

	Reset();

	return *this;
}


template <class Type, bool DelArray>
TOptDelPtr<Type, DelArray>& TOptDelPtr<Type, DelArray>::operator=(Type* ptr)
{
	Detach();

	BaseClass::SetPtr(ptr);

	return *this;
}


// protected methods

template <class Type, bool DelArray>
void TOptDelPtr<Type, DelArray>::Detach()
{
	if (m_releaseFlag){
		Type* ptr = BaseClass::GetPtr();
		if (ptr == NULL){
			return;
		}

		if (DelArray){
			delete[] ptr;
		}
		else{
			delete ptr;
		}
	}
}


} // namespace istd


#endif // !istd_TOptDelPtr_included


