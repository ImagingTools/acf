#pragma once


// ACF includes
#include <icomp/IComponent.h>
#include <icomp/CInterfaceManipBase.h>


template <class InterfaceType>
class TSharedCompPtr: protected CInterfaceManipBase
{
public:
	TSharedCompPtr(const TCompPtr& source)
		:m_ptr(source.ptr)
	{
	}

	TSharedCompPtr(icomp::IComponent* ptr)
	{
		m_ptr.SetPtr(ptr, ExtractIntefrace<InterfaceType>(ptr));
	}
	
	bool IsValid() const
	{
		return m_ptr.IsValid();
	}

	const InterfaceType* operator->() const
	{
		Q_ASSERT(m_ptr.IsValid());

		return m_ptr.operator->();
	}

	InterfaceType* operator->()
	{
		Q_ASSERT(m_ptr.IsValid());

		return m_ptr.operator->();
	}

	const InterfaceType& operator*() const
	{
		Q_ASSERT(m_ptr.IsValid());

		return *m_ptr;
	}

	InterfaceType& operator*()
	{
		Q_ASSERT(m_ptr.IsValid());

		return *m_ptr;
	}

private:
	typedef istd::TSharedInterfacePtr<InterfaceType, icomp::IComponent> PtrImpl;
	PtrImpl m_ptr;
};


} // namespace icomp


