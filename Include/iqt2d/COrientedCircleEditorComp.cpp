#include <iqt2d/COrientedCircleEditorComp.h>


namespace iqt2d
{


COrientedCircleEditorComp::COrientedCircleEditorComp()
:	m_reversePolarityAction(QIcon(":/Icons/Reverse"), "", this)
{

}


// protected methods

// reimplemented (iqt2d::TCircleBasedParamsGuiComp)

bool COrientedCircleEditorComp::PopulateActions(CActionAdapter& host, imod::IModel* modelPtr)
{
	if (!BaseClass::PopulateActions(host, modelPtr)){
		return false;
	}

	i2d::COrientedCircle* circlePtr = dynamic_cast<i2d::COrientedCircle*>(modelPtr);
	if (circlePtr != NULL){
		host.AddAction(m_reversePolarityAction);
	}

	return true;
}


// reimplemented (iqtgui::CGuiComponentBase)

void COrientedCircleEditorComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	m_reversePolarityAction.setText(QCoreApplication::translate("iqt2d", "Reverse orientation"));
}


// protected slots

void COrientedCircleEditorComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void COrientedCircleEditorComp::OnActionTriggered(QAction* actionPtr)
{
	if (actionPtr == &m_reversePolarityAction){
		i2d::COrientedCircle* circlePtr = dynamic_cast<i2d::COrientedCircle*>(BaseClass::GetObservedModel());
		if (circlePtr){
			circlePtr->SetOrientedOutside(!circlePtr->IsOrientedOutside());
		}

		return;
	}

	BaseClass::OnActionTriggered(actionPtr);
}


} // namespace iqt2d


