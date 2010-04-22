#ifndef iqt_CClipboardSerializerComp_included
#define iqt_CClipboardSerializerComp_included


#include "iser/IVersionInfo.h"
#include "iser/IFileLoader.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


class CClipboardSerializerComp:
			public ibase::CLoggerComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CClipboardSerializerComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);

		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_mimeTypeAttrPtr, "MimeType", "Describe clipboard type", true, "acf/generic");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_ATTR(istd::CString, m_mimeTypeAttrPtr);
};



} // namespace iqt


#endif // !iqt_CClipboardSerializerComp_included


