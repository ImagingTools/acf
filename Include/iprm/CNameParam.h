#ifndef iprm_CNameParam_included
#define iprm_CNameParam_included


// ACF includes
#include "iprm/INameParam.h"


namespace iprm
{


/**
	Implementation of a named object over iprm::INameParam interface.

	\ingroup Helpers
*/
class CNameParam: virtual public INameParam
{
public:
	// reimplemented (iprm::INameParam)
	virtual const QString& GetName() const;
	virtual void SetName(const QString& name);
	virtual bool IsNameFixed() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	QString m_name;
};


} // namespace iprm


#endif // !iprm_CNameParam_included


