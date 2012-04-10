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
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;

	// reimplemented (iprm::IOptionsManager)
	virtual void RemoveOption(int index);
	virtual bool InsertOption(
					QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription = QString(), 
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

		OptionInfo(const QString& optionName, QByteArray optionId, const QString& optionDescription)
			:optionName(optionName),
			optionId(optionId),
			optionDescription(optionDescription)
		{
		}

		QString optionName;
		QByteArray optionId;
		QString optionDescription;
	};

	typedef QVector<OptionInfo> Options;

	Options m_options;


private:
	I_REF(iprm::ISelectionConstraints, m_slaveSelectionConstraintsCompPtr);
};


} // namespace iprm


#endif // !iprm_COptionsManagerComp_included


