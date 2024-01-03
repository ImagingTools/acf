#ifndef iprm_CIdParamComp_included
#define iprm_CIdParamComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <iprm/CIdParam.h>


namespace iprm
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CIdParamComp:
			public icomp::CComponentBase,
			public iprm::CIdParam
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iprm::CIdParam BaseClass2;

	I_BEGIN_COMPONENT(CIdParamComp);
		I_REGISTER_INTERFACE(iprm::IIdParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_defaultIdAttrPtr, "DefaultId", "Default ID", false, "");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(QByteArray, m_defaultIdAttrPtr);
};


} // namespace iprm


#endif // !iprm_CIdParamComp_included


