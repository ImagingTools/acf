#ifndef istd_TRetSmartPtr_included
#define istd_TRetSmartPtr_included


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <istd/TPointerBase.h>


namespace istd
{


/**
	Implementation of data transfer smart pointer.
	It should be used to pointer transfer only, not to store the pointers becouse it overtakes the pointer.
	Typically it is used as smart pointer return parameters.
	The adventage against 'normal' smart pointer (istd::TSmartPtr) is that the copy constructor is faster, because it overtakes the pointer.
	\sa istd::TSmartPtr
*/
template <class Type>
class TTransPtr
{
public:
	/**
		Constructor overtaking the pointer.
		\param	pointer	pointer to overtake - \em warning: After this operation this pointer is invalid!
	*/
	TTransPtr(const TTransPtr& pointer);
	~TTransPtr();

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

	/**
		Set this pointer to NULL.
	*/
	void Reset();

	/**
		Swap two pointers.
	*/
	void Swap(TTransPtr& pointer);

	template <class CastedType>
	CastedType Cast() const
	{
		return dynamic_cast<CastedType>(GetPtr());
	}

	// operators
	/**
		Copy operator overtaking the pointer.
		\param	pointer	pointer to overtake - \em warning: After this operation this pointer is invalid!
	*/
	TTransPtr& operator=(const TTransPtr& pointer);
	Type& operator*() const;
	Type* operator->() const;

protected:
	class RefCountBase: public TPointerBase<Type>
	{
	public:
		typedef TPointerBase<Type> BaseClass;

		explicit RefCountBase(Type* pointer)
		:	BaseClass(pointer)
		{
			Q_ASSERT(pointer != NULL);
		}
		virtual ~RefCountBase(){}

		// abstract methods
		/**
			Called if new pointer is attached to this internal handler.
		*/
		virtual void OnAttached() = 0;
		/**
			Called if pointer is detached from this internal handler.
		*/
		virtual void OnDetached() = 0;
	};

	TTransPtr();

	/**
		Detach counter object without changing of internal counter pointer.
	*/
	void Detach();
	/**
		Get internal counter.
	*/
	RefCountBase* GetInternalCounter(const TTransPtr& pointer) const;

	RefCountBase* m_counterPtr;
};


// public methods

template <class Type>
TTransPtr<Type>::TTransPtr(const TTransPtr& pointer)
:	m_counterPtr(pointer.m_counterPtr)
{
	const_cast<TTransPtr&>(pointer).m_counterPtr = NULL;
}


template <class Type>
TTransPtr<Type>::~TTransPtr()
{
	Detach();
}


template <class Type>
bool TTransPtr<Type>::IsValid() const
{
	return (m_counterPtr != NULL) && (m_counterPtr->GetPtr() != NULL);
}


template <class Type>
inline const Type* TTransPtr<Type>::GetPtr() const
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type>
inline Type* TTransPtr<Type>::GetPtr()
{
	if (m_counterPtr != NULL){
		return m_counterPtr->GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Type>
void TTransPtr<Type>::Reset()
{
	Detach();

	m_counterPtr = NULL;
}


template <class Type>
void TTransPtr<Type>::Swap(TTransPtr& pointer)
{
	qSwap(m_counterPtr, pointer.m_counterPtr);
}


template <class Type>
TTransPtr<Type>& TTransPtr<Type>::operator=(const TTransPtr& pointer)
{
	if (m_counterPtr != pointer.m_counterPtr){
		Detach();

		m_counterPtr = pointer.m_counterPtr;

		const_cast<TTransPtr&>(pointer).m_counterPtr = NULL;
	}

	return *this;
}


template <class Type>
inline Type& TTransPtr<Type>::operator*() const
{
	Q_ASSERT(m_counterPtr != NULL);
	Q_ASSERT(m_counterPtr->GetPtr() != NULL);

	return *m_counterPtr->GetPtr();
}


template <class Type>
inline Type* TTransPtr<Type>::operator->() const
{
	Q_ASSERT(m_counterPtr != NULL);
	Q_ASSERT(m_counterPtr->GetPtr() != NULL);

	return m_counterPtr->GetPtr();
}


// protected methods

template <class Type>
TTransPtr<Type>::TTransPtr()
:	m_counterPtr(NULL)
{
}


template <class Type>
inline void TTransPtr<Type>::Detach()
{
	if (m_counterPtr != NULL){
		m_counterPtr->OnDetached();
	}
}


template <class Type>
typename TTransPtr<Type>::RefCountBase* TTransPtr<Type>::GetInternalCounter(const TTransPtr& pointer) const
{
	return pointer.m_counterPtr;
}


} // namespace istd


#endif // !istd_TRetSmartPtr_included


