#ifndef imod_TModelCompWrap_included
#define imod_TModelCompWrap_included


#include "istd/IChangeable.h"

#include "imod/TModelWrap.h"

#include "icomp/CComponentBase.h"


namespace icomp
{


/**
	Makes component and model from some class implementing interface \c istd::IChangeable.
	This model wrapper extends functionality of \c icomp::TModelWrap for components.

	\ingroup ComponentConcept
*/
template <class Base>
class TModelCompWrap: public imod::TModelWrap<Base>
{
public:
	typedef imod::TModelWrap<Base> BaseClass;

	I_BEGIN_COMPONENT(TModelCompWrap)
		I_REGISTER_INTERFACE(istd::IChangeable)
		I_REGISTER_INTERFACE(imod::IModel)
	I_END_COMPONENT
};


} // namespace icomp


#endif // !imod_TModelCompWrap_included


