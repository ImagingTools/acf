#ifndef iprm_CSelectionConstraintsComp_included
#define iprm_CSelectionConstraintsComp_included


// ACF includes
#include <iser/ISerializable.h>
#include <icomp/CComponentBase.h>
#include <iprm/IOptionsList.h>
#include <iprm/IEnableableParam.h>


namespace iprm
{


/**
	Implementation of selection constraints.
*/
class CSelectionConstraintsComp:
			public icomp::CComponentBase,
			virtual public iprm::IOptionsList
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionConstraintsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_ASSIGN_MULTI_0(m_optionNamesAttrPtr, "OptionNames", "Names of selectable options", true);
		I_ASSIGN_MULTI_0(m_optionDescriptionsAttrPtr, "OptionDescriptions", "Descriptions of selectable options", true);
		I_ASSIGN_MULTI_0(m_optionIdsAttrPtr, "OptionIds", "List of option IDs", true);
		I_ASSIGN_MULTI_0(m_optionsEnablersCompPtr, "OptionEnablers", "List of enablers for the options", false);
	I_END_COMPONENT;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

private:
	I_MULTITEXTATTR(m_optionNamesAttrPtr);
	I_MULTITEXTATTR(m_optionDescriptionsAttrPtr);
	I_MULTIATTR(QByteArray, m_optionIdsAttrPtr);
	I_MULTIREF(iprm::IEnableableParam, m_optionsEnablersCompPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionConstraintsComp_included


