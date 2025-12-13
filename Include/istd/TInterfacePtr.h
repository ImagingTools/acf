#pragma once


// STL includes
#include <memory>
#include <utility>

// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{


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
	Interface* GetPtr()
	{
		if constexpr (std::is_same_v<Interface, InterfaceType>){
			return m_interfacePtr;
		}
		else{
			return dynamic_cast<Interface*>(m_interfacePtr);
		}
	}

	template<typename Interface = InterfaceType>
	const Interface* GetPtr() const
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

	void SetPtr(InterfaceType* interfacePtr)
	{
		m_rootPtr.reset(interfacePtr);

		m_interfacePtr = interfacePtr;
	}

	void SetPtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
	{
		m_rootPtr.reset(rootPtr);
		m_interfacePtr = extractInterface();
	}

	void SetPtr(istd::IPolymorphic* rootPtr, InterfaceType* interfacePtr)
	{
		m_rootPtr.reset(rootPtr);
		m_interfacePtr = interfacePtr;
	}

	RootObjectPtr& GetBasePtr()
	{
		return m_rootPtr;
	}

	const RootObjectPtr& GetBasePtr() const
	{
		return m_rootPtr;
	}

	// STL support
	template <class T>
	T* dynamicCast()
	{
		return GetPtr<T>();
	}

	template <class T>
	const T* dynamicCast() const
	{
		return GetPtr<const T>();
	}

	operator bool() const noexcept
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

	TInterfacePtr()
		:m_interfacePtr(nullptr)
	{
	}

	TInterfacePtr(InterfaceType* interfacePtr)
		:m_interfacePtr(interfacePtr)
	{
		m_rootPtr.reset(interfacePtr);
	}

	TInterfacePtr(std::nullptr_t) noexcept
		:m_rootPtr(),
		m_interfacePtr(nullptr)
	{
	}

	explicit TInterfacePtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
	{
		SetPtr(rootPtr, extractInterface);
	}

	explicit TInterfacePtr(istd::IPolymorphic* rootPtr, InterfaceType* interfacePtr)
	{
		SetPtr(rootPtr, interfacePtr);
	}

	 // Move constructor
	TInterfacePtr(TInterfacePtr&& ptr) noexcept
	{
		m_rootPtr = std::move(ptr.m_rootPtr);
		m_interfacePtr = ptr.m_interfacePtr;
		ptr.m_interfacePtr = nullptr;
	}

	// Move assignment
	TInterfacePtr& operator=(TInterfacePtr&& ptr) noexcept
	{
		if (this != &ptr){
			m_rootPtr = std::move(ptr.m_rootPtr);
			m_interfacePtr = ptr.m_interfacePtr;
			ptr.m_interfacePtr = nullptr;
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

	TUniqueInterfacePtr()
		:BaseClass()
	{
	}


	TUniqueInterfacePtr(InterfaceType* interfacePtr)
		:BaseClass(interfacePtr)
	{
	}

	TUniqueInterfacePtr& operator=(std::nullptr_t) noexcept
	{
		BaseClass::Reset();
	
		return *this;
	}

	explicit TUniqueInterfacePtr(RootIntefaceType* rootPtr, const ExtractInterfaceFunc& extractInterface)
		:BaseClass(rootPtr, extractInterface)
	{
	}

	explicit TUniqueInterfacePtr(RootIntefaceType* rootPtr, InterfaceType* interfacePtr)
		:BaseClass(rootPtr, interfacePtr)
	{
	}

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TUniqueInterfacePtr(const TUniqueInterfacePtr& ptr) = delete;

	/**
		Move constructor.
	*/
	TUniqueInterfacePtr(TUniqueInterfacePtr&& ptr) noexcept
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
		ptr.m_interfacePtr = nullptr;
	}

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TUniqueInterfacePtr& operator=(const TUniqueInterfacePtr& ptr) = delete;

	/**
		Move operator.
	*/
	TUniqueInterfacePtr& operator=(TUniqueInterfacePtr&& ptr)
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
		ptr.m_interfacePtr = nullptr;

		return *this;
	}


	istd::IPolymorphic* PopPtr() noexcept
	{
		BaseClass::m_interfacePtr = nullptr;

		return BaseClass::m_rootPtr.release();
	}


	InterfaceType* PopInterfacePtr() noexcept
	{
		InterfaceType* retVal = BaseClass::m_interfacePtr;

		BaseClass::m_interfacePtr = nullptr;

		BaseClass::m_rootPtr.release();

		return retVal;
	}


	void TakeOver(TUniqueInterfacePtr<InterfaceType>& from)
	{
		BaseClass::m_interfacePtr = from.m_interfacePtr;

		BaseClass::m_rootPtr.reset(from.PopPtr());
	}

	
	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source)
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
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>&& source)
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
};


template <class InterfaceType, class RootIntefaceType = istd::IPolymorphic>
class TSharedInterfacePtr : public TInterfacePtr<InterfaceType, std::shared_ptr<RootIntefaceType>>
{
public:
	typedef TInterfacePtr<InterfaceType, std::shared_ptr<RootIntefaceType>> BaseClass;
	typedef typename BaseClass::ExtractInterfaceFunc ExtractInterfaceFunc;

	TSharedInterfacePtr()
		:BaseClass()
	{
	}

	~TSharedInterfacePtr()
	{
		Reset();
	}

	void Reset()
	{
		BaseClass::m_rootPtr.reset();

		BaseClass::m_interfacePtr = nullptr;
	}


	TSharedInterfacePtr(InterfaceType* interfacePtr)
		:BaseClass(interfacePtr)
	{
	}

	TSharedInterfacePtr(RootIntefaceType* rootPtr, const ExtractInterfaceFunc& extractInterface)
		:BaseClass(rootPtr, extractInterface)
	{
	}

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TSharedInterfacePtr(const TSharedInterfacePtr& ptr)
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
	}

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TSharedInterfacePtr(const std::shared_ptr<RootIntefaceType>& ptr)
	{
		BaseClass::m_rootPtr = ptr;
		BaseClass::m_interfacePtr = dynamic_cast<InterfaceType*>(ptr.get());
	}

	/**
		Move constructor.
	*/
	TSharedInterfacePtr(TSharedInterfacePtr&& ptr)
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
	}

	TSharedInterfacePtr(TUniqueInterfacePtr<InterfaceType>&& ptr)
	{
		*this = CreateFromUnique(ptr);
	}

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TSharedInterfacePtr& operator=(const TSharedInterfacePtr& ptr)
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;

		return *this;
	}

	/**
		Move operator.
	*/
	TSharedInterfacePtr& operator=(TSharedInterfacePtr&& ptr)
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;

		return *this;
	}

	TSharedInterfacePtr<InterfaceType>& FromUnique(TUniqueInterfacePtr<InterfaceType>& uniquePtr)
	{
		BaseClass::SetPtr(uniquePtr.GetBasePtr().get(), uniquePtr.GetPtr());

		uniquePtr.PopPtr();

		return *this;
	}

	TSharedInterfacePtr<InterfaceType>& FromUnique(TUniqueInterfacePtr<InterfaceType>&& uniquePtr)
	{
		BaseClass::SetPtr(uniquePtr.GetBasePtr().get(), uniquePtr.GetPtr());

		uniquePtr.PopPtr();

		return *this;
	}

	template <typename OtherInterface>
	static TSharedInterfacePtr<InterfaceType> CreateFromUnique(TUniqueInterfacePtr<OtherInterface>& uniquePtr)
	{
		TSharedInterfacePtr<InterfaceType> retVal;

		InterfaceType* interfacePtr = dynamic_cast<InterfaceType*>(uniquePtr.GetPtr());
		if (interfacePtr != nullptr){
			retVal.SetPtr(uniquePtr.GetBasePtr().get(), interfacePtr);

			uniquePtr.PopPtr();
		}

		return retVal;
	}


	static TSharedInterfacePtr<InterfaceType> CreateFromUnique(TUniqueInterfacePtr<InterfaceType>& uniquePtr)
	{
		TSharedInterfacePtr<InterfaceType> retVal;

		retVal.FromUnique(uniquePtr);

		return retVal;
	}

	static TSharedInterfacePtr<InterfaceType> CreateFromUnique(TUniqueInterfacePtr<InterfaceType>&& uniquePtr)
	{
		TSharedInterfacePtr<InterfaceType> retVal;

		retVal.FromUnique(uniquePtr);

		return retVal;
	}

	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source)
	{
		if (!source.IsValid()){
			Reset();

			return true;
		}

		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = std::move(source.GetBasePtr());

			BaseClass::m_interfacePtr = targetPtr;

			return true;
		}

		return false;
	}

	template<class SourceInterfaceType>
	bool MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>&& source)
	{
		if (!source.IsValid()){
			Reset();

			return true;
		}
		
		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = std::move(source.GetBasePtr());

			BaseClass::m_interfacePtr = targetPtr;

			return true;
		}

		return false;
	}


	template<class SourceInterfaceType>
	bool SetCastedPtr(TSharedInterfacePtr<SourceInterfaceType>& source)
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


template <class InterfaceType, class ImplPtr>
class TInterfaceAggregator: virtual public InterfaceType
{
public:
	TInterfaceAggregator(ImplPtr& implPtr)
		:m_implPtr(std::move(implPtr))
	{
	}

	TInterfaceAggregator(ImplPtr&& implPtr)
		:m_implPtr(std::move(implPtr))
	{
	}

	bool IsValid() const
	{
		return m_implPtr.IsValid();
	}

	const InterfaceType* operator->() const
	{
		Q_ASSERT(m_implPtr.IsValid());

		return dynamic_cast<const InterfaceType*>(m_implPtr.operator->());
	}

	InterfaceType* operator->()
	{
		Q_ASSERT(m_implPtr.IsValid());

		return dynamic_cast<InterfaceType*>(m_implPtr.operator->());
	}

	const InterfaceType& operator*() const
	{
		Q_ASSERT(m_implPtr.IsValid());

		return *dynamic_cast<const InterfaceType*>(m_implPtr.operator->());
	}

	InterfaceType& operator*()
	{
		Q_ASSERT(m_implPtr.IsValid());

		return *dynamic_cast<InterfaceType*>(m_implPtr.operator->());
	}

protected:
	ImplPtr m_implPtr;
};


} // namespace istd


