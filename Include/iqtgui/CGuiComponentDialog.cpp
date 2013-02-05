#include "iqtgui/CGuiComponentDialog.h"


// Qt includes
#include <QtGui/QVBoxLayout>
#include <QtGui/QCloseEvent>


namespace iqtgui
{


CGuiComponentDialog::CGuiComponentDialog(
			iqtgui::IGuiObject* guiObjectPtr,
			int buttons,
			bool isModal,
			QWidget* parentWidgetPtr)
:	BaseClass(parentWidgetPtr),
	m_buttonsBox(NULL),
	m_guiObjectPtr(NULL)
{
	// GUI pointer must be valid:
	Q_ASSERT(guiObjectPtr != NULL);

	// GUI must not be created:
	Q_ASSERT(!guiObjectPtr->IsGuiCreated());
	if (!guiObjectPtr->IsGuiCreated()){
		BaseClass::setModal(isModal);

		QVBoxLayout* dialogLayout = new QVBoxLayout(this);

		if (guiObjectPtr != NULL){
			if (guiObjectPtr->CreateGui(this)){
				m_guiObjectPtr = guiObjectPtr;

				dialogLayout->addWidget(m_guiObjectPtr->GetWidget());

				QWidget* slaveWidgetPtr = guiObjectPtr->GetWidget();
				Q_ASSERT(slaveWidgetPtr != NULL);

				BaseClass::resize(slaveWidgetPtr->sizeHint().expandedTo(BaseClass::sizeHint()));
			}
		}

		if (isModal || (buttons != 0)){
			QDialogButtonBox::StandardButtons buttonFlags = (buttons != 0)?
						(QDialogButtonBox::StandardButtons)buttons:
						QDialogButtonBox::Close;

			m_buttonsBox = new QDialogButtonBox(buttonFlags, Qt::Horizontal, this);
			connect(m_buttonsBox, SIGNAL(accepted()), this, SLOT(accept()));
			connect(m_buttonsBox, SIGNAL(rejected()), this, SLOT(reject()));

			dialogLayout->addWidget(m_buttonsBox);
		}
	}
}


CGuiComponentDialog::~CGuiComponentDialog()
{
	if (m_guiObjectPtr != NULL){
		Q_ASSERT(m_guiObjectPtr->IsGuiCreated());

		if (m_guiObjectPtr->IsGuiCreated()){
			m_guiObjectPtr->DestroyGui();
		}
	}
}


// protected methods

// reimplemented (QWidget)

void CGuiComponentDialog::closeEvent(QCloseEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	if (m_guiObjectPtr != NULL){
		bool ignoreClosing = false;
		m_guiObjectPtr->OnTryClose(&ignoreClosing);

		if (ignoreClosing){
			  eventPtr->ignore();

			  return;
		}
	}

	BaseClass::closeEvent(eventPtr);
}


void CGuiComponentDialog::keyPressEvent(QKeyEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);

	switch (eventPtr->key()){
        case Qt::Key_Escape:
		if (m_guiObjectPtr != NULL){
			bool ignoreClosing = false;
			m_guiObjectPtr->OnTryClose(&ignoreClosing);
			if (ignoreClosing){
				  return;
			}

			int currentResult = BaseClass::result();
			if (currentResult != 0){
				done(currentResult);

				return;
			}			
		}
		break;
	}

	BaseClass::keyPressEvent(eventPtr);
}


} // namespace iqtgui


