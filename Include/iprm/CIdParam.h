#ifndef iprm_CIdParam_included
#define iprm_CIdParam_included


// ACF includes
#include <iprm/IIdParam.h>


namespace iprm
{


/**
	Implementation of a named object over iprm::INameParam interface.
*/
class CIdParam: virtual public IIdParam
{
public:
	// reimplemented (iprm::INameParam)
	virtual QByteArray GetId() const override;
	virtual void SetId(const QByteArray& id) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	QByteArray m_id;
};


} // namespace iprm


#endif // !iprm_CIdParam_included


