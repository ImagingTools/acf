#ifndef iimg_CBitmapLoaderComp_included
#define iimg_CBitmapLoaderComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QStringList>

// ACF includes
#include <ifile/IFilePersistence.h>
#include <icomp/CComponentBase.h>
#include <ilog/TLoggerCompWrap.h>
#include <iimg/iimg.h>


namespace iimg
{


/**
	Bitmap loader component implementing interfaces \c ifile::IFilePersistence.
*/
class CBitmapLoaderComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_FORMAT = 0xe1a7d0
	};

	I_BEGIN_COMPONENT(CBitmapLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN_MULTI_0(m_extensionFilterAttrPtr, "ExtensionFilter", "Optional filter of extensions, lowercase as returned by QT", false)
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	bool IsOperationSupportedIntern(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr,
				bool forLoading,
				bool forSaving,
				bool useLog) const;

private:
	I_MULTIATTR(QByteArray, m_extensionFilterAttrPtr);
};


} // namespace iimg


#endif // !iimg_CBitmapLoaderComp_included


