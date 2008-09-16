#ifndef TSmartPtr_included
#define TSmartPtr_included


#include "istd/istd.h"

#include "istd/IPolymorphic.h"


namespace istd
{

/**
	Implementation of a smart pointer.
*/
template <typename PointerType>
class TSmartPtr: public istd::IPolymorphic
{
public:
	TSmartPtr();
	explicit TSmartPtr(PointerType* pointer);
	TSmartPtr(const TSmartPtr& other);

	virtual ~TSmartPtr();

	void SetPtr(PointerType* pointer);
	const PointerType* GetPtr() const;
	PointerType* GetPtr();
	const PointerType* operator->() const;
	PointerType* operator->();
	TSmartPtr& operator=(const TSmartPtr& otherCounter);
	bool IsValid() const;

protected:
	void Detach();

protected:
	class Counter
	{
	public:
		Counter(PointerType* ptr)
		{
			objectPtr = ptr;
			count = 0;
		}

		~Counter()
		{
			if (objectPtr != NULL && count == 0){
				delete objectPtr;
			}
		}

		int count;
		PointerType* objectPtr;
	};

	typename Counter* m_counter;
};


template <typename PointerType>
TSmartPtr<PointerType>::TSmartPtr()
{
	m_counter = new Counter(NULL);
}


template <typename PointerType>
TSmartPtr<PointerType>::TSmartPtr(PointerType* pointer)
{
	m_counter = NULL;

	SetPtr(pointer);
}


template <typename PointerType>
TSmartPtr<PointerType>::TSmartPtr(const TSmartPtr<PointerType>& other)
{
	m_counter = other.m_counter;
	m_counter->count++;
}


template <typename PointerType>
TSmartPtr<PointerType>::~TSmartPtr()
{
	Detach();
}


template <typename PointerType>
inline void TSmartPtr<PointerType>::SetPtr(PointerType* pointer)
{
	Detach();

	m_counter = new Counter(pointer);
}


template <typename PointerType>
inline const PointerType* TSmartPtr<PointerType>::GetPtr() const
{
	I_ASSERT(m_counter != NULL);

	return m_counter->objectPtr;
}


template <typename PointerType>
inline PointerType* TSmartPtr<PointerType>::GetPtr()
{
	I_ASSERT(m_counter != NULL);

	return m_counter->objectPtr;
}


template <typename PointerType>
inline const PointerType* TSmartPtr<PointerType>::operator->() const
{
	I_ASSERT(m_counter != NULL);
	I_ASSERT(m_counter->objectPtr != NULL);

	return GetPtr();
}


template <typename PointerType>
inline PointerType* TSmartPtr<PointerType>::operator->()
{
	I_ASSERT(m_counter != NULL);
	I_ASSERT(m_counter->objectPtr != NULL);

	return GetPtr();
}


template <typename PointerType>
TSmartPtr<PointerType>& TSmartPtr<PointerType>::operator=(const TSmartPtr<PointerType>& other)
{
	Detach();

	if (other.IsValid()){
		(const_cast<TSmartPtr<PointerType>&>(other)).m_counter->count++;
		m_counter = other.m_counter;
	}

	return *this;
}


template <typename PointerType>
bool TSmartPtr<PointerType>::IsValid() const
{
	if (m_counter != NULL){
		return (m_counter->objectPtr != NULL);
	}

	return false;
}


template <typename PointerType>
void TSmartPtr<PointerType>::Detach()
{
	if (m_counter != NULL){
		if(m_counter->count == 0){
			delete m_counter;
			m_counter = NULL;
		}
		else{
			m_counter->count--;
			I_ASSERT(m_counter->count >= 0);
		}
	}
}


} // namespace istd


#endif // !TSmartPtr_included

