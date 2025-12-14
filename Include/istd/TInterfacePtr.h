#pragma once


// STL includes
#include <memory>
#include <utility>
#include <functional>

// Qt includes
#include <QtCore/QDebug>

// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{


/**
	Specialized polymorphic pointer that manages the allocated root object
	(m_rootPtr) and simultaneously provides a pointer to the actual interface implementation (m_interfacePtr).
	The interface instance is "derived" from the root object using the provided
	extractor (ExtractInterfaceFunc) or an alternative extraction mechanism.
	Ownership and lifetime are tied to m_rootPtr; m_interfacePtr is a non-owning
	view into the interface and may point to a different (sub)object than m_rootPtr.
*/
template <class InterfaceType, class PolymorphicPointerImpl>
class TInterfacePtr
{
public:
	typedef PolymorphicPointerImpl RootObjectPtr;
	typedef std::function<InterfaceType* ()> ExtractInterfaceFunc;

	bool IsValid() const noexcept
	{
		return m_interfacePtr != nullptr;
	}

	template<typename Interface = InterfaceType>
	Interface* GetPtr() noexcept
	{
		if constexpr (std::is_same_v<Interface, InterfaceType>){
			return m_interfacePtr;
		}
		else{
			return dynamic_cast<Interface*>(m_interfacePtr);
		}
	}

	template<typename Interface = InterfaceType>
	const Interface* GetPtr() const noexcept
	{
		if constexpr (std::is_same_v<Interface, InterfaceType>){
			return m_interfacePtr;
		}
		else{
			return dynamic_cast<const Interface*>(m_interfacePtr);
		}
	}

	const InterfaceType* operator->() const noexcept
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return m_interfacePtr;
	}

	InterfaceType* operator->() noexcept
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return m_interfacePtr;
	}

	const InterfaceType& operator*() const noexcept
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return *m_interfacePtr;
	}

	InterfaceType& operator*() noexcept
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return *m_interfacePtr;
	}

	void Reset() noexcept
	{
		m_rootPtr.reset();
		m_interfacePtr = nullptr;
	}

	/**
		\note This overload assumes the provided interfacePtr actually points to
		an object whose dynamic type is compatible with RootObjectPtr::element_type.
		Prefer SetPtr(root, extract) when possible.
	*/
	void SetPtr(InterfaceType* interfacePtr)
	{
		// Reset root with the raw pointer. This is only safe if the pointer types match.
		m_rootPtr.reset(interfacePtr);

		m_interfacePtr = interfacePtr;
	}

	void SetPtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
	{
		m_rootPtr.reset(rootPtr);

		m_interfacePtr = extractInterface();
	}

	void SetPtr(istd::IPolymorphic* rootPtr, InterfaceType* interfacePtr) noexcept
	{
		m_rootPtr.reset(rootPtr);

		m_interfacePtr = interfacePtr;
	}

	RootObjectPtr& GetBasePtr() noexcept
	{
		return m_rootPtr;
	}

	const RootObjectPtr& GetBasePtr() const noexcept
	{
		return m_rootPtr;
	}

	// STL-like support
	template <class T>
	T* dynamicCast() noexcept
	{
		return GetPtr<T>();
	}

	template <class T>
	const T* dynamicCast() const noexcept
	{
		return GetPtr<const T>();
	}

	explicit operator bool() const noexcept
	{
		return IsValid();
	}

	TInterfacePtr& operator=(std::nullptr_t) noexcept
	{
		Reset();

		return *this;
	}

protected:
	TInterfacePtr(const TInterfacePtr&) = delete;
	TInterfacePtr& operator=(const TInterfacePtr&) = delete;

	TInterfacePtr() noexcept
		:m_interfacePtr(nullptr)
	{
	}

	TInterfacePtr(InterfaceType* interfacePtr) noexcept
		:m_interfacePtr(interfacePtr)
	{
		m_rootPtr.reset(interfacePtr);
	}

	TInterfacePtr(std::nullptr_t) noexcept
		:m_rootPtr(),
		m_interfacePtr(nullptr)
	{
	}

	TInterfacePtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface) noexcept
	{
		SetPtr(rootPtr, extractInterface);
	}

	TInterfacePtr(istd::IPolymorphic* rootPtr, InterfaceType* interfacePtr) noexcept
	{
		SetPtr(rootPtr, interfacePtr);
	}

	// Move constructor
	TInterfacePtr(TInterfacePtr&& ptr) noexcept
		:m_rootPtr(std::move(ptr.m_rootPtr)),
		m_interfacePtr(std::exchange(ptr.m_interfacePtr, nullptr))
	{
	}

	// Move assignment
	TInterfacePtr& operator=(TInterfacePtr&& ptr) noexcept
	{
		if (this != &ptr){
			m_rootPtr = std::move(ptr.m_rootPtr);
			m_interfacePtr = std::exchange(ptr.m_interfacePtr, nullptr);
		}
		return *this;
	}

protected:
	RootObjectPtr m_rootPtr;
	InterfaceType* m_interfacePtr = nullptr;
};


template <class InterfaceType, class RootIntefaceType = istd::IPolymorphic>
class TUniqueInterfacePtr : public TInterfacePtr<InterfaceType, std::unique_ptr<RootIntefaceType>>
{
public:
	typedef TInterfacePtr<InterfaceType, std::unique_ptr<RootIntefaceType>> BaseClass;
	typedef typename BaseClass::ExtractInterfaceFunc ExtractInterfaceFunc;

	static_assert(std::is_base_of_v<istd::IPolymorphic, RootIntefaceType>,
		"RootIntefaceType must derive from istd::IPolymorphic");
	static_assert(std::has_virtual_destructor_v<RootIntefaceType>,
		"RootIntefaceType must have a virtual destructor");

	TUniqueInterfacePtr() noexcept
		:BaseClass()
	{
	}

	TUniqueInterfacePtr(InterfaceType* interfacePtr) noexcept
		:BaseClass(interfacePtr)
	{
	}

	template <typename T>
	TUniqueInterfacePtr(std::unique_ptr<T>&& ptr) noexcept
	{
		BaseClass::m_interfacePtr = ptr.get();

		BaseClass::m_rootPtr = std::move(ptr);
	}
	
	TUniqueInterfacePtr& operator=(std::nullptr_t) noexcept
	{
		BaseClass::Reset();

		return *this;
	}


	explicit TUniqueInterfacePtr(RootIntefaceType* rootPtr, const ExtractInterfaceFunc& extractInterface) noexcept
		:BaseClass(rootPtr, extractInterface)
	{
	}

	explicit TUniqueInterfacePtr(RootIntefaceType* rootPtr, InterfaceType* interfacePtr) noexcept
		:BaseClass(rootPtr, interfacePtr)
	{
	}

	TUniqueInterfacePtr(const TUniqueInterfacePtr& ptr) = delete;
	TUniqueInterfacePtr& operator=(const TUniqueInterfacePtr& ptr) = delete;

	// Move constructor
	TUniqueInterfacePtr(TUniqueInterfacePtr&& ptr) noexcept
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = std::exchange(ptr.m_interfacePtr, nullptr);
	}

	// Move assignment
	TUniqueInterfacePtr& operator=(TUniqueInterfacePtr&& ptr) noexcept
	{
		if (this != &ptr){
			BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
			BaseClass::m_interfacePtr = std::exchange(ptr.m_interfacePtr, nullptr);
		}
		return *this;
	}

	/**
		Pop the root pointer.Caller takes ownership of the raw pointer.
	*/
	RootIntefaceType* PopPtr() noexcept
	{
		BaseClass::m_interfacePtr = nullptr;

		return BaseClass::m_rootPtr.release(); // caller owns returned pointer
	}

	InterfaceType* PopInterfacePtr() noexcept
	{
		InterfaceType* retVal = BaseClass::m_interfacePtr;

		BaseClass::m_interfacePtr = nullptr;

		BaseClass::m_rootPtr.release();

		return retVal;
	}

	/**
		Transfer ownership from another unique ptr(take over raw ownership)
	*/
	void TakeOver(TUniqueInterfacePtr<InterfaceType>& from) noexcept
	{
		BaseClass::m_interfacePtr = from.m_interfacePtr;

		BaseClass::m_rootPtr.reset(from.PopPtr());
	}

	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source) noexcept
	{
		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = std::move(source.GetBasePtr());
			BaseClass::m_interfacePtr = targetPtr;
			source.Reset();
			return true;
		}
		return false;
	}

	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>&& source) noexcept
	{
		return MoveCastedPtr(source);
	}
};


template <class InterfaceType, class RootIntefaceType = istd::IPolymorphic>
class TSharedInterfacePtr : public TInterfacePtr<InterfaceType, std::shared_ptr<RootIntefaceType>>
{
public:
	typedef TInterfacePtr<InterfaceType, std::shared_ptr<RootIntefaceType>> BaseClass;
	typedef typename BaseClass::ExtractInterfaceFunc ExtractInterfaceFunc;

	static_assert(std::is_base_of_v<istd::IPolymorphic, RootIntefaceType>, "RootIntefaceType must derive from istd::IPolymorphic");
	static_assert(std::has_virtual_destructor_v<RootIntefaceType>, "RootIntefaceType must have a virtual destructor");

	TSharedInterfacePtr() noexcept
		:BaseClass()
	{
	}

	TSharedInterfacePtr(InterfaceType* interfacePtr) noexcept
		:BaseClass(interfacePtr)
	{
	}

	TSharedInterfacePtr(RootIntefaceType* rootPtr, const ExtractInterfaceFunc& extractInterface) noexcept
		:BaseClass(rootPtr, extractInterface)
	{
	}

	TSharedInterfacePtr(const TSharedInterfacePtr& ptr) noexcept
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
	}

	template <typename U>
	TSharedInterfacePtr(const TSharedInterfacePtr<U>& other) noexcept
	{
		BaseClass::m_rootPtr = other.GetBasePtr();
		BaseClass::m_interfacePtr = static_cast<InterfaceType*>(other.GetPtr());
	}

	explicit TSharedInterfacePtr(const std::shared_ptr<RootIntefaceType>& ptr) noexcept
	{
		BaseClass::m_rootPtr = ptr;
		BaseClass::m_interfacePtr = dynamic_cast<InterfaceType*>(ptr.get());
	}

	// Move constructor
	TSharedInterfacePtr(TSharedInterfacePtr&& ptr) noexcept
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = std::exchange(ptr.m_interfacePtr, nullptr);
	}

	/**
		Construct from unique by transferring ownership into shared_ptr.
	*/
	TSharedInterfacePtr(TUniqueInterfacePtr<InterfaceType>&& ptr) noexcept
	{
		*this = CreateFromUnique(ptr);
	}

	~TSharedInterfacePtr()
	{
		Reset();
	}

	void Reset() noexcept
	{
		BaseClass::m_rootPtr.reset();
		BaseClass::m_interfacePtr = nullptr;
	}

	TSharedInterfacePtr& operator=(const TSharedInterfacePtr& ptr) noexcept
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
		return *this;
	}

	TSharedInterfacePtr& operator=(const std::shared_ptr<istd::IPolymorphic>& ptr) noexcept
	{
		InterfaceType* interfacePtr = dynamic_cast<InterfaceType*>(ptr.get());
		if (interfacePtr != nullptr){
			BaseClass::m_rootPtr = ptr;

			BaseClass::m_interfacePtr = interfacePtr;
		}

		return *this;
	}

	template <typename U>
	TSharedInterfacePtr& operator=(const TSharedInterfacePtr<U>& ptr) noexcept
	{
		BaseClass::m_rootPtr = ptr.GetBasePtr();
		BaseClass::m_interfacePtr = ptr.GetPtr();
		return *this;
	}

	TSharedInterfacePtr& operator=(TSharedInterfacePtr&& ptr) noexcept
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = std::exchange(ptr.m_interfacePtr, nullptr);
		return *this;
	}

	// Convert from unique to shared. After this call, uniquePtr no longer owns the object.
	TSharedInterfacePtr& FromUnique(TUniqueInterfacePtr<InterfaceType>& uniquePtr) noexcept
	{
		if (!uniquePtr.IsValid()){
			Reset();
			return *this;
		}

		// Acquire raw pointer in one step and assign to shared_ptr to avoid leaks on exceptions.
		RootIntefaceType* rawRoot = uniquePtr.PopPtr(); // caller (this function) now owns rawRoot
		if (rawRoot == nullptr){
			Reset();
			return *this;
		}

		// Create shared_ptr from raw pointer (shared_ptr now owns it)
		BaseClass::m_rootPtr = std::shared_ptr<RootIntefaceType>(rawRoot);
		BaseClass::m_interfacePtr = dynamic_cast<InterfaceType*>(BaseClass::m_rootPtr.get());

		return *this;
	}

	TSharedInterfacePtr& FromUnique(TUniqueInterfacePtr<InterfaceType>&& uniquePtr) noexcept
	{
		return FromUnique(uniquePtr);
	}

	template <typename OtherInterface>
	static TSharedInterfacePtr CreateFromUnique(TUniqueInterfacePtr<OtherInterface>& uniquePtr) noexcept
	{
		TSharedInterfacePtr retVal;
		if (!uniquePtr.IsValid()){
			return retVal;
		}

		// Try dynamic cast on the raw pointer before transferring ownership
		InterfaceType* interfacePtr = dynamic_cast<InterfaceType*>(uniquePtr.GetPtr());
		if (interfacePtr != nullptr){
			RootIntefaceType* rawRoot = uniquePtr.PopPtr();
			retVal.BaseClass::m_rootPtr = std::shared_ptr<RootIntefaceType>(rawRoot);
			retVal.BaseClass::m_interfacePtr = interfacePtr;
		}

		return retVal;
	}

	static TSharedInterfacePtr CreateFromUnique(TUniqueInterfacePtr<InterfaceType>& uniquePtr) noexcept
	{
		TSharedInterfacePtr retVal;
		retVal.FromUnique(uniquePtr);
		return retVal;
	}

	static TSharedInterfacePtr CreateFromUnique(TUniqueInterfacePtr<InterfaceType>&& uniquePtr) noexcept
	{
		return CreateFromUnique(uniquePtr);
	}

	// Move-cast from unique: transfer ownership if dynamic_cast succeeds
	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source) noexcept
	{
		if (!source.IsValid()){
			Reset();
			return true;
		}

		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			// Transfer ownership of the unique_ptr into shared_ptr safely
			RootIntefaceType* rawRoot = source.PopPtr();
			BaseClass::m_rootPtr = std::shared_ptr<RootIntefaceType>(rawRoot);
			BaseClass::m_interfacePtr = targetPtr;
			return true;
		}

		return false;
	}

	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>&& source) noexcept
	{
		return MoveCastedPtr(source);
	}

	/**
		Set from another shared pointer if dynamic_cast succeeds.
	*/
	template<class SourceInterfaceType>
	bool SetCastedPtr(TSharedInterfacePtr<SourceInterfaceType>& source) noexcept
	{
		if (!source.IsValid()){
			Reset();
			return true;
		}

		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = source.GetBasePtr();
			BaseClass::m_interfacePtr = targetPtr;
			return true;
		}

		return false;
	}
};


} // namespace istd


