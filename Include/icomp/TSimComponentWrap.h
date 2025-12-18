#pragma once


// ACF includes
#include <istd/CClassInfo.h>
#include <icomp/ICompositeComponent.h>
#include <icomp/TComponentWrap.h>
#include <icomp/CSimComponentContextBase.h>


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.

	\ingroup ComponentConcept
*/
template <class Base>
class TSimComponentWrap:
			public TComponentWrap<Base>,
			virtual public ICompositeComponent
{
public:
	typedef TComponentWrap<Base> BaseClass;

	TSimComponentWrap();

	~TSimComponentWrap()
	{
		m_contextPtr.reset();
	}

	/**
		Initialilze component after setting all its attributes and references.
	*/
	void InitComponent();

	/**
		Set named attribute.
		\param	attributeId		ID of attribute.
		\param	attributePtr	pointer to attribute instance. It will be automatically deleted.
	*/
	bool SetAttr(const QByteArray& attributeId, const iser::IObject* attributePtr)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetAttr(attributeId, attributePtr);
	}

	/**
		Set named reference to some component.
	*/
	bool SetRef(const QByteArray& referenceId, IComponentSharedPtr componentPtr, const QByteArray& subelementId = "")
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetRef(referenceId, componentPtr, subelementId);
	}

	/**
		Set named reference to some component.
	*/
	bool InsertMultiRef(const QByteArray& referenceId, IComponentSharedPtr componentPtr, const QByteArray& subelementId = "")
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->InsertMultiRef(referenceId, componentPtr, subelementId);
	}

	/**
		Set factory of component instance.
	*/
	bool SetFactory(const QByteArray& factoryId, const CSimComponentContextBase::ComponentsFactory* factoryPtr)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetFactory(factoryId, factoryPtr);
	}

	/**
		Insert new factory instance into multi-factory attribute.
	*/
	bool InsertMultiFactory(const QByteArray& factoryId, const CSimComponentContextBase::ComponentsFactory* factoryPtr)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->InsertMultiFactory(factoryId, factoryPtr);
	}

	/**
		Set instance of \c bool attribute.
	*/
	bool SetBoolAttr(const QByteArray& attributeId, bool value)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetBoolAttr(attributeId, value);
	}

	/**
		Set instance of \c int attribute.
	*/
	bool SetIntAttr(const QByteArray& attributeId, int value)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetIntAttr(attributeId, value);
	}

	/**
		Set instance of \c double attribute.
	*/
	bool SetDoubleAttr(const QByteArray& attributeId, double value)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetDoubleAttr(attributeId, value);
	}

	/**
		Set instance of \c QString attribute.
	*/
	bool SetStringAttr(const QByteArray& attributeId, const QString& value)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetStringAttr(attributeId, value);
	}

	/**
		Set instance of \c QByteArray attribute.
	*/
	bool SetIdAttr(const QByteArray& attributeId, const QByteArray& value)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->SetIdAttr(attributeId, value);
	}

	/**
		Insert new attribute to multi attributes.
		\param	attributeId		ID of attribute (multi attribute).
		\param	attribute		single attribute value.
	*/
	template <class Attribute>
	bool InsertMultiAttr(const QByteArray& attributeId, const Attribute& attribute)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->InsertMultiAttr(attributeId, attribute);
	}

	bool InsertMultiAttr(const QByteArray& attributeId, const QString& attribute)
	{
		SimulationContext* implPtr = dynamic_cast<SimulationContext*>(m_contextPtr.get());

		return implPtr->InsertMultiAttr(attributeId, attribute);
	}


	// reimplemented (icomp::ICompositeComponent)
	virtual IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const override;
	virtual IComponentContextSharedPtr GetSubcomponentContext(const QByteArray& componentId) const override;
	virtual IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const override;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) override;

	// reimplemented (icomp::IComponent)
	virtual const IComponent* GetParentComponent(bool ownerOnly = false) const override;

protected:
	class SimulationContext : public CSimComponentContextBase
	{
	public:
		explicit SimulationContext(const IComponentStaticInfo* infoPtr)
			:CSimComponentContextBase(infoPtr)
		{
		}
	
		~SimulationContext()
		{
		}

		// reimplemented (icomp::IComponentContext)
		virtual const QByteArray& GetContextId() const override
		{
			static QByteArray id = istd::CClassInfo::GetName<Base>();

			return id;
		}

		IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const
		{
			ComponentsMap::ConstIterator iter = m_componentsMap.constFind(componentId);
			if (iter != m_componentsMap.constEnd()){
				return iter.value();
			}

			return IComponentSharedPtr();
		}

		IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const
		{
			FactoriesMap::ConstIterator iter = m_factoriesMap.constFind(componentId);
			if (iter != m_factoriesMap.constEnd()){
				Q_ASSERT(iter.value() != nullptr);

				return IComponentUniquePtr(iter.value()->CreateInstance());
			}

			return nullptr;
		}
	};

	IComponentContextSharedPtr m_contextPtr;
	IComponentSharedPtr m_componentPtr;
};


// public methods

template <class Base>
TSimComponentWrap<Base>::TSimComponentWrap()
	:BaseClass()
{
	m_contextPtr.reset(new SimulationContext(&BaseClass::GetComponentStaticInfo()));
}


template <class Base>
void TSimComponentWrap<Base>::InitComponent()
{
	SetComponentContext(m_contextPtr, NULL, false);
}


// reimplemented (icomp::ICompositeComponent)

template <class Base>
IComponentSharedPtr TSimComponentWrap<Base>::GetSubcomponent(const QByteArray& componentId) const
{
	const SimulationContext* implPtr = dynamic_cast<const SimulationContext*>(m_contextPtr.get());

	return implPtr->GetSubcomponent(componentId);
}


template <class Base>
icomp::IComponentContextSharedPtr TSimComponentWrap<Base>::GetSubcomponentContext(const QByteArray& /*componentId*/) const
{
	return IComponentContextSharedPtr();
}


template <class Base>
IComponentUniquePtr TSimComponentWrap<Base>::CreateSubcomponent(const QByteArray& componentId) const
{
	const SimulationContext* implPtr = dynamic_cast<const SimulationContext*>(m_contextPtr.get());

	return implPtr->CreateSubcomponent(componentId);
}


template <class Base>
void TSimComponentWrap<Base>::OnSubcomponentDeleted(const IComponent* /*subcomponentPtr*/)
{
	qFatal("Simulated component cannot have sub-components");
}


// reimplemented (icomp::IComponent)

template <class Base>
const IComponent* TSimComponentWrap<Base>::GetParentComponent(bool ownerOnly) const
{
	if (ownerOnly){
		return NULL;
	}
	else{
		return const_cast<TSimComponentWrap<Base>* >(this);
	}
}


template <class Base>
class TSimSharedComponentPtr: public IComponentSharedPtr
{
public:
	typedef icomp::TSimComponentWrap<Base> Impl;

	TSimSharedComponentPtr()
	{
		Impl* ptr = new Impl;

		reset(ptr);
	}

	const Impl& GetImpl() const
	{
		return *dynamic_cast<Impl*>(get());
	}

	Impl& GetImpl()
	{
		return *dynamic_cast<Impl*>(get());
	}

	Impl* operator->()
	{
		return dynamic_cast<Impl*>(get());
	}

	Impl& operator *()
	{
		return *dynamic_cast<Impl*>(get());
	}
};


} // namespace icomp


