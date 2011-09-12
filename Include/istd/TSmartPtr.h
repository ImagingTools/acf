#ifndef istd_TSmartPtr_included
#define istd_TSmartPtr_included


#include "istd/TTransPtr.h"


namespace istd
{


/**
	Implementation of a smart pointer.
	This implementation uses internal counter object. Internal counter object is used to
	count of instances of smart pointers and will be shared by all its copies.

	\ingroup Main
*/
template <class Type, class Accessor = DefaultAccessor<Type> >
class TSmartPtr: public TTransPtr<Type>
{
public:
	typedef TTransPtr<Type> BaseClass;
	typedef typename BaseClass::RefCountBase RefCountBase;

	TSmartPtr();
	TSmartPtr(Type* pointer);
	TSmartPtr(const TTransPtr<Type>& pointer);
	TSmartPtr(const TSmartPtr& pointer);

	/**
		Set pointed object.
	*/
	void SetPtr(Type* pointer);

	/**
		Set this pointer using casted pointer of other type.
		If casting is not possible, object will be removed.
	*/
	template <class SourceType>
	bool SetCastedOrRemove(SourceType* pointer)
	{
		Type* castedPtr = dynamic_cast<Type*>(pointer);

		SetPtr(castedPtr);

		if (castedPtr != NULL){
			return true;
		}
		else{
			Accessor::Delete(pointer);

			return false;
		}
	}

	// operators
	TSmartPtr& operator=(const TTransPtr<Type>& pointer);
	TSmartPtr& operator=(const TSmartPtr& pointer);

protected:
	class RefCounter: public RefCountBase
	{
	public:
		typedef RefCountBase BaseClass;

		RefCounter(Type* pointer)
		:	BaseClass(pointer)
		{
			m_count = 1;
		}

		// reimplemented (RefCountBase)
		virtual void OnAttached()
		{
			I_ASSERT(IsValid());
			I_ASSERT(m_count > 0);

			++m_count;
		}

		virtual void OnDetached()
		{
			I_ASSERT(IsValid());
			I_ASSERT(m_count > 0);

			if (--m_count <= 0){
				Accessor::Delete(BaseClass::GetPtr());

				delete this;
			}
		}

	private:
		int m_count;
	};
};


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr()
{
	m_counterPtr = NULL;
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(Type* pointer)
{
	m_counterPtr = new RefCounter(pointer);
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(const TTransPtr<Type>& pointer)
{
	m_counterPtr = GetInternalCounter(pointer);

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>::TSmartPtr(const TSmartPtr& pointer)
{
	m_counterPtr = pointer.m_counterPtr;

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}
}


template <class Type, class Accessor>
inline void TSmartPtr<Type, Accessor>::SetPtr(Type* pointer)
{
	Detach();

	if (pointer != NULL){
		m_counterPtr = new RefCounter(pointer);
	}
	else{
		m_counterPtr = NULL;
	}
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>& TSmartPtr<Type, Accessor>::operator=(const TTransPtr<Type>& pointer)
{
	Detach();

	m_counterPtr = GetInternalCounter(pointer);

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}

	return *this;
}


template <class Type, class Accessor>
TSmartPtr<Type, Accessor>& TSmartPtr<Type, Accessor>::operator=(const TSmartPtr& pointer)
{
	Detach();

	m_counterPtr = pointer.m_counterPtr;

	if (m_counterPtr != NULL){
		m_counterPtr->OnAttached();
	}

	return *this;
}


} // namespace istd


#endif // !istd_TSmartPtr_included


