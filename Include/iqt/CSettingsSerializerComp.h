#ifndef iqt_CSettingsSerializerComp_included
#define iqt_CSettingsSerializerComp_included


// Qt includes
#include <QtCore/QSettings>

// ACF includes
#include "ifile/IFilePersistence.h"
#include "icomp/CComponentBase.h"
#include "ibase/IApplicationInfo.h"
#include "ilog/TLoggerCompWrap.h"
#include "iqt/iqt.h"


namespace iqt
{


/**
	Store and load parameter as global settings.

	\ingroup Persistence
*/
class CSettingsSerializerComp:
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSettingsSerializerComp);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
		I_ASSIGN(m_rootKeyAttrPtr, "RootKey", "The root key in the file/registry for the serialized object", true, "Data");
		I_ASSIGN(m_scopeAttrPtr, "Scope", "The scope for the settingsin the registry. 0 - User scope.\n1 - System scope", false, QSettings::UserScope);
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
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(QByteArray, m_rootKeyAttrPtr);
	I_ATTR(int, m_scopeAttrPtr);
};


} // namespace iqt


#endif // !iqt_CSettingsSerializerComp_included


