#ifndef iprm_CEnableableParam_included
#define iprm_CEnableableParam_included


// ACF includes
#include <iprm/IEnableableParam.h>


namespace iprm
{


/**
	Basic implementation of IEnableableParam interface.
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

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const;

private:
	bool m_isEnabled;
};


} // namespace iprm


#endif // iprm_CEnableableParam_included


