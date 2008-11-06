#ifndef iqtgui_CFileDialogSerializerComp_included
#define iqtgui_CFileDialogSerializerComp_included


#include "iser/IFileLoader.h"
#include "iser/IFileLoaderInfo.h"

#include "ibase/TMessageProducerWrap.h"

#include "icomp/CComponentBase.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


class CFileDialogSerializerComp: 
			public ibase::TMessageProducerWrap<icomp::CComponentBase>, 
			virtual public iser::IFileLoader,
			virtual public iser::IFileLoaderInfo
{
public:
	typedef ibase::TMessageProducerWrap<icomp::CComponentBase> BaseClass;

	enum MessageId
	{
		MI_NONE_SUPPORTS = 0xf8e0
	};

	I_BEGIN_COMPONENT(CFileDialogSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_REGISTER_INTERFACE(iser::IFileLoaderInfo)
		I_ASSIGN_MULTI_0(m_serializersCompPtr, "Serializers", "List of file serializers will be used as slaves", true)
		I_ASSIGN_MULTI_0(m_fileFiltersAttrPtr, "FileFilters", "File Filters", true)
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;

	// reimplemented (iser::IFileLoaderInfo)
	virtual istd::CString GetLastLoadFileName() const;
	virtual istd::CString GetLastSaveFileName() const;

protected:
	virtual QString GetOpenFileName(const istd::CString& filePath) const;
	virtual QString GetSaveFileName(const istd::CString& filePath) const;

	iser::IFileLoader* GetLoaderFor(const QString& filePath, bool beQuiet) const;

private:
	I_MULTIREF(iser::IFileLoader, m_serializersCompPtr);
	I_MULTIATTR(istd::CString, m_fileFiltersAttrPtr);

	mutable QString m_lastOpenPath;
	mutable QString m_lastSavePath;
};
	  

} // namespace iqtgui


#endif // !iqtgui_CFileDialogSerializerComp_included

