#ifndef iqt_CSettingsSerializerComp_included
#define iqt_CSettingsSerializerComp_included


// ACF includes
#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Store and load parameter as global settings.
*/
class CSettingsSerializerComp:
			public ibase::CLoggerComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSettingsSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
		I_ASSIGN(m_rootKeyAttrPtr, "RootKey", "The root key in the file/registry for the serialized object", true, "Data");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(QByteArray, m_rootKeyAttrPtr);
};


} // namespace iqt


#endif // !iqt_CSettingsSerializerComp_included


