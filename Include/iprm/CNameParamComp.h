#ifndef iprm_CNameParamComp_included
#define iprm_CNameParamComp_included


// ACF includes
#include <icomp/CComponentBase.h>

#include <iprm/CNameParam.h>


namespace iprm
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CNameParamComp:
			public icomp::CComponentBase,
			public iprm::CNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iprm::CNameParam BaseClass2;

	I_BEGIN_COMPONENT(CNameParamComp);
		I_REGISTER_INTERFACE(iprm::INameParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_defaultNameAttrPtr, "DefaultName", "Default name", false, "");
		I_ASSIGN(m_isNameFixedAttrPtr, "IsNameFixed", "When enabled, the name is fixed and cannot be changed during run tume", true, false);
	I_END_COMPONENT;

	// reimplemented (iprm::INameParam)
	virtual void SetName(const QString& name);
	virtual bool IsNameFixed() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_TEXTATTR(m_defaultNameAttrPtr);
	I_ATTR(bool, m_isNameFixedAttrPtr);
};


} // namespace iprm


#endif // !iprm_CNameParamComp_included


