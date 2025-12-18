#include <iprm/CIdParamComp.h>


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CIdParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIdAttrPtr.IsValid()){
		BaseClass2::SetId(*m_defaultIdAttrPtr);
	}
}


} // namespace iprm


