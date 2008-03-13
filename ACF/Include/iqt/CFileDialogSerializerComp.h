#ifndef iqt_CFileDialogSerializerComp_included
#define iqt_CFileDialogSerializerComp_included


#include "iqt/iqt.h"


#include "iser/IFileSerializer.h"

#include "icomp/CComponentBase.h"


namespace iqt
{


class CFileDialogSerializerComp: public icomp::CComponentBase, virtual public iser::IFileSerializer
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_BEGIN_COMPONENT(CFileDialogSerializerComp)
		I_REGISTER_INTERFACE(iser::IFileSerializer)
		I_ASSIGN(m_fileFiltersCompPtr, "FileFilters", "File Filters", "*.*", true)
		I_ASSIGN(m_fileFiltersAttrPtr, "IconsPath", "Path to the icon files", ".", true)
	I_END_COMPONENT

	// reimplemented (iser::IFileSerializer)
	virtual int LoadFromFile(iser::ISerializable& data, const istd::CString& fileName = istd::CString());
	virtual int SaveToFile(iser::ISerializable& data, const istd::CString& fileName = istd::CString());
	virtual const istd::CString& GetLastSaveFileName() const;
	virtual const istd::CString& GetLastOpenFileName() const;

protected:
	virtual QString GetOpenFileName(const istd::CString& fileName) const;
	virtual QString GetSaveFileName(const istd::CString& fileName) const;

	iser::IFileSerializer* GetSerializerFor(const QString& fileName) const;

protected:
	I_MULITREF(iser::IFileSerializer, m_serializersCompPtr);
	I_MUTLIATTR(icomp::CStringAttribute, m_fileFiltersAttrPtr);

	mutable istd::CString m_lastOpenFileName;
	mutable istd::CString m_lastSaveFileName;

	mutable QString m_lastOpenPath;
	mutable QString m_lastSavePath;
};
	  

} // namespace iqt


#endif // !iqt_CFileDialogSerializerComp_included

