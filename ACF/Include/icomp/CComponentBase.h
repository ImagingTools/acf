#ifndef icomp_CComponentBase_included
#define icomp_CComponentBase_included


#include "icomp/IComponent.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TInterfaceRegistrator.h"
#include "icomp/TAttributePtr.h"
#include "icomp/TAttributeStaticInfo.h"
#include "icomp/TReferencePtr.h"
//#include "icomp/TMultiReferencePtr.h"
//#include "icomp/TFactoryPtr.h"
//#include "icomp/TMultiFactoryPtr.h"
#include "icomp/TComponentStaticInfo.h"



namespace icomp
{


class CComponentBase: virtual public IComponent
{
public:
	/**
		Create component and assign it to specific context.
		\param	contextPtr	pointer to context of this component. It cannot be NULL.
	*/
	CComponentBase();

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const type_info& interfaceType, const ::std::string& subId = "");
	virtual const IComponentContext* GetComponentContext() const;
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// static methods
	static icomp::IComponentStaticInfo& InitStaticInfo(CComponentBase* componentPtr);

protected:
	// reimplemented (icomp::IComponent)
	virtual void SetComponentContext(const icomp::IComponentContext* contextPtr);

private:
	const IComponentContext* m_contextPtr;
};


}//namespace icomp


/**
	General ACF component declaration.
	It should be placed in class declaration as first line.
*/
#define I_BEGIN_COMPONENT(ComponentType)\
	static const icomp::IComponentStaticInfo& InitStaticInfo(ComponentType* componentPtr)\
	{\
		static icomp::TComponentStaticInfo<ComponentType> staticInfo(&BaseClass::InitStaticInfo(componentPtr));

#define I_END_COMPONENT\
		return staticInfo;\
	}

#define I_REGISTER_INTERFACE(InterfaceType)\
	static icomp::TInterfaceRegistrator<InterfaceType> staticRegistrator##__LINE__(staticInfo);

#define I_ATTR(attrType, member)\
	typedef attrType member##_Type;\
	icomp::TAttributePtr<attrType> member;

#define I_REF(interfaceType, member)\
	typedef icomp::TReferencePtr<interfaceType>::AttributeType member##_Type;\
	icomp::TReferencePtr<interfaceType> member;

#define I_MULTIREF(interfaceType, member)\
	typedef icomp::TMultiReferencePtr<interfaceType>::AttributeType member##_Type;\
	icomp::TMultiReferencePtr<interfaceType> member;

#define I_FACT(interfaceType, member)\
	typedef icomp::TFactoryPtr<interfaceType>::AttributeType member##_Type;\
	icomp::TFactoryPtr<interfaceType> member;

#define I_MULTIFACT(interfaceType, member)\
	typedef icomp::TMultiFactoryPtr<interfaceType>::AttributeType member##_Type;\
	icomp::TMultiFactoryPtr<interfaceType> member;

#define I_ASSIGN(member, id, description, defaultValue, isObligatory)\
	static member##_Type member##_Def(defaultValue);\
	static icomp::TAttributeStaticInfo<member##_Type> member##_Info(staticInfo, id, description, &member##_Def, isObligatory);\
	if (componentPtr != NULL){\
		componentPtr->member.Init(componentPtr, member##_Info);\
	}


#endif // !icomp_CComponentBase_included


