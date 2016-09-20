#ifndef iqtprm_CEnableableParamGuiComp_included
#define iqtprm_CEnableableParamGuiComp_included


// ACF includes
#include <iprm/IEnableableParam.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_CEnableableParamGuiComp.h>


namespace iqtprm
{


class CEnableableParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CEnableableParamGuiComp,
			iprm::IEnableableParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CEnableableParamGuiComp,
				iprm::IEnableableParam> BaseClass;

	I_BEGIN_COMPONENT(CEnableableParamGuiComp);
		I_ASSIGN(m_enableableLabelAttrPtr, "Label", "Label for the enable/disable control", false, "");
	I_END_COMPONENT;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_EnableableCheck_stateChanged(int state);

private:
	I_ATTR(QString, m_enableableLabelAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CEnableableParamGuiComp_included


