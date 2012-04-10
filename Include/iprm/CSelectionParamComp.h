#ifndef iprm_CSelectionParamComp_included
#define iprm_CSelectionParamComp_included


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"


namespace iprm
{


/**
	Basic implementation of selection parameter.
*/
class CSelectionParamComp:
			public icomp::CComponentBase,
			virtual public ISelectionParam
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

	CSelectionParamComp();

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Set selection index according to a given option ID.
	*/
	bool SetSelectedOptionIndexById(const QByteArray& selectedOptionId);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(ISelectionConstraints, m_constraintsCompPtr);
	I_REF(ISelectionParam, m_subselectionCompPtr);
	I_ATTR(int, m_defaultIndexAttrPtr);

	int m_selectedOptionIndex;
};


} // namespace iprm


#endif // !iprm_CSelectionParamComp_included

