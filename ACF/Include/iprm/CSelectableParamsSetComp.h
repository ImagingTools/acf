#ifndef iprm_CSelectableParamsSetComp_included
#define iprm_CSelectableParamsSetComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Interface allowing to select single option from list of options.
*/
class CSelectableParamsSetComp:
			public icomp::CComponentBase,
			virtual public IParamsSet,
			virtual public ISelectionParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectableParamsSetComp)
		I_REGISTER_INTERFACE(IParamsSet)
		I_REGISTER_INTERFACE(ISelectionParam)
		I_ASSIGN(m_selectionIdAttrPtr, "SelectionId", "ID of selection in parameter set", true, "Input")
		I_ASSIGN(m_paramsManagerCompPtr, "ParamsManager", "Manager of parameter set used to realize selection", true, "ParamsManager")
	I_END_COMPONENT

	// reimplemented (iprm::IParamsSet)
	virtual const iser::ISerializable* GetParameter(const std::string& id) const;
	virtual iser::ISerializable* GetEditableParameter(const std::string& id);

	// reimplemented (iprm::ISelectionParam)
	virtual int GetOptionsCount() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual const istd::CString& GetOptionName(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_selectedIndex;

	I_ATTR(istd::CString, m_selectionIdAttrPtr);
	I_REF(IParamsManager, m_paramsManagerCompPtr);
};


} // namespace iprm


#endif // !iprm_CSelectableParamsSetComp_included


