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
	virtual bool IsEnabled() const override;
	virtual bool IsEnablingAllowed() const override;
	virtual bool SetEnabled(bool isEnabled = true) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;

private:
	bool m_isEnabled;
};


} // namespace iprm


#endif // iprm_CEnableableParam_included


