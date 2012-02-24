#ifndef iqtgui_CIconProviderComp_included
#define iqtgui_CIconProviderComp_included


// ACF includes
#include "iqtgui/IIconProvider.h"

#include "icomp/CComponentBase.h"


namespace iqtgui
{


class CIconProviderComp: public icomp::CComponentBase, virtual public iqtgui::IIconProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CIconProviderComp);
		I_REGISTER_INTERFACE(iqtgui::IIconProvider);
		I_ASSIGN_MULTI_0(m_iconFilesAttrPtr, "IconFiles", "Names of the icon files", true);
		I_ASSIGN(m_iconPathAttrPtr, "IconsPath", "Path to the icon files", true, ".");
	I_END_COMPONENT;

	// reimplemented (iqtgui::IIconProvider)
	virtual int GetIconCount() const;
	virtual QIcon GetIcon(int index) const;

private:
	I_MULTIATTR(QString, m_iconFilesAttrPtr);
	I_ATTR(QString, m_iconPathAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CIconProviderComp_included

