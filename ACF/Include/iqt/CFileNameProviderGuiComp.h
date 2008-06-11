#ifndef iqt_CFileNameProviderGuiComp_included
#define iqt_CFileNameProviderGuiComp_included


#include "istd/IFileNameProvider.h"

#include "iqt/TDesignerGuiCompBase.h"

#include "iqt/Generated/ui_CFileNameProviderGuiComp.h"


namespace iqt
{


class CFileNameProviderGuiComp: public iqt::TDesignerGuiCompBase<Ui::CFileNameProviderGuiComp>,
								virtual public istd::IFileNameProvider
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiCompBase<Ui::CFileNameProviderGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CFileNameProviderGuiComp)
		I_ASSIGN(m_browseTitleAttrPtr, "BrowseTitle", "Title of browse dialog", false, "Select File/Directory");
		I_ASSIGN(m_fileFilterAttrPtr, "FileFilter", "File filter", false, "*.*");
		I_ASSIGN(m_isDirectoryAttrPtr, "IsDirectory", "Open existing directory flag", false, false);
	I_END_COMPONENT

	// reimplemented (istd::IFileNameProvider)
	virtual istd::CString GetFileName() const;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private slots:
	void OnBrowse();
	void OnFileNameChanged(const QString& fileName);

private:
	I_ATTR(istd::CString, m_browseTitleAttrPtr);
	I_ATTR(istd::CString, m_fileFilterAttrPtr);
	I_ATTR(bool, m_isDirectoryAttrPtr);
};


} // namespace iqt


#endif // !iqt_CFileNameProviderGuiComp_included


