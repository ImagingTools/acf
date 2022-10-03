#include <iqtgui/CDialogGuiComp.h>


// Qt includes
#include <QtGui/QIcon>
#include <QtCore/QtGlobal>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QPushButton>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif




namespace iqtgui
{


// public methods

CDialogGuiComp::CDialogGuiComp()
	:m_dialogCommand("Show Dialog", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR),
	m_dialogPtr(NULL)
{
	m_commandsProvider.SetParent(this);
}


// reimplemented (iqtgui::IDialog)

int CDialogGuiComp::ExecuteDialog(IGuiObject* parentPtr)
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog(*m_dialogButtonsAttrPtr, parentPtr));
	if (dialogPtr.IsValid()){
		if (*m_isModalAttrPtr){
			m_dialogPtr = dialogPtr.GetPtr();
		
			int retVal = dialogPtr->exec();
		
			m_dialogPtr = NULL;

			return retVal;
		}
		else{
			m_dialogPtr = dialogPtr.GetPtr();

			dialogPtr->setModal(false);
			m_dialogCommand.SetEnabled(false);

			dialogPtr->setAttribute(Qt::WA_DeleteOnClose);

			dialogPtr->installEventFilter(this);

			dialogPtr->show();

			dialogPtr.PopPtr();

			return QDialog::Accepted;
		}
	}

	return QDialog::Rejected;
}


// protected methods

iqtgui::CGuiComponentDialog* CDialogGuiComp::CreateComponentDialog(int buttons, IGuiObject* parentPtr) const
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_guiCompPtr.IsValid()){
		QWidget* parentWidgetPtr = (parentPtr != NULL)? parentPtr->GetWidget(): NULL;

		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_guiCompPtr.GetPtr(),
								buttons,
								true,
								parentWidgetPtr));

		dialogPtr->setWindowFlags((Qt::WindowFlags)*m_windowFlagsAttrPtr);

		if (m_dialogTitleAttrPtr.IsValid()){
			dialogPtr->setWindowTitle((*m_dialogTitleAttrPtr));
		}
		else{
			dialogPtr->setWindowTitle(QCoreApplication::applicationName());
		}

		if (m_dialogIconPathAttrPtr.IsValid()){
			dialogPtr->setWindowIcon(QIcon(*m_dialogIconPathAttrPtr));
		}
		else{
			dialogPtr->setWindowIcon(QApplication::windowIcon());
		}

		if (m_initialDialogSizeXAttrPtr.IsValid() && m_initialDialogSizeYAttrPtr.IsValid()){
			dialogPtr->SetDialogGeometry(*m_initialDialogSizeXAttrPtr, *m_initialDialogSizeYAttrPtr);
		}
		else{
			dialogPtr->SetDialogGeometry(m_initialDialogSizeAttrPtr.IsValid() ? *m_initialDialogSizeAttrPtr : 0.0);
		}

		if (*m_defaultButtonAttrPtr != 0){
			QDialogButtonBox* boxPtr = const_cast<QDialogButtonBox*>(dialogPtr->GetButtonBoxPtr());
			if (boxPtr != nullptr){
				QList<QAbstractButton*> buttons = boxPtr->buttons();
				for (QAbstractButton* buttonPtr : buttons){
					QPushButton* pushButtonPtr = dynamic_cast<QPushButton*>(buttonPtr);
					if (pushButtonPtr != nullptr){
						pushButtonPtr->setDefault(false);
						pushButtonPtr->setAutoDefault(false);
					}
				}

				QPushButton* pushButtonPtr = boxPtr->button((QDialogButtonBox::StandardButton)*m_defaultButtonAttrPtr);
				if (pushButtonPtr != nullptr){
					pushButtonPtr->setDefault(true);
					if (!(*m_defaultButtonPropertyAttrPtr).isEmpty()){
						pushButtonPtr->setProperty(*m_defaultButtonPropertyAttrPtr, true);

						qApp->style()->polish(dialogPtr.GetPtr());
					}
				}
			}
		}
	}

	return dialogPtr.PopPtr();
}


void CDialogGuiComp::OnRetranslate()
{
	if (m_dialogPtr != NULL){
		if (m_dialogTitleAttrPtr.IsValid()){
			m_dialogPtr->setWindowTitle((*m_dialogTitleAttrPtr));
		}
		else{
			m_dialogPtr->setWindowTitle(QCoreApplication::applicationName());
		}
	}

	istd::CChangeNotifier changePtr(&m_commandsProvider);

	QIcon commandIcon;
	if (m_dialogIconPathAttrPtr.IsValid()){
		QString iconPath = *m_dialogIconPathAttrPtr;

		commandIcon = QIcon(iconPath);
	}

	m_rootMenuCommand.SetName(*m_rootMenuNameAttrPtr);
	m_dialogCommand.SetVisuals(*m_menuNameAttrPtr, *m_menuNameAttrPtr, *m_menuDescriptionAttrPtr, commandIcon);
}


// reimplemented (QObject)

bool CDialogGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	Q_ASSERT(eventPtr != NULL);
	Q_ASSERT(sourcePtr != NULL);

	if ((eventPtr->type() == QEvent::LanguageChange) && (sourcePtr == qApp)){
		OnRetranslate();
	}

	if (sourcePtr == m_dialogPtr){
		if (eventPtr->type() == QEvent::Close){
			m_dialogCommand.setEnabled(true);
		}

		if (eventPtr->type() == QEvent::Hide){
			m_dialogCommand.setEnabled(true);
		}
	}

	return BaseClass2::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (icomp::CComponentBase)

void CDialogGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_rootMenuCommand.SetName(*m_rootMenuNameAttrPtr);

	QIcon commandIcon;
	if (m_dialogIconPathAttrPtr.IsValid()){
		QString iconPath = *m_dialogIconPathAttrPtr;

		commandIcon = QIcon(iconPath);
	}

	m_dialogCommand.SetVisuals(*m_menuNameAttrPtr, *m_menuNameAttrPtr, *m_menuDescriptionAttrPtr, commandIcon);
	
	m_rootMenuCommand.InsertChild(&m_dialogCommand);
	m_rootCommand.InsertChild(&m_rootMenuCommand);

	connect(&m_dialogCommand, SIGNAL(triggered()), this, SLOT(OnCommandActivated()));

	qApp->installEventFilter(this);
}


// protected slots

void CDialogGuiComp::OnCommandActivated()
{
	ExecuteDialog(NULL);
}


// public methods of the embedded class CommandsProvider

CDialogGuiComp::CommandsProvider::CommandsProvider()
	:m_parentPtr(NULL)
{
}


void CDialogGuiComp::CommandsProvider::SetParent(CDialogGuiComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CDialogGuiComp::CommandsProvider::GetCommands() const
{
	Q_ASSERT(m_parentPtr != NULL);

	return &m_parentPtr->m_rootCommand;
}


} // namespace iqtgui


