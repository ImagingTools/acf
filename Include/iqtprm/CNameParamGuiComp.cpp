#include <iqtprm/CNameParamGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtGui/QRegExpValidator>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLabel>
#else
#include <QtGui/QLabel>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CNameParamGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iprm::INameParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		objectPtr->SetName(NameEdit->text());
	}
}


void CNameParamGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	iprm::INameParam* objectPtr = GetObservedObject();
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
	Q_ASSERT(NameFrame->layout() == NULL);

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

	if (*m_updateOnEveryChangeAttrPtr){
		connect(NameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnNameChanged(const QString&)));
	}
	else{
		connect(NameEdit, SIGNAL(editingFinished()), this, SLOT(OnNameEdited()));
	}

	if (m_regularExpressionAttrPtr.IsValid()){
		QRegExp expression(*m_regularExpressionAttrPtr);

		QRegExpValidator* validatorPtr = new QRegExpValidator(expression, NameEdit);

		NameEdit->setValidator(validatorPtr);
	}
}


// private slots

void CNameParamGuiComp::OnNameChanged(const QString& /*text*/)
{
	DoUpdateModel();
}


void CNameParamGuiComp::OnNameEdited()
{
	DoUpdateModel();
}


} // namespace iqtprm


