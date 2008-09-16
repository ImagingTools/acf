#ifndef iqt_CFileDialogSerializerComp_included
#define iqt_CFileDialogSerializerComp_included


#include "iqt/iqt.h"


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"


namespace iqt
{


class CFileDialogSerializerComp: public icomp::CComponentBase, virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileDialogSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_ASSIGN_MULTI_0(m_serializersCompPtr, "Serializers", "List of file serializers will be used as slaves", true)
		I_ASSIGN_MULTI_0(m_fileFiltersAttrPtr, "FileFilters", "File Filters", true)
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual const istd::CString& GetLastLoadFileName() const;
	virtual const istd::CString& GetLastSaveFileName() const;

protected:
	virtual QString GetOpenFileName(const istd::CString& filePath) const;
	virtual QString GetSaveFileName(const istd::CString& filePath) const;

	iser::IFileLoader* GetLoaderFor(const QString& filePath) const;

private:
	I_MULTIREF(iser::IFileLoader, m_serializersCompPtr);
	I_MULTIATTR(istd::CString, m_fileFiltersAttrPtr);

	mutable istd::CString m_lastOpenFileName;
	mutable istd::CString m_lastSaveFileName;

	mutable QString m_lastOpenPath;
	mutable QString m_lastSavePath;
};
	  

} // namespace iqt


#endif // !iqt_CFileDialogSerializerComp_included

