#ifndef iqtprm_CButtonBasedSelectionParamGuiComp_included
#define iqtprm_CButtonBasedSelectionParamGuiComp_included


// Qt includes
#include <QtGui/QCommandLinkButton>

// ACF includes
#include "iprm/ISelectionParam.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "Generated/ui_CButtonBasedSelectionParamGuiComp.h"


namespace iqtprm
{

/**
	QCommandButton-based editor for an exclusive option selection.
*/
class CButtonBasedSelectionParamGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CButtonBasedSelectionParamGuiComp, iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CButtonBasedSelectionParamGuiComp, iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CButtonBasedSelectionParamGuiComp);
		I_ASSIGN(m_iconProviderCompPtr, "IconProvider", "Provider of the button icons", false, "IconProvider");
		I_ASSIGN(m_fontSizeAttrPtr, "FontSize", "Font Size", true, 8);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Icon Size", true, 16);
		I_ASSIGN(m_compactDescriptionAttrPtr, "CompactDescription", "Show description for checked buttons only", true, false);
		I_ASSIGN(m_layoutTypeAttrPtr, "LayoutType", "0 - horizontal\n1 - vertical\n2 and more - number of columns", true, 1);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnButtonClicked(int index);

private:
	I_REF(iqtgui::IIconProvider, m_iconProviderCompPtr);
	I_ATTR(int, m_fontSizeAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_compactDescriptionAttrPtr);
	I_ATTR(int, m_layoutTypeAttrPtr);

	QButtonGroup m_buttonsGroup;
};


} // namespace iqtprm


#endif // !iqtprm_CButtonBasedSelectionParamGuiComp_included


