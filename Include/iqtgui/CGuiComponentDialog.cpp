#include <iqtgui/CGuiComponentDialog.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtGui/QCloseEvent>

#if QT_VERSION > QT_VERSION_CHECK(5, 15, 0)
#include <QtGui/QScreen>
#else
#include <QtWidgets/QDesktopWidget>
#endif

#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#endif


namespace iqtgui
{


// public methods

CGuiComponentDialog::CGuiComponentDialog(
			iqtgui::IGuiObject* guiObjectPtr,
			int buttons,
			bool isModal,
			QWidget* parentWidgetPtr)
	:BaseClass(parentWidgetPtr, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
	m_buttonsBox(NULL),
	m_guiObjectPtr(NULL),
	m_screenFactorX(0.0),
	m_screenFactorY(0.0),
	m_screenPositionPtr(NULL)
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

		if (buttons >= 0){
			m_buttonsBox = new QDialogButtonBox(QDialogButtonBox::StandardButtons(buttons), Qt::Horizontal, this);
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



void CGuiComponentDialog::SetDialogGeometry(double screenFactor, const QPoint* positionPtr)
{
	m_screenFactorX = screenFactor;
	m_screenFactorY = screenFactor;

	m_screenPositionPtr = positionPtr;
}


void CGuiComponentDialog::SetDialogGeometry(double screenFactorX, double screenFactorY, const QPoint* positionPtr)
{
	m_screenFactorX = screenFactorX;
	m_screenFactorY = screenFactorY;

	m_screenPositionPtr = positionPtr;
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

		int currentResult = BaseClass::result();
		if (currentResult != 0){
			done(currentResult);

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


void CGuiComponentDialog::showEvent(QShowEvent* eventPtr)
{
	BaseClass::showEvent(eventPtr);

	BaseClass::adjustSize();

	QRect screenRect;
#if QT_VERSION > QT_VERSION_CHECK(5, 15, 0)
	QScreen* screenPtr = nullptr;

	if (m_guiObjectPtr != nullptr){
		screenPtr = m_guiObjectPtr->GetWidget()->screen();
	}
	else{
		screenPtr = screen();
	}

	if (screenPtr != nullptr){
		screenRect = screenPtr->geometry();
	}
#else
	const QDesktopWidget* desktopPtr = QApplication::desktop();
	Q_ASSERT(desktopPtr != NULL);

	screenRect = desktopPtr->screenGeometry((m_guiObjectPtr != NULL) ? m_guiObjectPtr->GetWidget() : NULL);
#endif

	if (m_screenFactorX > 0 && m_screenFactorY > 0){
		double screenFactorX = qMin(0.99, m_screenFactorX);
		double screenFactorY = qMin(0.99, m_screenFactorY);

		BaseClass::resize(int(screenRect.width() * screenFactorX), int(screenRect.height() * screenFactorY));
	}
	else{
		BaseClass::resize(sizeHint());
	}

	if (m_screenPositionPtr == NULL){
		QSize dialogHalfSize = size() / 2;
	
		BaseClass::move(screenRect.center() - QPoint(dialogHalfSize.width(), dialogHalfSize.height()));
	}
	else{
		BaseClass::move(*m_screenPositionPtr);
	}
}


} // namespace iqtgui


