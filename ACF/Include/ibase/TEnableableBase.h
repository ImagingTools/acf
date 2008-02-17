#ifndef ibase_TEnableableBase_included
#define ibase_TEnableableBase_included


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/ibase.h"


namespace ibase
{


/**
	Basic implementation of IEnabled interface.
*/
template <class BaseClass>
class TEnableableBase: virtual public BaseClass
{
public:
	TEnableableBase();

	// pseudo-reimplemented (istd::IEnableable)
	virtual bool IsEnabled() const;
	virtual bool IsEnablingAllowed() const;
	virtual void SetEnabled(bool isEnabled = true);

protected:
	/**
		Serialize state of enable flag.
	*/
	virtual bool SerializeState(ibase::IArchive& archive);

protected:
	bool m_isEnabled;
};


// public methods

TEnableableBase::Enableable()
{
	m_isEnabled = true;
}


// pseudo-reimplemented (istd::IEnableable)

bool TEnableableBase::IsEnabled() const
{
	return m_isEnabled;
}


bool TEnableableBase::IsEnablingAllowed() const
{
	return true;
}


void TEnableableBase::SetEnabled(bool isEnabled)
{
	if (IsEnablingAllowed()){
		m_isEnabled = isEnabled;
	}
}


// protected methods

bool TEnableableBase::SerializeState(iser::IArchive& archive)
{
	static iser::CArchiveTag enabledTag("Enabled", "Object enabled state");

	bool retVal = archive.beginTag(enabledTag);
	retVal = retVal && archive.process(m_isEnabled);
	retVal = retVal && archive.endTag(enabledTag);

	return retVal;
}


} // namespace ibase


#endif // ibase_TEnableableBase_included


