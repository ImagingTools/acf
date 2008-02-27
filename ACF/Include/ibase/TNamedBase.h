#ifndef ibase_TNamedBase_included
#define ibase_TNamedBase_included


#include "ibase/ibase.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/TChangeNotifier.h"


namespace ibase
{


/**
	\ingroup ibase

	Template based implementation of a named object. 
	This class is a pseudo-implementation of istd::INamed interface.
*/
template <class BaseClass>
class TNamedBase: virtual public BaseClass
{
public:
	// peudeo-reimplemented (istd::INamed)
	virtual const istd::CString& GetName() const;
	virtual void SetName(const istd::CString& name);

protected:
	virtual bool SerializeName(iser::IArchive& archive);

protected:
	istd::CString m_name;
};



// peudeo-reimplemented (istd::INamed)

template <class BaseClass>
const istd::CString& TNamedBase<BaseClass>::GetName() const
{
	return m_name;
}


template <class BaseClass>
void TNamedBase<BaseClass>::SetName(const istd::CString& name)
{
	istd::TChangeNotifier<istd::IChangeable> changePtr(this);

	m_name = name;
}


// protected members

template <class BaseClass>
bool TNamedBase<BaseClass>::SerializeName(iser::IArchive& archive)
{
	static iser::CArchiveTag nameTag("Name", "Object name");

	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(m_name);
	retVal = retVal && archive.EndTag(nameTag);

	return retVal;
}


} // namespace ibase


#endif // ibase_TNamedBase_included
