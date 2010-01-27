#include "ibase/CUuidComp.h"


#include "istd/CStaticServicesProvider.h"

#include "isys/ISystemEnvironment.h"


namespace ibase
{


// reimplemented (iser::ISerializable)

bool CUuidComp::Serialize(iser::IArchive& archive)
{
	return SerializeName(archive);
}


// protected methods

// reimplemented (icomp::IComponent)

void CUuidComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	isys::ISystemEnvironment* systemEnvironmentPtr = istd::GetService<isys::ISystemEnvironment>();
	if (systemEnvironmentPtr != NULL){
		SetName(systemEnvironmentPtr->GetUniqueIdentifier());
	}
}


} // namespace ibase


