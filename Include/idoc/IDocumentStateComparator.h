#ifndef idoc_IDocumentStateComparator_included
#define idoc_IDocumentStateComparator_included


#include "istd/IChangeable.h"


namespace idoc
{


/**
	Allows to store some document state used as reference for comarision.
*/
class IDocumentStateComparator: virtual public istd::IPolymorphic
{
public:
	/**
		Store state of some document for later comarision.
	*/
	virtual void StoreState(const istd::IChangeable& document) = 0;
	/**
		Check if document state is the same as stored state.
	*/
	virtual bool CheckStateEquals(const istd::IChangeable& document) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentStateComparator_included


