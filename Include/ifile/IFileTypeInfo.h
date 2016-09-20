#ifndef ifile_IFileTypeInfo_included
#define ifile_IFileTypeInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/IChangeable.h>


namespace ifile
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
		QF_SAVE = 1 << 0,
		/**
			No saving operation should be considered.
		*/
		QF_LOAD = 1 << 1,
		/**
			Operations with specified file name should be considered.
		*/
		QF_FILE = 1 << 2,

		/**
			Operations with specified directory name should be considered.
		*/
		QF_DIRECTORY = 1 << 3,

		/**
			Operations without specified path (anonymous) should be considered.
		*/
		QF_ANONYMOUS = 1 << 4
	};

	/**
		Get file extensions supported by this loader
		\param	result			list of extensions, e.g. {"txt", "doc"}.
		\param	dataObjectPtr	optional pointer to data object for which all possible extensions are requested.
								It can be NULL if any object is meant.
		\param	flags			set of flags \sa QueryFlags.
		\param	doAppend		if true, list of extensions should be appended to existing list.
	*/
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const = 0;

	/**
		Get description of object type associated with single extension.
	*/
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const = 0;
};


} // namespace ifile


#endif // !ifile_IFileTypeInfo_included


