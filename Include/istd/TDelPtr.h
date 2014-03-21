#ifndef istd_TDelPtr_included
#define istd_TDelPtr_included


// ACF includes
#include "istd/TPointerBase.h"


namespace istd
{


/**
	Pointer wrapper providing automatic deleting pointed object during destruction.

	\ingroup Main
*/
template <class Type, class Accessor = DefaultAccessor<Type> >
class TDelPtr: public TPointerBase<Type>
{
public:
	typedef TPointerBase<Type> BaseClass;

	/**
		Construct and init this pointer.
		\param	ptr		pointer to object.
	*/
	TDelPtr(Type* ptr = NULL);

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TDelPtr(const TDelPtr& ptr);

	/**
		Destructor.
		It calls Reset() to delete pointed object.
	 */
	~TDelPtr();

	/**
		Remove object pointed by internal pointer and set this pointer to NULL.
	*/
	void Reset();

	/**
		Set new value of internal pointer.
		If internal pointer wasn't NULL, it will be deleted.
	 */
	void SetPtr(Type* ptr);

	/**
		Reset internal pointer value without deleting instance and return previos value.
	 */
	Type* PopPtr();
	/**
		Take internal pointer over.
		It set pointer from other object and detach it from them.
	*/
	void TakeOver(TDelPtr& sourcePtr);

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TDelPtr& operator=(const TDelPtr& ptr);
	/**
		Assign operator.
		It removes refererenced object before new is assigned.
		\param	ptr		pointer to object.
	*/
	TDelPtr& operator=(Type* ptr);

	/**
		Set internal pointer using casted pointer of other type.
		If casting is not possible, parameter object will be removed.
	*/
	template <class SourceType>
	bool SetCastedOrRemove(SourceType* ptr)
	{
		Type* castedPtr = dynamic_cast<Type*>(ptr);

		SetPtr(castedPtr);

		if (castedPtr != NULL){
			return true;
		}
		else{
			delete ptr;

			return false;
		}
	}

protected:
	/**
		Remove referenced object.
		Warning: internal pointer will be not changed.
	*/
	void Detach();
};


// inline methods

template <class Type, class Accessor>
inline TDelPtr<Type, Accessor>::TDelPtr(Type* ptr)
:	BaseClass(ptr)
{
}


template <class Type, class Accessor>
inline TDelPtr<Type, Accessor>::TDelPtr(const TDelPtr<Type, Accessor>& I_IF_DEBUG(ptr))
:	BaseClass(NULL)
{
	Q_ASSERT(ptr.GetPtr() == NULL);
}


template <class Type, class Accessor>
inline TDelPtr<Type, Accessor>::~TDelPtr()
{
	TDelPtr::Detach();
}


template <class Type, class Accessor>
inline void TDelPtr<Type, Accessor>::Reset()
{
	SetPtr(NULL);
}


template <class Type, class Accessor>
inline void TDelPtr<Type, Accessor>::SetPtr(Type* ptr)
{
	Detach();

	BaseClass::SetPtr(ptr);
}


template <class Type, class Accessor>
inline Type* TDelPtr<Type, Accessor>::PopPtr()
{
	Type* slavePtr = BaseClass::GetPtr();
	BaseClass::SetPtr(NULL);

	return slavePtr;
}


template <class Type, class Accessor>
void TDelPtr<Type, Accessor>::TakeOver(TDelPtr& sourcePtr)
{
	SetPtr(sourcePtr.PopPtr());
}


// public methods

template <class Type, class Accessor>
TDelPtr<Type, Accessor>& TDelPtr<Type, Accessor>::operator=(const TDelPtr& I_IF_DEBUG(ptr))
{
	Q_ASSERT(ptr.GetPtr() == NULL);

	Reset();

	return *this;
}


template <class Type, class Accessor>
TDelPtr<Type, Accessor>& TDelPtr<Type, Accessor>::operator=(Type* ptr)
{
	Detach();

	BaseClass::SetPtr(ptr);

	return *this;
}


// protected methods

template <class Type, class Accessor>
void TDelPtr<Type, Accessor>::Detach()
{
	Type* ptr = BaseClass::GetPtr();
	if (ptr == NULL){
		return;
	}

	Accessor::Delete(ptr);
}


} // namespace istd


#endif // !istd_TDelPtr_included


