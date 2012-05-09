#ifndef idoc_IDocumentStateComparator_included
#define idoc_IDocumentStateComparator_included


// ACF includes
#include "istd/IChangeable.h"


namespace idoc
{


/**
	Allows to store some document state used as reference for comarision.
*/
class IDocumentStateComparator: virtual public istd::IChangeable
{
public:
	enum DocumentChangeFlag
	{
		/**
			There is no information about document change.
		*/
		DCF_UNKNOWN,
		/**
			Current document state equals the stored one.
		*/
		DCF_EQUAL,
		/**
			Current document state equals the stored one.
		*/
		DCF_DIFFERENT
	};

	/**
		Check if there is stored document available.
	*/
	virtual bool HasStoredDocumentState() const = 0;
	/**
		Store state of some document.
	*/
	virtual bool StoreDocumentState() = 0;
	/**
		Restore last stored document state.
	*/
	virtual bool RestoreDocumentState() = 0;
	/**
		Check if document state is the same as stored state.
	*/
	virtual DocumentChangeFlag GetDocumentChangeFlag() const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentStateComparator_included


