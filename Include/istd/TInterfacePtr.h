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

	bool IsValid() const
	{
		return m_interfacePtr != nullptr;
	}

	InterfaceType* GetPtr() const
	{
		return m_interfacePtr;
	}

	const InterfaceType* operator->() const
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return m_interfacePtr;
	}

	InterfaceType* operator->()
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return const_cast<InterfaceType*>(m_interfacePtr);
	}

	const InterfaceType& operator*() const
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return *m_interfacePtr;
	}

	InterfaceType& operator*()
	{
		Q_ASSERT(m_interfacePtr != nullptr);

		return *m_interfacePtr;
	}

	void Reset()
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

protected:
	TInterfacePtr()
		:m_interfacePtr(nullptr)
	{
	}

	TInterfacePtr(InterfaceType* interfacePtr)
		:m_interfacePtr(interfacePtr)
	{
		m_rootPtr.reset(interfacePtr);
	}

	TInterfacePtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
	{
		SetPtr(rootPtr, extractInterface);
	}

	/**
		Copy constructor.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TInterfacePtr(const TInterfacePtr& ptr)
	{
		I_IF_DEBUG(Q_ASSERT(ptr.GetPtr() == NULL));
	}

	/**
		Move constructor.
	*/
	TInterfacePtr(TInterfacePtr&& ptr)
	{
		m_rootPtr = std::move(ptr.m_rootPtr);

		m_interfacePtr = ptr.m_interfacePtr;
	}

	/**
		Assign operator.
		This implementation has no function and is provided only for compatibility with STL.
		The source pointer must be invalid (NULL).
	*/
	TInterfacePtr& operator=(const TInterfacePtr& ptr)
	{
		I_IF_DEBUG(Q_ASSERT(ptr.GetPtr() == NULL));

		Reset();

		return *this;
	}

	/**
		Move operator.
	*/
	TInterfacePtr& operator=(TInterfacePtr&& ptr)
	{
		m_rootPtr = std::move(ptr.m_rootPtr);

		m_interfacePtr = ptr.m_interfacePtr;

		return *this;
	}

protected:
	RootObjectPtr m_rootPtr;
	InterfaceType* m_interfacePtr = nullptr;
};


template <class InterfaceType>
class TUniqueInterfacePtr : public TInterfacePtr<InterfaceType, std::unique_ptr<istd::IPolymorphic>>
{
public:
	typedef TInterfacePtr<InterfaceType, std::unique_ptr<istd::IPolymorphic>> BaseClass;
	typedef typename BaseClass::ExtractInterfaceFunc ExtractInterfaceFunc;

	TUniqueInterfacePtr()
		:BaseClass()
	{
	}


	TUniqueInterfacePtr(InterfaceType* interfacePtr)
		:BaseClass(interfacePtr)
	{
	}

	TUniqueInterfacePtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
		:BaseClass(rootPtr, extractInterface)
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
	TUniqueInterfacePtr(TUniqueInterfacePtr&& ptr)
	{
		BaseClass::m_rootPtr = std::move(ptr.m_rootPtr);
		BaseClass::m_interfacePtr = ptr.m_interfacePtr;
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

		return *this;
	}


	istd::IPolymorphic* PopPtr()
	{
		BaseClass::m_interfacePtr = nullptr;

		return BaseClass::m_rootPtr.release();
	}

	InterfaceType* PopInterfacePtr()
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
	void MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source)
	{
		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = std::move(source.GetBasePtr());

			BaseClass::m_interfacePtr = targetPtr;
		}
	}
};


template <class InterfaceType>
class TSharedInterfacePtr : public TInterfacePtr<InterfaceType, std::shared_ptr<istd::IPolymorphic>>
{
public:
	typedef TInterfacePtr<InterfaceType, std::shared_ptr<istd::IPolymorphic>> BaseClass;
	typedef typename BaseClass::ExtractInterfaceFunc ExtractInterfaceFunc;

	TSharedInterfacePtr()
		:BaseClass()
	{
	}


	TSharedInterfacePtr(InterfaceType* interfacePtr)
		:BaseClass(interfacePtr)
	{
	}

	TSharedInterfacePtr(istd::IPolymorphic* rootPtr, const ExtractInterfaceFunc& extractInterface)
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
		Move constructor.
	*/
	TSharedInterfacePtr(TSharedInterfacePtr&& ptr)
	{
		BaseClass::m_rootPtr = ptr.m_rootPtr;
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
	void MoveCastedPtr(TUniqueInterfacePtr<SourceInterfaceType>& source)
	{
		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = std::move(source.GetBasePtr());

			BaseClass::m_interfacePtr = targetPtr;
		}
	}

	template<class SourceInterfaceType>
	void SetCastedPtr(TSharedInterfacePtr<SourceInterfaceType>& source)
	{
		InterfaceType* targetPtr = dynamic_cast<InterfaceType*>(source.GetPtr());
		if (targetPtr != nullptr){
			BaseClass::m_rootPtr = source.GetBasePtr();

			BaseClass::m_interfacePtr = targetPtr;
		}
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


