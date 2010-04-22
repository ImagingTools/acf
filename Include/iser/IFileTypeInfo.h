#ifndef iser_IFileTypeInfo_included
#define iser_IFileTypeInfo_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Provide loading and saving of objects.

	\ingroup Persistence
*/
class IFileTypeInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Describe detailed options of query.
	*/
	enum QueryFlags
	{
		/**
			No loading operation should be considered.
		*/
		QF_NO_LOADING = 0x0001,
		/**
			No saving operation should be considered.
		*/
		QF_NO_SAVING = 0x0002,
	};

	/**
		Get file extensions supported by this loader
		\param	result		list of extensions, e.g. {"txt", "doc"}.
		\param	flags		set of flags \sa QueryFlags.
		\param	doAppend	if true, list of extensions should be appended to existing list.
	*/
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const = 0;

	/**
		Get description of object type associated with single extension.
	*/
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const = 0;
};


} // namespace iser


#endif // !iser_IFileTypeInfo_included


