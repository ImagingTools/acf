#ifndef icomp_CComponentBase_included
#define icomp_CComponentBase_included


#include "istd/CClassInfo.h"

#include "icomp/IComponent.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TInterfaceRegistrator.h"
#include "icomp/TAttribute.h"
#include "icomp/TMultiAttribute.h"
#include "icomp/TAttributeMember.h"
#include "icomp/TMultiAttributeMember.h"
#include "icomp/TReferenceMember.h"
#include "icomp/TMultiReferenceMember.h"
#include "icomp/TFactoryMember.h"
#include "icomp/TMultiFactoryMember.h"
#include "icomp/TAttributeStaticInfo.h"
#include "icomp/TComponentStaticInfo.h"
#include "icomp/CBaseComponentStaticInfo.h"



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
	*/
	CComponentBase();

	// reimplemented (icomp::IComponent)
	virtual const IComponent* GetParentComponent(bool ownerOnly = false) const;
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const std::string& subId = "");
	virtual const IComponentContext* GetComponentContext() const;
	virtual void SetComponentContext(
				const icomp::IComponentContext* contextPtr,
				const IComponent* parentPtr,
				bool isParentOwner);
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual const IComponentContext* GetSubcomponentContext(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr);

	// static methods
	static const icomp::IComponentStaticInfo& InitStaticInfo(CComponentBase* componentPtr);

protected:
	/**
		Check if component is active.
		Component is active between \c OnComponentCreated and \c OnComponentDestroyed.
	*/
	bool IsComponentActive() const;

private:
	const IComponentContext* m_contextPtr;
	const IComponent* m_parentPtr;
	bool m_isParentOwner;
};


// inline protected methods

inline bool CComponentBase::IsComponentActive() const
{
	return (m_contextPtr != NULL);
}


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
		static icomp::CBaseComponentStaticInfo staticInfo(&BaseClass::InitStaticInfo(NULL));\
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
	typedef icomp::TAttributeMember< attrType > member##_Type;\
	typedef icomp::TAttributeMember< attrType >::AttributeType member##_AttrType;\
	icomp::TAttributeMember< attrType > member;

/**
	Declare simple single attribute member.
*/
#define I_ATTR(attrType, member)\
	I_USER_ATTR(icomp::TAttribute< attrType >, member)

/**
	Declare multiple user type attribute member.
*/
#define I_USER_MULTIATTR(attrType, member)\
	typedef icomp::TMultiAttributeMember< attrType > member##_Type;\
	typedef icomp::TMultiAttributeMember< attrType >::AttributeType member##_AttrType;\
	icomp::TMultiAttributeMember< attrType > member;

/**
	Declare simple multiple attribute member.
*/
#define I_MULTIATTR(attrType, member)\
	I_USER_MULTIATTR(icomp::TMultiAttribute<attrType>, member)

/**
	Declare single reference to another component.
*/
#define I_REF(interfaceType, member)\
	typedef icomp::TReferenceMember<interfaceType> member##_Type;\
	typedef icomp::TReferenceMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TReferenceMember<interfaceType> member;

/**
	Declare single reference to another component with \c interfaceType as template argument.
*/
#define I_TREF(interfaceType, member)\
	typedef icomp::TReferenceMember<interfaceType> member##_Type;\
	typedef typename icomp::TReferenceMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TReferenceMember<interfaceType> member;

/**
	Declare multiple reference to another component with \c interfaceType as template argument.
*/
#define I_TMULTIREF(interfaceType, member)\
	typedef icomp::TMultiReferenceMember<interfaceType> member##_Type;\
	typedef typename icomp::TMultiReferenceMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TMultiReferenceMember<interfaceType> member;


/**
	Declare multiple reference to another component.
*/
#define I_MULTIREF(interfaceType, member)\
	typedef icomp::TMultiReferenceMember<interfaceType> member##_Type;\
	typedef icomp::TMultiReferenceMember<interfaceType>::AttributeType member##_AttrType;\
	icomp::TMultiReferenceMember<interfaceType> member;

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
	static icomp::TAttributeStaticInfo<member##_AttrType> member##_Info(staticInfo, id, description, &member##_Default, isObligatory, istd::CClassInfo::GetInfo<member##_Type::InterfaceType>());\
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
	static member##_AttrType member##_Default(int(sizeof(member##_DefaultElements) / sizeof(member##_AttrType::ValueType)), member##_DefaultElements);\
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

/**
	Used to assign value for multiply parameters (attributes, references or factories) with 3 default parameter.
*/
#define I_ASSIGN_MULTI_4(member, id, description, isObligatory, defaultValue1, defaultValue2, defaultValue3, defaultValue4)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2, defaultValue3, defaultValue4};\
	I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)

/**
	Used to assign value for multiply parameters (attributes, references or factories) with 3 default parameter.
*/
#define I_ASSIGN_MULTI_5(member, id, description, isObligatory, defaultValue1, defaultValue2, defaultValue3, defaultValue4, defaultValue5)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2, defaultValue3, defaultValue4, defaultValue5};\
	I_ASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)


/**
	Used to assign value for single parameter with template type (attribute, reference or factory).
*/
#define I_TASSIGN_BASE(member, id, description, isObligatory)\
	static icomp::TAttributeStaticInfo<member##_AttrType> member##_Info(staticInfo, id, description, &member##_Default, isObligatory, istd::CClassInfo::GetInfo<typename member##_Type::InterfaceType>());\
	if (componentPtr != NULL){\
		componentPtr->member.Init(componentPtr, member##_Info);\
	}

/**
	Used to assign value for single parameter with template type (attribute, reference or factory).
*/
#define I_TASSIGN(member, id, description, isObligatory, defaultValue)\
	static member##_AttrType member##_Default(defaultValue);\
	I_TASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters with template type (attributes, references or factories).
*/
#define I_TASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)\
	static member##_AttrType member##_Default(int(sizeof(member##_DefaultElements) / sizeof(member##_AttrType::ValueType)), member##_DefaultElements);\
	I_TASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters with template type (attributes, references or factories) with no default parameter.
*/
#define I_TASSIGN_MULTI_0(member, id, description, isObligatory)\
	static member##_AttrType member##_Default(0, NULL);\
	I_TASSIGN_BASE(member, id, description, isObligatory)

/**
	Used to assign value for multiply parameters with template type (attributes, references or factories) with 1 default parameter.
*/
#define I_TASSIGN_MULTI_1(member, id, description, isObligatory, defaultValue1)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1};\
	I_TASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)

/**
	Used to assign value for multiply parameters with template type (attributes, references or factories) with 2 default parameter.
*/
#define I_TASSIGN_MULTI_2(member, id, description, isObligatory, defaultValue1, defaultValue2)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2};\
	I_TASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)

/**
	Used to assign value for multiply parameters (with template type attributes, references or factories) with 3 default parameter.
*/
#define I_TASSIGN_MULTI_3(member, id, description, isObligatory, defaultValue1, defaultValue2, defaultValue3)\
	static member##_AttrType::ValueType member##_DefaultElements[] = {defaultValue1, defaultValue2, defaultValue3};\
	I_TASSIGN_MULTI_BASE(member, id, description, isObligatory, defaultValue)


/**
	Cast to specified interface trying to use component interface query.
	It extends standard dynamic_cast functinality when you use composed components.
*/
template <class Dest>
Dest* CompCastPtr(istd::IPolymorphic* objectPtr)
{
	const icomp::IComponent* componentPtr = dynamic_cast<const icomp::IComponent*>(objectPtr);
	if (componentPtr != NULL){
		icomp::IComponent* parentComponentPtr = const_cast<icomp::IComponent*>(componentPtr->GetParentComponent(true));

		if (parentComponentPtr != NULL){
			Dest* retVal = (Dest*)parentComponentPtr->GetInterface(istd::CClassInfo(typeid(Dest)));

			if (retVal != NULL){
				return retVal;
			}
		}
	}

	return dynamic_cast<Dest*>(objectPtr);
}


/**
	Cast to specified interface trying to use component interface query.
	It extends standard dynamic_cast functinality when you use composed components.
	\overload
*/
template <class Dest>
const Dest* CompCastPtr(const istd::IPolymorphic* objectPtr)
{
	const icomp::IComponent* componentPtr = dynamic_cast<const icomp::IComponent*>(objectPtr);
	if (componentPtr != NULL){
		icomp::IComponent* parentComponentPtr = const_cast<icomp::IComponent*>(componentPtr->GetParentComponent(true));

		if (parentComponentPtr != NULL){
			const Dest* retVal = (const Dest*)parentComponentPtr->GetInterface(istd::CClassInfo(typeid(Dest)));

			if (retVal != NULL){
				return retVal;
			}
		}
	}

	return dynamic_cast<const Dest*>(objectPtr);
}


#endif // !icomp_CComponentBase_included

