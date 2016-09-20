#ifndef iqt_CClipboardSerializerComp_included
#define iqt_CClipboardSerializerComp_included


// ACF includes
#include <iser/IVersionInfo.h>
#include <ifile/IFilePersistence.h>
#include <ilog/TLoggerCompWrap.h>
#include <iqt/iqt.h>


namespace iqt
{


/**
	A clipboard serializer component.

	\ingroup Persistence
*/
class CClipboardSerializerComp:
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CClipboardSerializerComp);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_mimeTypeAttrPtr, "MimeType", "Describe clipboard type", true, "acf/generic");
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

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_ATTR(QString, m_mimeTypeAttrPtr);
};



} // namespace iqt


#endif // !iqt_CClipboardSerializerComp_included


