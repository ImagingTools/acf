#pragma once


// ACF includes
#include <istd/TInterfacePtr.h>


namespace istd
{


template<class InterfaceType>
class TOptInterfacePtr
{
public:
	typedef InterfaceType Interface;
	typedef istd::TUniqueInterfacePtr<InterfaceType> UniqueInterfacePtr;
	typedef istd::TSharedInterfacePtr<InterfaceType> SharedInterfacePtr;

	TOptInterfacePtr()
	{
	}

	TOptInterfacePtr(Interface* ptr)
		:m_ptr(ptr)
	{
	}

	TOptInterfacePtr(SharedInterfacePtr ptr)
		:m_taskPtr(ptr)
	{
	}

	TOptInterfacePtr(UniqueInterfacePtr& ptr)
	{
		m_taskPtr.FromUnique(ptr);
	}

	void SetPtr(UniqueInterfacePtr& ptr)
	{
		m_taskPtr.FromUnique(ptr);
	}

	void SetPtr(SharedInterfacePtr ptr)
	{
		m_taskPtr = ptr;
	}

	void TakeOver(Interface* ptr)
	{
		m_ptr = SharedInterfacePtr(ptr);
	}

	void SetOptionalPtr(Interface* ptr)
	{
		m_ptr = ptr;
	}

	bool IsValid() const
	{
		return (m_ptr != nullptr) || m_taskPtr.IsValid();
	}

	void Reset() const
	{
		m_ptr = nullptr;
		m_taskPtr.Reset();
	}

	template<typename InterfaceCast = InterfaceType>
	InterfaceCast* GetPtr() const
	{
		Interface* retVal = nullptr;
		if (m_ptr != nullptr){
			retVal = m_ptr;
		}
		else if (m_taskPtr.IsValid()){
			retVal = m_taskPtr.GetPtr();
		}
		else {
			return nullptr;
		}

		if constexpr (std::is_same_v<InterfaceCast, InterfaceType>){
			return static_cast<InterfaceCast*>(retVal);
		}
		else{
			return dynamic_cast<InterfaceCast*>(retVal);
		}

		return nullptr;
	}

	const Interface* operator->() const
	{
		Q_ASSERT(GetPtr() != nullptr);

		return GetPtr();
	}

	Interface* operator->()
	{
		Q_ASSERT(GetPtr() != nullptr);

		return const_cast<Interface*>(GetPtr());
	}

	const Interface& operator*() const
	{
		Q_ASSERT(GetPtr() != nullptr);

		return *GetPtr();
	}

	Interface& operator*()
	{
		Q_ASSERT(GetPtr() != nullptr);

		return *GetPtr();
	}

private:
	SharedInterfacePtr m_taskPtr;
	Interface* m_ptr = nullptr;
};


} // namespace istd


