#ifndef iprm_COptionsManager_included
#define iprm_COptionsManager_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <iprm/IOptionsManager.h>
#include <iprm/CSelectionParam.h>


namespace iprm
{


class IOptionsList;


/**
	Implementation of a simple options manager.
*/
class COptionsManager:
			public CSelectionParam,
			virtual public IOptionsManager
{
public:
	typedef CSelectionParam BaseClass;

	COptionsManager();

	/**
		Set additional fixed list of options.
	*/
	void SetFixedOptionsList(const iprm::IOptionsList* slaveSelectionConstraintsPtr);

	/**
		Removes all options.
	*/
	virtual void ResetOptions();

	/**
		Get the index for the given option ID.
		\return found option index, or a negative value if the option with the requested ID was not found.
	*/
	int GetOptionIndexById(const QByteArray& optionId) const;

	/**
		Get the index for the given option name.
		\return found option index, or a negative value if the option with the requested name was not found.
		\note If the container contains more options with the same name, the first found option index will be returned.
	*/
	int GetOptionIndexByName(const QString& optionName) const;

	/**
		Check if selection is serialized.
	*/
	bool IsSelectionSerialized() const;

	/**
		Turn on or off selection serializing.
	*/
	void SetSelectionSerialized(bool  state);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

	// reimplemented (iprm::IOptionsManager)
	virtual int GetOptionOperationFlags(int index = -1) const override;
	virtual bool SetOptionEnabled(int index, bool isEnabled = true) override;
	virtual bool RemoveOption(int index) override;
	virtual bool InsertOption(
					const QString& optionName,
					const QByteArray& optionId,
					const QString& optionDescription = QString(),
					int index = -1) override;
	virtual bool SwapOptions(int index1, int index2) override;
	virtual bool SetOptionName(int index, const QString& optionName) override;
	virtual bool SetOptionDescription(int index, const QString& optionDescription) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iser::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	struct OptionInfo
	{
		OptionInfo()
			:isEnabled(true)
		{
		}

		OptionInfo(const QString& optionName, QByteArray optionId, const QString& optionDescription)
			:optionName(optionName),
			optionId(optionId),
			optionDescription(optionDescription),
			isEnabled(true)
		{
		}

		QString optionName;
		QByteArray optionId;
		QString optionDescription;
		bool isEnabled;
	};

	typedef QVector<OptionInfo> Options;

	Options m_options;

	const iprm::IOptionsList* m_slaveSelectionConstraintsPtr;

	bool m_isSelectionSerialized;
};


} // namespace iprm


#endif // !iprm_COptionsManager_included


