#pragma once


// ACF includes
#include <istd/TInterfacePtr.h>


namespace istd
{


/**
	@brief A wrapper for managed and unmanaged memory.
	
	The TOptInterfacePtr class allows you to store either:
	- A managed pointer (std::shared_ptr<T>) that controls the lifetime of the object.
	- An unmanaged raw pointer (T*) that is non-owning and must be managed externally.
	@tparam InterfaceType Type of the object being wrapped.
*/
template<class InterfaceType>
class TOptInterfacePtr
{
public:
	typedef InterfaceType Interface;
	typedef istd::TUniqueInterfacePtr<InterfaceType> UniqueInterfacePtr;
	typedef istd::TSharedInterfacePtr<InterfaceType> SharedInterfacePtr;

	TOptInterfacePtr(TOptInterfacePtr&&) noexcept = default;
	TOptInterfacePtr& operator=(TOptInterfacePtr&&) noexcept = default;
	TOptInterfacePtr() = default;

	// Disallow ambiguous raw-ptr constructor (use SetUnmanagedPtr or AdoptRawPtr)
	TOptInterfacePtr(Interface* unamangedPtr) = delete;


	// Non-owning, explicit
	static TOptInterfacePtr FromUnmanaged(InterfaceType* ptr)
	{
		TOptInterfacePtr retVal;

		retVal.SetUnmanagedPtr(ptr);

		return retVal;
	}


	explicit TOptInterfacePtr(SharedInterfacePtr managedPtr)
		:m_sharedPtr(managedPtr)
	{
	}


	explicit TOptInterfacePtr(UniqueInterfacePtr& managedPtr)
	{
		m_sharedPtr.FromUnique(managedPtr);
	}


	void SetUnmanagedPtr(Interface* ptr)
	{
		m_unmanagedPtr = ptr;

		m_sharedPtr.Reset();

		AssertInvariant();
	}


	void SetManagedPtr(SharedInterfacePtr managedPtr)
	{
		m_sharedPtr = managedPtr;

		m_unmanagedPtr = nullptr;

		AssertInvariant();
	}


	/**
		Adopts ownership of a raw pointer.The pointer must be allocated in a manner compatible with SharedInterfacePtr's deleter.
		Do NOT pass stack pointers or memory owned elsewhere.
	*/
	void AdoptRawPtr(Interface* rawPtr)
	{
		m_sharedPtr = rawPtr;

		m_unmanagedPtr = nullptr;

		AssertInvariant();
	}
	

	template <typename T>
	bool AdoptCastedRawPtr(T* rawPtr)
	{
		if (rawPtr == nullptr) {
			Reset();

			return true;
		}

		InterfaceType* castedPtr = dynamic_cast<InterfaceType*> (rawPtr);
		if (castedPtr != nullptr) {
			AdoptRawPtr(castedPtr);

			return true;
		}

		return false;
	}


	template <class T>
	bool SetCastedPtr(TSharedInterfacePtr<T>& source)
	{
		if (!source.IsValid()) {
			Reset();

			return true;
		}

		SharedInterfacePtr sharedPtr;
		if (sharedPtr.SetCastedPtr(source)) {
			m_sharedPtr = sharedPtr;

			m_unmanagedPtr = nullptr;

			return true;
		}

		return false;
	}


	void TakeOver(TOptInterfacePtr& ptr)
	{
		m_sharedPtr = ptr.m_sharedPtr;

		m_unmanagedPtr = ptr.m_unmanagedPtr;

		ptr.m_sharedPtr.Reset();
		ptr.m_unmanagedPtr = nullptr;

		AssertInvariant();
	}


	template <class T>
	bool TakeOver(TUniqueInterfacePtr<T>& source)
	{
		if (!source.IsValid()) {
			Reset();

			AssertInvariant();

			return true;
		}

		SharedInterfacePtr sharedPtr = SharedInterfacePtr::CreateFromUnique(source);
		if (sharedPtr.IsValid()) {
			m_sharedPtr = sharedPtr;
			m_unmanagedPtr = nullptr;

			Q_ASSERT(!source.IsValid());

			AssertInvariant();

			return true;
		}

		return false;
	}


	template <class T>
	bool TakeOver(TUniqueInterfacePtr<T>&& source)
	{
		if (!source.IsValid()) {
			Reset();

			AssertInvariant();

			return true;
		}

		SharedInterfacePtr sharedPtr = SharedInterfacePtr::CreateFromUnique(source);
		if (sharedPtr.IsValid()) {
			m_sharedPtr = sharedPtr;
			m_unmanagedPtr = nullptr;
			Q_ASSERT(!source.IsValid());

			AssertInvariant();

			return true;
		}

		return false;
	}


	bool IsValid() const
	{
		return (m_unmanagedPtr != nullptr) || m_sharedPtr.IsValid();
	}


	bool IsManaged() const
	{
		return m_sharedPtr.IsValid();
	}


	bool IsUnmanaged() const
	{
		return (m_unmanagedPtr != nullptr);
	}


	void Reset()
	{
		m_unmanagedPtr = nullptr;

		m_sharedPtr.Reset();
	}


	template<typename InterfaceCast = InterfaceType>
	const InterfaceCast* GetPtr() const
	{
		AssertInvariant();

		const Interface* retVal = nullptr;
		if (m_unmanagedPtr != nullptr){
			retVal = m_unmanagedPtr;
		}
		else if (m_sharedPtr.IsValid()){
			retVal = m_sharedPtr.GetPtr();
		}
		else {
			return nullptr;
		}

		if constexpr (std::is_same_v<InterfaceCast, InterfaceType>){
			return retVal;
		}
		else{
			return dynamic_cast<const InterfaceCast*>(retVal);
		}
	}


	template<typename InterfaceCast = InterfaceType>
	InterfaceCast* GetPtr() // non-const
	{
		AssertInvariant();
	
		Interface* retVal = nullptr;
		if (m_unmanagedPtr) {
			retVal = m_unmanagedPtr;
		}
		else if (m_sharedPtr.IsValid()) {
			retVal = m_sharedPtr.GetPtr(); // should be non-const getter
		}
		else {
			return nullptr;
		}

		if constexpr (std::is_same_v<InterfaceCast, InterfaceType>) {
			return retVal;
		}
		else {
			return dynamic_cast<InterfaceCast*>(retVal);
		}
	}


	const Interface* operator->() const
	{
		Q_ASSERT(GetPtr() != nullptr);

		return GetPtr();
	}


	Interface* operator->()
	{
		Q_ASSERT(GetPtr() != nullptr);

		return GetPtr();
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


	/**
		Copy constructor.
	*/
	TOptInterfacePtr(const TOptInterfacePtr& ptr)
	{
		m_sharedPtr = ptr.m_sharedPtr;

		m_unmanagedPtr = ptr.m_unmanagedPtr;
	}


	/**
		Assign operator.
	*/
	TOptInterfacePtr& operator=(const TOptInterfacePtr& ptr)
	{
		m_sharedPtr = ptr.m_sharedPtr;

		m_unmanagedPtr = ptr.m_unmanagedPtr;

		return *this;
	}


	explicit operator bool() const noexcept
	{
		return IsValid();
	}

private:
	inline void AssertInvariant() const
	{
		Q_ASSERT((m_sharedPtr.IsValid() && (m_unmanagedPtr == nullptr)) || (!m_sharedPtr.IsValid() && (m_unmanagedPtr != nullptr)) || (!m_sharedPtr.IsValid() && (m_unmanagedPtr == nullptr)));
	}

private:

	/**
		Managed memory
	*/
	SharedInterfacePtr m_sharedPtr;

	/**
		Unmanaged memory.
	*/
	Interface* m_unmanagedPtr = nullptr;
};


} // namespace istd


