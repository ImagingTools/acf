#include "iprm/CNameParamComp.h"


namespace iprm
{


// public methods

// reimplemented (iprm::INameParam)

void CNameParamComp::SetName(const QString& name)
{
	if (!*m_isNameFixedAttrPtr){
		BaseClass2::SetName(name);
	}
}


bool CNameParamComp::IsNameFixed() const
{
	return *m_isNameFixedAttrPtr;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultNameAttrPtr.IsValid()){
		BaseClass2::SetName(*m_defaultNameAttrPtr);
	}
}


} // namespace iprm


