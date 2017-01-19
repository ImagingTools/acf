#ifndef iqtprm_CNameParamGuiComp_included
#define iqtprm_CNameParamGuiComp_included


// ACF includes
#include <iprm/INameParam.h>

#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/iqtprm/ui_CNameParamGuiComp.h>


namespace iqtprm
{


class CNameParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CNameParamGuiComp,
			iprm::INameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CNameParamGuiComp,
				iprm::INameParam> BaseClass;
	/**
		Command label position
	*/
	enum LabelPosition
	{
		/**
			Left from the name field
		*/
		LP_LEFT,

		/**
			On top of the name field
		*/
		LP_TOP
	};

	I_BEGIN_COMPONENT(CNameParamGuiComp);
		I_ASSIGN(m_labelAttrPtr, "Label", "Label for the named object", false, "");
		I_ASSIGN(m_labelPositionAttrPtr, "LabelPosition", "Selection label position. 0 - Left from the selector,\n1 - On top of the selector", false, LP_LEFT);
		I_ASSIGN(m_regularExpressionAttrPtr, "InputFilter", "Input filter in form of a regular expression", false, "");
		I_ASSIGN(m_updateOnEveryChangeAttrPtr, "UpdateOnEveryTextChange", "If set the model will be updated after every text change, else only if Enter pressed or focus lost", true, true);
	I_END_COMPONENT;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private Q_SLOTS:
	void OnNameChanged(const QString& text);
	void OnNameEdited();

private:
	I_TEXTATTR(m_labelAttrPtr);
	I_ATTR(int, m_labelPositionAttrPtr);
	I_ATTR(QByteArray, m_regularExpressionAttrPtr);
	I_ATTR(bool, m_updateOnEveryChangeAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CNameParamGuiComp_included


