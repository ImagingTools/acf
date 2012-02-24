#ifndef iqt_CBitmapLoaderComp_included
#define iqt_CBitmapLoaderComp_included


// Qt includes
#include <QDir>
#include <QStringList>


// ACF includes
#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader.
*/
class CBitmapLoaderComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7d0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT
	};

	I_BEGIN_COMPONENT(CBitmapLoaderComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	bool IsOperationSupportedIntern(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr,
				bool forLoading,
				bool forSaving,
				bool useLog) const;
};


} // namespace iqt


#endif // !iqt_CBitmapLoaderComp_included


