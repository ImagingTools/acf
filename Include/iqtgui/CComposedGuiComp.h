#ifndef iqtgui_CComposedGuiComp_included
#define iqtgui_CComposedGuiComp_included


// Qt includes
#include <QWidget>
#include <QSplitter>


// ACF includes
#include "istd/CString.h"

#include "iser/IFileLoader.h"

#include "iprm/IParamsSet.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"


namespace iqtgui
{


/**
	Component for the layouting/grouping of the UI-subcomponents.

	Five different types of the layout/grouping are provided:\n
	1. Vertical or horizontal placing of components without any visible container\n
	2. Vertical or horizontal placing of components in a labeled group box\n
	3. Vertical or horizontal placing of component in a splitted view\n
	4. Tool box\n
	5. Tab widget\n
*/
class CComposedGuiComp:
			public iqtgui::TRestorableGuiWrap<
						iqtgui::TGuiComponentBase<QWidget> >
{
public:
	typedef iqtgui::TRestorableGuiWrap<
				iqtgui::TGuiComponentBase<QWidget> > BaseClass;

	I_BEGIN_COMPONENT(CComposedGuiComp);
		I_ASSIGN_MULTI_0(m_guisCompPtr, "Guis", "List of GUI's", true);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of of gui names", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout", true, false);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - simple\n* 1 - tool box\n* 2 - tab\n* 3 - splitter", true, false);
		I_ASSIGN(m_settingsKeyAttrPtr, "SettingsKey", "Key for saving/restoring of the layout information in the registry", false, "SettingsKey");
	I_END_COMPONENT;

	CComposedGuiComp();

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	/**
		Create a key for saving/restoring of the layout information over settings provider.
	*/
	QString GetSettingsKey() const;

private:
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_MULTIATTR(istd::CString, m_namesAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(istd::CString, m_settingsKeyAttrPtr);

	QSplitter* m_splitterPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CComposedGuiComp_included


