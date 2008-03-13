#ifndef iqt_CIconProviderComp_included
#define iqt_CIconProviderComp_included


#include "iqt/iqt.h"


#include "iqt/IIconProvider.h"

#include "icomp/CComponentBase.h"


namespace iqt
{


class CIconProviderComp: public icomp::CComponentBase, public iqt::IIconProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_BEGIN_COMPONENT(CIconProviderComp)
		I_REGISTER_INTERFACE(iqt::IIconProvider)
		I_ASSIGN(m_iconFilesAttrPtr, "IconFiles", "Names of the icon files", ".", true)
		I_ASSIGN(m_iconPathAttrPtr, "IconsPath", "Path to the icon files", ".", true)
	I_END_COMPONENT

	// reimplemented (iqt::IIconProvider)
	virtual int GetIconCount() const;
	virtual QIcon GetIcon(int index) const;

private:
	I_MULTIATTR(icomp::CStringAttribute, m_iconFilesAttrPtr);
	I_ATTR(icomp::CStringAttribute, m_iconPathAttrPtr);
};


} // namespace iqt


#endif // !iqt_CIconProviderComp_included

