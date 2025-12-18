#ifndef iprm_CNameParam_included
#define iprm_CNameParam_included


// ACF includes
#include <iprm/INameParam.h>


namespace iprm
{


/**
	Implementation of a named object over iprm::INameParam interface.
*/
class CNameParam: virtual public INameParam
{
public:
	// reimplemented (iprm::INameParam)
	virtual const QString& GetName() const override;
	virtual void SetName(const QString& name) override;
	virtual bool IsNameFixed() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;

protected:
	QString m_name;
};


} // namespace iprm


#endif // !iprm_CNameParam_included


