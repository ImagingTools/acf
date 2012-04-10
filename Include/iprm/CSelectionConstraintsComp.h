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
		I_ASSIGN_MULTI_0(m_optionDescriptionsAttrPtr, "OptionDescriptions", "Descriptions of selectable options", true);
	I_END_COMPONENT;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;

private:
	I_MULTIATTR(QString, m_optionNamesAttrPtr);
	I_MULTIATTR(QString, m_optionDescriptionsAttrPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionConstraintsComp_included


