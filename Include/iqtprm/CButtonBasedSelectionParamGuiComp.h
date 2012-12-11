#ifndef iqtprm_CButtonBasedSelectionParamGuiComp_included
#define iqtprm_CButtonBasedSelectionParamGuiComp_included


// Qt includes
#include <QtGui/QCommandLinkButton>
#include <QtGui/QToolButton>
#include <QtGui/QRadioButton>

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
		I_ASSIGN(m_layoutTypeAttrPtr, "LayoutType", "0 - horizontal\n1 - vertical\n2 and more - number of columns", true, 1);
		I_ASSIGN(m_descriptionTypeAttrPtr, "DescriptionType", "0 - No description visible\n1 - Default (description is alvays visible)\n2 - Compact (description is visible for checked button only)", true, 1);
		I_ASSIGN(m_showTooltipAttrPtr, "DescriptionAsTooltip", "Show description as tooltip", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnButtonClicked(int index);

private:
	struct SelectionInfo{
		SelectionInfo* parentInfoPtr;
		iprm::ISelectionParam* paramPtr;
		int index;
	};

	QList<SelectionInfo*> m_selectionInfos;
	QList<SelectionInfo*> m_allSelectionInfos;

	void CreateButtons(const iprm::ISelectionParam* paramPtr, SelectionInfo* parentSelectionInfoPtr, int& totalButtons);
	QAbstractButton* CreateButton(QWidget* parentPtr, const QString& description);

private:
	I_REF(iqtgui::IIconProvider, m_iconProviderCompPtr);
	I_ATTR(int, m_fontSizeAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_showTooltipAttrPtr);
	I_ATTR(int, m_layoutTypeAttrPtr);
	I_ATTR(int, m_descriptionTypeAttrPtr);

	QButtonGroup m_buttonsGroup;
};


} // namespace iqtprm


#endif // !iqtprm_CButtonBasedSelectionParamGuiComp_included


