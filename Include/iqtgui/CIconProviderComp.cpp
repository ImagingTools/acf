#include <iqtgui/CIconProviderComp.h>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IIconProvider)

int CIconProviderComp::GetIconCount() const
{
	return m_iconFilesAttrPtr.GetCount();
}


QIcon CIconProviderComp::GetIcon(int iconIndex) const
{
	static QIcon emptyIcon;

	Q_ASSERT(iconIndex < m_iconFilesAttrPtr.GetCount());
	Q_ASSERT(iconIndex >= 0);

	if (iconIndex >= 0 && iconIndex < m_iconFilesAttrPtr.GetCount()){
		QString iconPath = QString(*m_iconPathAttrPtr) + QString("/") + QString(m_iconFilesAttrPtr[iconIndex]);

		return QIcon(GetIconPath(iconPath));
	}

	return emptyIcon;
}


} // namespace iqtgui


