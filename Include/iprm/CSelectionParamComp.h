#ifndef iprm_CSelectionParamComp_included
#define iprm_CSelectionParamComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iprm/IOptionsList.h"
#include "iprm/CSelectionParam.h"


namespace iprm
{


/**
	Component level implementation of selection parameter.
*/
class CSelectionParamComp:
			public icomp::CComponentBase,
			public CSelectionParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints describing allowed value ranges", false, "Constraints");
		I_ASSIGN(m_subselectionCompPtr, "Subselection", "Subselection object", false, "Subselection");
		I_ASSIGN(m_defaultIndexAttrPtr, "DefaultIndex", "Default value of selected index", true, 0);
	I_END_COMPONENT;

	// reimplemented (iprm::ISelectionParam)
	virtual ISelectionParam* GetSubselection(int index) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IOptionsList, m_constraintsCompPtr);
	I_REF(ISelectionParam, m_subselectionCompPtr);
	I_ATTR(int, m_defaultIndexAttrPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionParamComp_included

