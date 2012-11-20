#ifndef iqtprm_COptionsManagerGuiComp_included
#define iqtprm_COptionsManagerGuiComp_included


// Qt includes
#include <QtGui/QComboBox>


// ACF includes
#include "istd/TPointerVector.h"
#include "imod/CMultiModelDispatcherBase.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsManager.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtprm/Generated/ui_COptionsManagerGuiComp.h"


namespace iqtprm
{


/**
	Simple editor for an exclusive option selection.
*/
class COptionsManagerGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::COptionsManagerGuiComp, iprm::ISelectionParam>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsManagerGuiComp, iprm::ISelectionParam> BaseClass;

	/**
		Command label position
	*/
	enum LabelPosition
	{
		/**
			Left from the selector
		*/
		LP_LEFT,

		/**
			On top of the selector
		*/
		LP_TOP
	};

	I_BEGIN_COMPONENT(COptionsManagerGuiComp);
		I_ASSIGN(m_optionsLabelAttrPtr, "OptionsLabel", "Command label for the options selector", false, "Select");
		I_ASSIGN(m_infoLabelAttrPtr, "InfoLabel", "Information label for the options selector", false, "Info");
		I_ASSIGN(m_infoIconProviderCompPtr, "InfoIconProvider", "Provider of the info icon", false, "InfoIconProvider");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of the used icons", false, 32);
		I_ASSIGN(m_labelPositionAttrPtr, "LabelPosition", "Selection label position. 0 - Left from the selector,\n1 - On top of the selector", false, LP_LEFT);
	I_END_COMPONENT;

	COptionsManagerGuiComp();
			
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiShown();
	virtual void OnGuiRetranslate();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

protected Q_SLOTS:
	void OnSelectionChanged(int index);
	void OnEditingFinished();
	void OnTextEdited(const QString& text);
	void OnTextChanged(const QString& text);

private:
	void UpdateComboBox();
	void UpdateDescriptionFrame();

private:
	I_ATTR(QString, m_optionsLabelAttrPtr);
	I_ATTR(QString, m_infoLabelAttrPtr);
	I_ATTR(int, m_labelPositionAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_REF(iqtgui::IIconProvider, m_infoIconProviderCompPtr);

	/**
		Flag to indicate that the option text is currently editing.
	*/
	bool m_isEditingFlag;
	istd::TDelPtr<QLabel> m_selectorLabelPtr;
};


} // namespace iqtprm


#endif // !iqtprm_COptionsManagerGuiComp_included


