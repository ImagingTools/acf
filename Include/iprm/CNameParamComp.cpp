#include "iprm/CNameParamComp.h"


namespace iprm
{


// reimplemented (iser::ISerializable)

bool CNameParamComp::Serialize(iser::IArchive& archive)
{
	return SerializeName(archive);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CNameParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultNameAttrPtr.IsValid()){
		SetName(*m_defaultNameAttrPtr);
	}
}


} // namespace ibase


