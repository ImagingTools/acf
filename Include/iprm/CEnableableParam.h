#ifndef iprm_CEnableableParam_included
#define iprm_CEnableableParam_included


// ACF includes
#include "iprm/IEnableableParam.h"


namespace iprm
{


/**
	Basic implementation of IEnableableParam interface.

	\ingroup Helpers
*/
class CEnableableParam: virtual public iprm::IEnableableParam
{
public:
	CEnableableParam(bool isEnabled = true);

	// reimplemented (iprm::IEnableableParam)
	virtual bool IsEnabled() const;
	virtual bool IsEnablingAllowed() const;
	virtual bool SetEnabled(bool isEnabled = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_isEnabled;
};


} // namespace iprm


#endif // iprm_CEnableableParam_included


