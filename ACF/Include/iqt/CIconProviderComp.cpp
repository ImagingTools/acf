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

	if (index >= 0 && index < m_iconFilesAttrPtr.GetCount()){
		return QIcon(iqt::GetQString(m_iconPathAttrPtr.GetValue() + istd::CString("/") + m_iconFilesAttrPtr.GetValue(index)));
	}

	return emptyIcon;
}


} // namespace iqt
