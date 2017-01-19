#ifndef iqtprm_CSelectionParamIndexGuiComp_included
#define iqtprm_CSelectionParamIndexGuiComp_included


#include <iprm/ISelectionParam.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_CSelectionParamIndexGuiComp.h>


namespace iqtprm
{


class CSelectionParamIndexGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSelectionParamIndexGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectionParamIndexGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamIndexGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_OptionIndexSlider_valueChanged(int value);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectionParamIndexGuiComp_included


