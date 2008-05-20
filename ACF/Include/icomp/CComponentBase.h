#ifndef icomp_CComponentBase_included
#define icomp_CComponentBase_included


#include "icomp/IComponent.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TInterfaceRegistrator.h"
#include "icomp/TSingleAttribute.h"
#include "icomp/TMultiAttribute.h"
#include "icomp/TSingleAttributePtr.h"
#include "icomp/TMultiAttributePtr.h"
#include "icomp/TReferencePtr.h"
#include "icomp/TMultiReferencePtr.h"
#include "icomp/TFactoryMember.h"
#include "icomp/TMultiFactoryMember.h"
#include "icomp/TAttributeStaticInfo.h"
#include "icomp/TComponentStaticInfo.h"
#include "icomp/TBaseComponentStaticInfo.h"



namespace icomp
{


/**
	Base class for component implementation.
	This class is compatible with the macros defined in this file \c (CComponentBase.h).
*/
class CComponentBase: virtual public IComponent
{
public:
	/**
		Create component and assign it to specific context.
		\param	contextPtr	pointer to context of this component. It cannot be NULL.
	*/
	CComponentBase();

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const type_info& interfaceType, const std::string& subId = "");
	virtual const IComponentContext* GetComponentContext() const;
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();
	virtual void SetComponentContext(const icomp::IComponentContext* contextPtr);

	// static methods
	static const icomp::IComponentStaticInfo& InitStaticInfo(CComponentBase* componentPtr);

private:
	const IComponentContext* m_contextPtr;
};


}//namespace icomp


/**
	Begin of general ACF component declaration.
	It should be placed in class declaration as first line.
*/
#define I_BEGIN_COMPONENT(ComponentType)\
	static const icomp::IComponentStaticInfo& InitStaticInfo(ComponentType* componentPtr)\
	{\
		static icomp::TComponentStaticInfo<ComponentType> staticInfo(&BaseClass::InitStaticInfo(NULL));\
		static bool isStaticInitialized = false;\
		if (isStaticInitialized && (componentPtr == NULL)){\
			return staticInfo;\
		}\
		isStaticInitialized = true;\
		BaseClass::InitStaticInfo(componentPtr);

/**
	Begin of ACF component declaration for non-instantiable components.
	It should be placed in class declaration in 'public' block.
*/
#define I_BEGIN_BASE_COMPONENT(ComponentType)\
	static const icomp::IComponentStaticInfo& InitStaticInfo(ComponentType* componentPtr)\
	{\
		BaseClass::InitStaticInfo(componentPtr);\
		static icomp::TBaseComponentStaticInfo<ComponentType> staticInfo(&BaseClass::InitStaticInfo(NULL));\
		static bool isStaticInitialized = false;\
		if (isStaticInitialized && (componentPtr == NULL)){\
			return staticInfo;\
		}\
		isStaticInitialized = true;\
		BaseClass::InitStaticInfo(componentPtr);

/**
	End of general ACF component declaration.
	It should be placed in class declaration as first line.
*/
#define I_END_COMPONENT\
		return staticInfo;\
	}

/**
	Declaration of supported interfaces.
*/
#define I_REGISTER_INTERFACE(InterfaceType)\
	static icomp::TInterfaceRegistrator<InterfaceType> AUTO_VARNAME(staticRegistrator)(staticInfo);

/**
	Declare user type attribute member.
*/
#define I_USER_ATTR(attrType, member)\
	typedef icomp::TSingleAttributePtr< attrType > member##_Type;\
	typedef icomp::TSingleAttributePtr< attrType >::AttributeType member##_AttrType;\
	icomp::TSingleAttributePtr< attrType > member;

/**
	Declare simple single attribute member.
*/
#define I_ATTR(attrType, member)\
	I_USER_ATTR(icomp::TSingleAttribute< attrType >, member)

/**
	Declare multiple user type attribute member.
*/
#define I_USER_MULTIATTR(attrType, member)\
	typedef icomp::TMultiAttributePtr< attrType > member##_Type;\
	typedef icomp::TMultiAttributePtr< attrType >::AttributeType member##_AttrType;\
	icomp::TMultiAttributePtr< attrType > member;

/**
	Declare simple multiple attribute member.
*/
#define I_MULTIATTR(attrType, member)\
	I_USER_MULTIATTR(icomp::TMultiAttribute<attrType>, member)

/**
	Declare single reference to another component.
*/
#define I_REF(interfaceType, member)\
	typedef icomp::TReferencePtr<interfaceType> member##_Type;\
	typedef icomp::TReferencePtr<interfaceType>::AttributeType member##_AttrType;\
	icomp::TReferencePtr<interfaceType> member;

/**
	Declare multiple reference to another component.
*/
#define I_MULTIREF(interfaceType, member)\
	typedef icomp::TMultiReferencePtr<interfaceType> member##_Type;\
	typedef icomp::TMultiReferencePtr<interfaceType>::AttributeType member##_AttrType;\
	icomp::TMultiReferencePtr<interfaceType> member;

/**
	Declare single factory of components.
*/
#define I_FACT(interfaceType, member)\
	typedef icomp::TFactoryMember<interfaceType> member##_Type;\
	typedef icomp::TFactoryMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TFactoryMember<interfaceType> member;

/**
	Declare multiple factory of components.
*/
#define I_MULTIFACT(interfaceType, member)\
	typedef icomp::TMultiFactoryMember<interfaceType> member##_Type;\
	typedef icomp::TMultiFactoryMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TMultiFactoryMember<interfaceType> member;

/**
	Used to assign value for single parameter (attribute, reference or factory).
*/
#define I_ASSIGN_BASE(member, id, description, isObligatory)\
	static icomp::TAttributeStaticInfo<member##_AttrType> member##_Info(staticInfo, id, description, &member##_Default, isObligatory, &typeid(member##_Type::InterfaceType));\
	if (componentPtr != NULL){\
		componentPtr->member.Init(componentPtr, member##_Info);\
	}

/**
	Used to assign value for single parameter (attribute, reference or factory).
*/
#define I_ASSIGN(member, id, description, isObligatory, defaultValue)\
	static member##_AttrType member##_Default(defaultValue);\
	I_ASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters (attributes, references or factories).
*/
#define I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)\
	static member##_AttrType member##_Default(sizeof(member##_DefaultElements) / sizeof(member##_AttrType::ValueType), member##_DefaultElements);\
	I_ASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters (attributes, references or factories) with no default parameter.
*/
#define I_ASSIGN_MULTI_0(member, id, description, isObligatory)\
	static member##_AttrType member##_Default(0, NULL);\
	I_ASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters (attributes, references or factories) with 1 default parameter.
*/
#define I_ASSIGN_MULTI_1(member, id, description, isObligatory, defaultValue1)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1};\
	I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)

/**
	Used to assign value for multiply parameters (attributes, references or factories) with 2 default parameter.
*/
#define I_ASSIGN_MULTI_2(member, id, description, isObligatory, defaultValue1, defaultValue2)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2};\
	I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)

/**
	Used to assign value for multiply parameters (attributes, references or factories) with 3 default parameter.
*/
#define I_ASSIGN_MULTI_3(member, id, description, isObligatory, defaultValue1, defaultValue2, defaultValue3)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2, defaultValue3};\
	I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)


#endif // !icomp_CComponentBase_included


