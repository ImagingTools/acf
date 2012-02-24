#include "iqtprm/CNameParamGuiComp.h"


// Qt includes
#include <QLabel>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CNameParamGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iprm::INameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetName(NameEdit->text());
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CNameParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	iprm::INameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		NameEdit->setText(objectPtr->GetName());
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CNameParamGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	int labelPosition = LP_LEFT;

	if (m_labelPositionAttrPtr.IsValid()){
		labelPosition = *m_labelPositionAttrPtr;
	}

	QLayout* selectorLayoutPtr = NULL;
	I_ASSERT(NameFrame->layout() == NULL);

	if (labelPosition == LP_LEFT){
		selectorLayoutPtr = new QHBoxLayout(NameFrame);

	}
	else{
		selectorLayoutPtr = new QVBoxLayout(NameFrame);			
	}

	selectorLayoutPtr->setContentsMargins(0, 0, 0, 0);

	if (m_labelAttrPtr.IsValid()){
		QLabel* selectorLabelPtr = new QLabel(NameFrame);
		selectorLabelPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
		selectorLabelPtr->setText(*m_labelAttrPtr);

		selectorLayoutPtr->addWidget(selectorLabelPtr);
	}

	selectorLayoutPtr->addWidget(NameEdit);


	connect(NameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnNameChanged(const QString&)));
}


// private slots

void CNameParamGuiComp::OnNameChanged(const QString& /*text*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


} // namespace iqtprm


