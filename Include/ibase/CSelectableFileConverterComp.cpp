#include "ibase/CSelectableFileConverterComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{


// public methods
	
CSelectableFileConverterComp::CSelectableFileConverterComp()
:	m_optionsCount(0),
	m_selectedOptionIndex(-1)
{
}


// reimplemented (ibase::IFileConvertCopy)

bool CSelectableFileConverterComp::CopyFile(
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const iprm::IParamsSet* /*paramsPtr*/) const
{
	if (m_selectedOptionIndex >= 0){
		return m_slaveConvertersCompPtr[m_selectedOptionIndex]->CopyFile(inputFilePath, outputFilePath);
	}

	return false;
}


// reimplemented (iprm::ISelectionParam)

const iprm::ISelectionConstraints* CSelectableFileConverterComp::GetConstraints() const
{
	return this;
}


int CSelectableFileConverterComp::GetSelectedOptionIndex() const
{
	return m_selectedOptionIndex;
}


bool CSelectableFileConverterComp::SetSelectedOptionIndex(int index)
{
	if (index >= 0 && index < m_optionsCount){
		istd::CChangeNotifier changePtr(this);

		m_selectedOptionIndex = index;

		return true;
	}

	return false;
}


iprm::ISelectionParam* CSelectableFileConverterComp::GetActiveSubselection() const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectableFileConverterComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag selectedOptionIndexTag("SelectedIndex", "Selected option index");
	retVal = retVal && archive.BeginTag(selectedOptionIndexTag);
	retVal = retVal && archive.Process(m_selectedOptionIndex);
	retVal = retVal && archive.EndTag(selectedOptionIndexTag);

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CSelectableFileConverterComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_slaveConverterNamesAttrPtr.IsValid());
	I_ASSERT(m_slaveConvertersCompPtr.IsValid());

	m_optionsCount = istd::Min(m_slaveConvertersCompPtr.GetCount(), m_slaveConverterNamesAttrPtr.GetCount());
	if (m_optionsCount > 0){
		m_selectedOptionIndex = 0;
	}
}


// protected methods

// reimplemented (iprm::ISelectionConstraints)

int CSelectableFileConverterComp::GetOptionsCount() const
{
	return m_optionsCount;
}


const istd::CString& CSelectableFileConverterComp::GetOptionName(int index) const
{
	I_ASSERT(m_slaveConverterNamesAttrPtr.IsValid());

	return m_slaveConverterNamesAttrPtr[index];
}


} // namespace ibase


