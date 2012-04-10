#include "icmpstr/CMultiAttributeDelegateWidget.h"


// Qt includes
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QFocusEvent>


// ACF includes
#include "icmpstr/CMultiAttributeEditor.h"


// public methods
	
namespace icmpstr
{


// public methods of embedded class CMultiAttributeDelegateWidget

CMultiAttributeDelegateWidget::CMultiAttributeDelegateWidget(
			QItemDelegate& itemDelegate,
			const CElementSelectionInfoManagerBase& elementSelectionInfoManager,
			QWidget* parentWidget,
			const QByteArray& attributeId,
			int attributeFlags)
	:BaseClass(parentWidget),
	m_itemDelegate(itemDelegate),
	m_attributeId(attributeId),
	m_attributeFlags(attributeFlags),
	m_elementSelectionInfoManager(elementSelectionInfoManager)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	m_textEditor = new QLineEdit(this);
	setFocusProxy(m_textEditor);
	setFocusPolicy(Qt::StrongFocus);

	QPushButton* editButton = new QPushButton(this);
	editButton->setMaximumSize(QSize(19, 19));
	editButton->setText("...");
	connect(editButton, SIGNAL(clicked()), this, SLOT(OnShowDialog()));

	layout->addWidget(m_textEditor);
	layout->addWidget(editButton);

	setAutoFillBackground(true);
}


QString CMultiAttributeDelegateWidget::GetText() const
{
	return m_textEditor->text();
}


void CMultiAttributeDelegateWidget::SetText(const QString& text)
{
	m_textEditor->setText(text);

	m_textEditor->setCursorPosition(0);
}


// protected slots

void CMultiAttributeDelegateWidget::OnShowDialog()
{
	istd::TDelPtr<CMultiAttributeEditor> multiEditorDialogPtr(new CMultiAttributeEditor(m_elementSelectionInfoManager, m_attributeId, m_attributeFlags));
	multiEditorDialogPtr->setWindowTitle(m_attributeId);

	multiEditorDialogPtr->SetEditorText(m_textEditor->text());

	if (multiEditorDialogPtr->exec() == QDialog::Accepted){
		SetText(multiEditorDialogPtr->GetAttributeValues().join(";"));

		// clear own focus:
		clearFocus();

		// reset focus to the parent widget
		I_ASSERT(parentWidget() != NULL);
		parentWidget()->setFocus();

		// Send focus out event, to enforce the closing of the attribute editor:
		QFocusEvent e(QEvent::FocusOut);
		
		QApplication::sendEvent(this, &e);
	}
}


} // namespace icmpstr


