#ifndef iprm_COptionsManager_included
#define iprm_COptionsManager_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/ISerializable.h"
#include "iprm/IOptionsManager.h"


namespace iprm
{


class IOptionsList;


/**
	Implementation of a simple options manager.
*/
class COptionsManager:
			virtual public iprm::IOptionsManager,
			virtual public iser::ISerializable
{
public:
	COptionsManager();

	void SetSlaveSelectionConstraints(const iprm::IOptionsList* slaveSelectionConstraintsPtr);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

	// reimplemented (iprm::IOptionsManager)
	virtual void SetOptionEnabled(int index, bool isEnabled = true);
	virtual bool RemoveOption(int index);
	virtual bool InsertOption(
					const QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription = QString(), 
					int index = -	1);
	virtual bool SwapOptions(int index1, int index2);
	virtual bool SetOptionName(int index, const QString& optionName);
	virtual bool SetOptionDescription(int index, const QString& optionDescription);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

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
		bool isEnabled;
	};

	typedef QVector<OptionInfo> Options;

	Options m_options;

private:
	const iprm::IOptionsList* m_slaveSelectionConstraintsPtr;
};


} // namespace iprm


#endif // !iprm_COptionsManager_included


