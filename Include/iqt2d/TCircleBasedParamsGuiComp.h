#ifndef iqt2d_TCircleBasedParamsGuiComp_included
#define iqt2d_TCircleBasedParamsGuiComp_included


// ACF includes
#include "iqt2d/TShapeParamsGuiCompBase.h"

#include "GeneratedFiles/iqt2d/ui_CCircleParamsGuiComp.h"


namespace iqt2d
{


template <class CircleBasedShape, class CircleBasedModel>
class TCircleBasedParamsGuiComp: 
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CCircleParamsGuiComp,
						CircleBasedShape,
						CircleBasedModel>
{
public:
	typedef iqt2d::TShapeParamsGuiCompBase<
		Ui::CCircleParamsGuiComp,
		CircleBasedShape,
		CircleBasedModel> BaseClass;

	I_BEGIN_BASE_COMPONENT(TCircleBasedParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

	// reimplemented (iqt2d::TShapeParamsGuiCompBase)
	virtual void OnActionTriggered(QAction* actionPtr);

protected:
	using BaseClass::GetObservedObject;
	using BaseClass::GetObservedModel;
	using BaseClass::DoUpdateModel;
	using BaseClass::UpdateAllViews;
	using BaseClass::IsPositionFixed;
	using BaseClass::CreateToolsMenu;
	using BaseClass::tr;
	using BaseClass::GetQtWidget;
	using BaseClass::XSpin;
	using BaseClass::YSpin;
	using BaseClass::RadiusSpin;
	using BaseClass::ToolsButton;
	using BaseClass::GetUnitName;
	using BaseClass::PositionUnitLabel;
	using BaseClass::RadiusUnitLabel;
};


// public methods

// reimplemented (imod::IModelEditor)

template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	CircleBasedModel* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	objectPtr->SetPosition(postion);

	double radius = RadiusSpin->value();
	objectPtr->SetRadius(radius);
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();
}


template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	CircleBasedModel* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();

		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		RadiusSpin->setValue(objectPtr->GetRadius());

		UpdateAllViews();
	}

	// tools actions
	CreateToolsMenu(ToolsButton);
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (IsPositionFixed()){
		XSpin->setEnabled(false);
		YSpin->setEnabled(false);
	}
	else{
		QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
		QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	}

	QObject::connect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QString unitName = GetUnitName();

	if (!unitName.isEmpty()){
		PositionUnitLabel->setText(unitName);
		RadiusUnitLabel->setText(unitName);
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setVisible(true);
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


// reimplemented (iqt2d::TShapeParamsGuiCompBase)

template <class CircleBasedShape, class CircleBasedModel>
void TCircleBasedParamsGuiComp<CircleBasedShape, CircleBasedModel>::OnActionTriggered(QAction* /*actionPtr*/)
{
}


} // namespace iqt2d


#endif // !iqt2d_TCircleBasedParamsGuiComp_included


