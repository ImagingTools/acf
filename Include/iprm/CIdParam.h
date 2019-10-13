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
	virtual QByteArray GetId() const;
	virtual void SetId(const QByteArray& id);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool IsEqual(const IChangeable& object) const;
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	QByteArray m_id;
};


} // namespace iprm


#endif // !iprm_CIdParam_included


