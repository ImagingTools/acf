#ifndef iprm_CSelectionConstraintsComp_included
#define iprm_CSelectionConstraintsComp_included


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "iprm/ISelectionConstraints.h"


namespace iprm
{


/**
	Implementation of selection constraints.
*/
class CSelectionConstraintsComp:
			public icomp::CComponentBase,
			virtual public iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionConstraintsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_ASSIGN_MULTI_0(m_optionNamesAttrPtr, "OptionNames", "Names of selectable options", true);
	I_END_COMPONENT;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetOptionsCount() const;
	virtual istd::CString GetOptionName(int index) const;

private:
	I_MULTIATTR(istd::CString, m_optionNamesAttrPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionConstraintsComp_included


