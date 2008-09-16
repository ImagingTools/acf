#include "iqt/CIconProviderComp.h"


namespace iqt
{


// public methods

// reimplemented (iqt::IIconProvider)

int CIconProviderComp::GetIconCount() const
{
	return m_iconFilesAttrPtr.GetCount();
}


QIcon CIconProviderComp::GetIcon(int iconIndex) const
{
	static QIcon emptyIcon;

	I_ASSERT(iconIndex < m_iconFilesAttrPtr.GetCount());
	I_ASSERT(iconIndex >= 0);

	if (iconIndex >= 0 && iconIndex < m_iconFilesAttrPtr.GetCount()){
		return QIcon(iqt::GetQString(istd::CString(*m_iconPathAttrPtr) + istd::CString("/") + istd::CString(m_iconFilesAttrPtr[iconIndex])));
	}

	return emptyIcon;
}


} // namespace iqt


