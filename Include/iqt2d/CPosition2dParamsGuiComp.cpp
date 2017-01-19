#include <iqt2d/CPosition2dParamsGuiComp.h>


// ACF includes
#include <istd/CChangeGroup.h>


namespace iqt2d
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPosition2dParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPosition2d* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	objectPtr->SetPosition(postion);
}


void CPosition2dParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPosition2d* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();
		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		UpdateAllViews();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPosition2dParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	XLabel->setVisible(m_xLabelStrAttrPtr.IsValid());
	YLabel->setVisible(m_yLabelStrAttrPtr.IsValid());

	if (IsPositionFixed()){
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
	}
	else{
		QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}
}


void CPosition2dParamsGuiComp::OnGuiDestroyed()
{
	if (!IsPositionFixed()){
		QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	BaseClass::OnGuiDestroyed();
}


void CPosition2dParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (m_altrnativeStrAttrPtr.IsValid()){
		PositionLabel->setText(*m_altrnativeStrAttrPtr);
	}

	if (m_xLabelStrAttrPtr.IsValid()){
		XLabel->setText(*m_xLabelStrAttrPtr);
	}

	if (m_yLabelStrAttrPtr.IsValid()){
		YLabel->setText(*m_yLabelStrAttrPtr);
	}

	const QString& unitName = GetUnitName();

	if (!unitName.isEmpty()){
		PositionUnitLabel->setText(unitName);
		PositionUnitLabel->setVisible(true);
	}
	else{
		PositionUnitLabel->setVisible(false);
	}
}


// protected slots

void CPosition2dParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CPosition2dParamsGuiComp::OnActionTriggered(QAction* /*actionPtr*/)
{
	// dummy implementation, to be reimplemented
}


} // namespace iqt2d


