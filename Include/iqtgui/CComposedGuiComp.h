#ifndef iqtgui_CComposedGuiComp_included
#define iqtgui_CComposedGuiComp_included


// Qt includes
#include <QtCore/QString>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#else
#include <QtGui/QWidget>
#include <QtGui/QSplitter>
#endif

// ACF includes
#include "iprm/IParamsSet.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CMultiPageGuiCompBase.h"


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
	6. Stacked widget
*/
class CComposedGuiComp:
			public iqtgui::TRestorableGuiWrap<CMultiPageGuiCompBase>
{
public:
	typedef iqtgui::TRestorableGuiWrap<CMultiPageGuiCompBase> BaseClass;

	I_BEGIN_COMPONENT(CComposedGuiComp);
		I_ASSIGN_MULTI_0(m_guisCompPtr, "Guis", "List of GUI's", true);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - Simple (Group Box)\n* 1 - Tool Box\n* 2 - Tab Bar\n* 3 - Splitter\n* 4 - Stacked", true, false);
		I_ASSIGN(m_settingsKeyAttrPtr, "SettingsKey", "Key for saving/restoring of the layout information in the registry", false, "SettingsKey");
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar for tab design\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_flatViewAttrPtr, "FlatView", "Tries to apply flat view to the design if possible (Group Box: without frame; Tab Bar: triangular tabs; Slider: transparent grip)", true, false);
	I_END_COMPONENT;

	QWidget* GetPageContainerWidget() const;

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// reimplemented (iqtgui::CMultiPageGuiCompBase)
	virtual int GetPagesCount() const;
	virtual iqtgui::IGuiObject* GetPageGuiComponent(int pageIndex) const;
	virtual int GetDesignType() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	/**
		Create a key for saving/restoring of the layout information over settings provider.
	*/
	QString GetSettingsKey() const;

private:
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(QString, m_settingsKeyAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(bool, m_flatViewAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CComposedGuiComp_included


