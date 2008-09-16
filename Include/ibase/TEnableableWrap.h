#ifndef ibase_TEnableableWrap_included
#define ibase_TEnableableWrap_included


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/ibase.h"


namespace ibase
{


/**
	Basic implementation of IEnabled interface.
*/
template <class Base>
class TEnableableWrap: virtual public Base
{
public:
	typedef Base BaseClass;

	TEnableableWrap();

	// pseudo-reimplemented (istd::IEnableable)
	virtual bool IsEnabled() const;
	virtual bool IsEnablingAllowed() const;
	virtual void SetEnabled(bool isEnabled = true);

protected:
	/**
		Serialize state of enable flag.
	*/
	virtual bool SerializeEnabled(iser::IArchive& archive);

private:
	bool m_isEnabled;
};


// public methods

template <class Base>
TEnableableWrap<Base>::TEnableableWrap()
{
	m_isEnabled = true;
}


// pseudo-reimplemented (istd::IEnableable)

template <class Base>
bool TEnableableWrap<Base>::IsEnabled() const
{
	return m_isEnabled;
}


template <class Base>
bool TEnableableWrap<Base>::IsEnablingAllowed() const
{
	return true;
}


template <class Base>
void TEnableableWrap<Base>::SetEnabled(bool isEnabled)
{
	if (IsEnablingAllowed() && (isEnabled != m_isEnabled)){
		istd::CChangeNotifier changePtr(this);
	
		m_isEnabled = isEnabled;
	}
}


// protected methods

template <class Base>
bool TEnableableWrap<Base>::SerializeEnabled(iser::IArchive& archive)
{
	static iser::CArchiveTag enabledTag("IsEnabled", "Object enabled state");

	bool retVal = archive.BeginTag(enabledTag);
	retVal = retVal && archive.Process(m_isEnabled);
	retVal = retVal && archive.EndTag(enabledTag);

	return retVal;
}


} // namespace ibase


#endif // ibase_TEnableableWrap_included


