#ifndef ibase_CUuidComp_included
#define ibase_CUuidComp_included


#include "icomp/CComponentBase.h"

#include "ibase/TNamedWrap.h"


namespace ibase
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CUuidComp:
				public icomp::CComponentBase,
				public ibase::CNamed,
				virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CUuidComp);
		I_REGISTER_INTERFACE(istd::INamed);
		I_REGISTER_INTERFACE(iser::ISerializable);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace ibase


#endif // !ibase_CUuidComp_included


