#ifndef ibase_CSelectableFileConverterComp_included
#define ibase_CSelectableFileConverterComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"

#include "ibase/IFileConvertCopy.h"


namespace ibase
{


class CSelectableFileConverterComp:
			public icomp::CComponentBase,
			virtual public ibase::IFileConvertCopy,
			virtual public iprm::ISelectionParam,
			virtual protected iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectableFileConverterComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_slaveConvertersCompPtr, "SlaveConverters", "List of the slave converters", true);
		I_ASSIGN_MULTI_0(m_slaveConverterNamesAttrPtr, "SlaveConverterNames", "List of the converter names", true);
		I_ASSIGN_MULTI_0(m_slaveConverterDescriptionsAttrPtr, "SlaveConverterDescriptions", "List of the converter's descriptions", true);
	I_END_COMPONENT;

	CSelectableFileConverterComp();

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFile(
				const QString& inputFilePath,
				const QString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual std::string GetOptionId(int index) const;

private:
	I_MULTIREF(ibase::IFileConvertCopy, m_slaveConvertersCompPtr);
	I_MULTIATTR(QString, m_slaveConverterNamesAttrPtr);
	I_MULTIATTR(QString, m_slaveConverterDescriptionsAttrPtr);

	int m_optionsCount;
	int m_selectedOptionIndex;
};


} // namespace ibase


#endif // !ibase_CSelectableFileConverterComp_included
