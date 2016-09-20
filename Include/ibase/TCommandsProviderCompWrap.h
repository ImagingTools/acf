#ifndef ibase_TCommandsProviderCompWrap_included
#define ibase_TCommandsProviderCompWrap_included


// ACF includes
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>


namespace ibase
{


/**
	Generic macro for ICommandsProvider implementation.
*/
template <class Base>
class TCommandsProviderCompWrap:
			public Base,
			public imod::TModelWrap<ibase::ICommandsProvider>
{
public:
	typedef Base BaseClass;

	I_BEGIN_COMPONENT(TCommandsProviderCompWrap);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
	I_END_COMPONENT;
};


} // namespace ibase


#endif // !ibase_TCommandsProviderCompWrap_included


