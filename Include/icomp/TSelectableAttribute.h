#ifndef icomp_TSelectableAttribute_included
#define icomp_TSelectableAttribute_included


#include "icomp/TMultiAttribute.h"


namespace icomp
{		


/**
	Template implementation of multiple attribute with one selectable value.
	\internal
	Don't use direct this class, use macros I_MULTIATTR and I_ASSIGN instead.
*/
template <typename Value> 
class TSelectableAttribute: public icomp::TMultiAttribute<Value>
{
public:
	typedef icomp::TMultiAttribute<Value> BaseClass;

	TSelectableAttribute();
	TSelectableAttribute(const TSelectableAttribute& attribute);

	const Value& GetSelectedValue() const;
	int GetSelectedIndex() const;
	void SetSelected(int index);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_selectedIndex;
};


template <typename Value> 
TSelectableAttribute<Value>::TSelectableAttribute()
	:m_selectedIndex(-1)
{
}


template <typename Value> 
TSelectableAttribute<Value>::TSelectableAttribute(const TSelectableAttribute& attribute)
	:BaseClass(attribute)
{
	m_selectedIndex = attribute.m_selectedIndex;
}


template <typename Value> 
const Value& TSelectableAttribute<Value>::GetSelectedValue() const
{
	static Value emptyValue;

	if(m_selectedIndex > -1 && m_selectedIndex < GetCount()){
		return GetValue(m_selectedIndex);
	}

	return emptyValue;
}


template <typename Value> 
int TSelectableAttribute<Value>::GetSelectedIndex() const
{
	return m_selectedIndex;
}


template <typename Value> 
void TSelectableAttribute<Value>::SetSelected(int index)
{
	I_ASSERT(index < GetCount());

	m_selectedIndex = index;
}


// reimplemented (iser::ISerializable)

template <typename Value> 
bool TSelectableAttribute<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	static iser::CArchiveTag selectedIndexTag("SelectedIndex", "Selected index");

	retVal = retVal && archive.BeginTag(selectedIndexTag);
	retVal = retVal && archive.Process(m_selectedIndex);
	retVal = retVal && archive.EndTag(selectedIndexTag);

	return retVal;
}


typedef TSelectableAttribute<istd::CString> CSelectableStringAttribute;
typedef TSelectableAttribute<int> CSelectableIntAttribute;
typedef TSelectableAttribute<double> CSelectableDoubleAttribute;
typedef TSelectableAttribute<bool> CSelectableBoolAttribute;


} // namespace icomp


#endif // !icomp_TSelectableAttribute_included

