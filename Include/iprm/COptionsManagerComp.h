#ifndef iprm_COptionsManagerComp_included
#define iprm_COptionsManagerComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IOptionsManager.h"
#include "iprm/ISelectionConstraints.h"


namespace iprm
{


/**
	Implementation of selection constraints.
*/
class COptionsManagerComp:
			public icomp::CComponentBase,
			virtual public iprm::IOptionsManager,
			virtual public iser::ISerializable,
			virtual public iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(COptionsManagerComp);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_REGISTER_INTERFACE(iprm::IOptionsManager);
		I_ASSIGN(m_slaveSelectionConstraintsCompPtr, "SlaveConstraints", "Slave constraints object", false, "SlaveConstraints");
	I_END_COMPONENT;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual istd::CString GetOptionName(int index) const;
	virtual istd::CString GetOptionDescription(int index) const;
	virtual std::string GetOptionId(int index) const;

	// reimplemented (iprm::IOptionsManager)
	virtual void RemoveOption(int index);
	virtual bool InsertOption(
					istd::CString& optionName,
					const std::string& optionId,
					const istd::CString& optionDescription = istd::CString(), 
					int index = -	1);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	struct OptionInfo
	{
		OptionInfo()
		{
		}

		OptionInfo(const istd::CString& optionName, std::string optionId, const istd::CString& optionDescription)
			:optionName(optionName),
			optionId(optionId),
			optionDescription(optionDescription)
		{
		}

		istd::CString optionName;
		std::string optionId;
		istd::CString optionDescription;
	};

	typedef std::vector<OptionInfo> Options;

	Options m_options;


private:
	I_REF(iprm::ISelectionConstraints, m_slaveSelectionConstraintsCompPtr);
};


} // namespace iprm


#endif // !iprm_COptionsManagerComp_included


