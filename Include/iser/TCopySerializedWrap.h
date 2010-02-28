#ifndef iser_TCopySerializedWrap_included
#define iser_TCopySerializedWrap_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/TUpdateManagerWrap.h"


namespace iser
{


/// @cond DOCUMENT_ALL
/**
	Try copy of two objects using of serialization.

	\ingroup Persistence
*/
extern bool CopyByArchive(const istd::IChangeable& object, istd::IChangeable& result);
/// @endcond


template <class Base>
class TCopySerializedWrap: public istd::TUpdateManagerWrap<Base>
{
public:
	typedef istd::TUpdateManagerWrap<Base> BaseClass;

	// pseudo-reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object);
};


// public methods

// pseudo-reimplemented (istd::IChangeable)

template <class Base>
bool TCopySerializedWrap<Base>::CopyFrom(const istd::IChangeable& object)
{
	if (BaseClass::CopyFrom(object)){
		return true;
	}
	else{
		return CopyByArchive(object, *this);
	}
}


} // namespace iser


#endif // !iser_TCopySerializedWrap_included


