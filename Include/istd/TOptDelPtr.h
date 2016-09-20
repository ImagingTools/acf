#ifndef istd_TOptDelPtr_included
#define istd_TOptDelPtr_included


// ACF includes
#include <istd/istd.h>
#include <istd/TPointerBase.h>


namespace istd
{


/**
	Pointer wrapper providing activatable deleting pointed object during destruction.
		
	\ingroup Main
*/
template <class Type, bool DelArray = false>
class TOptDelPtr: public TPointerBase<Type>
{
public:
	typedef TPointerBase<Type> BaseClass;

	/**
		Construct and init this pointer.
		\param	ptr			pointer to object.
		\param	releaseFlag	if true, pointed object will be automatically released.
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
		Get state of release flag.
		If this flag is true, pointed object will be automatically removed from memory.
		If this pointer isn't valid, returned value has no meaning.
	*/
	bool IsToRelase() const;

	/**
		Remove object pointed by internal pointer and set this pointer to NULL.
	*/
	void Reset();

	/**
		Set new value of internal pointer.
		If internal pointer wasn't NULL, it will be deleted.
		\param	releaseFlag	if true, pointed object will be automatically released.
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

	/**
		Set internal pointer using casted pointer of other type.
		If casting is not possible, parameter object will be removed.
		\param	releaseFlag	if true, pointed object will be automatically released.
	*/
	template <class SourceType>
	bool SetCastedOrRemove(SourceType* ptr, bool releaseFlag = false)
	{
		Type* castedPtr = dynamic_cast<Type*>(ptr);

		SetPtr(castedPtr, releaseFlag);

		if (castedPtr != NULL){
			return true;
		}
		else if (releaseFlag){
			delete ptr;
		}

		return false;
	}

protected:
	/**
		Remove referenced object.
		Warning: internal pointer will be not changed.
	*/
	void Detach();

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
:	BaseClass(NULL)
{
	if (!ptr.m_releaseFlag){
		SetPtr(ptr.GetPtr(), false);

		return;
	}

	Q_ASSERT(ptr.GetPtr() == NULL);
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
inline bool TOptDelPtr<Type, DelArray>::IsToRelase() const
{
	return m_releaseFlag;
}


template <class Type, bool DelArray>
inline void TOptDelPtr<Type, DelArray>::SetPtr(Type* ptr, bool releaseFlag)
{
	Detach();

	BaseClass::SetPtr(ptr);

	m_releaseFlag = releaseFlag;
}


template <class Type, bool DelArray>
inline Type* TOptDelPtr<Type, DelArray>::PopPtr()
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
TOptDelPtr<Type, DelArray>& TOptDelPtr<Type, DelArray>::operator=(const TOptDelPtr& I_IF_DEBUG(ptr))
{
	if (m_releaseFlag){
		I_IF_DEBUG(Q_ASSERT(ptr.GetPtr() == NULL));
	}

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


