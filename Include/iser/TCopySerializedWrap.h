#ifndef iser_TCopySerializedWrap_included
#define iser_TCopySerializedWrap_included


// ACF includes
#include "istd/IChangeable.h"


namespace iser
{


/// @cond DOCUMENT_ALL
/**
	Try copy of two objects using of serialization.

	\ingroup Persistence
*/
extern bool CopyByArchive(const istd::IChangeable& object, istd::IChangeable& result);
/**
	Compare two objects using coparision of serialized streams.

	\ingroup Persistence
*/
extern bool CompareByArchive(const istd::IChangeable& object1, const istd::IChangeable& object2);
/// @endcond


template <class Base>
class TCopySerializedWrap: public Base
{
public:
	typedef Base BaseClass;

	// pseudo-reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS);
	virtual bool IsEqual(const istd::IChangeable& object) const;
};


// public methods

// pseudo-reimplemented (istd::IChangeable)

template <class Base>
int TCopySerializedWrap<Base>::GetSupportedOperations() const
{
	return BaseClass::GetSupportedOperations() | istd::IChangeable::SO_COPY | istd::IChangeable::SO_COMPARE;
}


template <class Base>
bool TCopySerializedWrap<Base>::CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode)
{
	if (BaseClass::CopyFrom(object, mode)){
		return true;
	}
	else if (mode == BaseClass::CM_WITHOUT_REFS){
		return CopyByArchive(object, *this);
	}
	else{
		return false;
	}
}


template <class Base>
bool TCopySerializedWrap<Base>::IsEqual(const istd::IChangeable& object) const
{
	if ((BaseClass::GetSupportedOperations() & istd::IChangeable::SO_COMPARE) != 0){
		return BaseClass::IsEqual(object);
	}

	return CompareByArchive(object, *this);
}


} // namespace iser


#endif // !iser_TCopySerializedWrap_included


