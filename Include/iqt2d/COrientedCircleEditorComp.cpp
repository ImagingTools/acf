#include "iqt2d/COrientedCircleEditorComp.h"


namespace iqt2d
{


COrientedCircleEditorComp::COrientedCircleEditorComp()
:	m_reversePolarityAction(QIcon(":/Icons/Reverse"), QCoreApplication::translate("iqt2d", "Reverse orientation"), this)
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


