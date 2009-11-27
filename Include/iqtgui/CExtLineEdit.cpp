#include "iqtgui/CExtLineEdit.h"


// Qt includes
#include <QResizeEvent>


// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqtgui
{


// public methods

CExtLineEdit::CExtLineEdit(
				const QString& startupText,
				int margins,
				QWidget* parent)
	:BaseClass(parent),
	m_margins(margins)
{
	m_iconPtr = new QLabel(this);

	m_iconPtr->move(m_margins, 0);
	m_iconPtr->setCursor(QCursor(Qt::ArrowCursor));

	m_iconPtr->hide();

	SetStartupText(startupText);

	connect(this, SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));
	connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(OnTextChanged(const QString&)));
}


void CExtLineEdit::SetStartupText(const QString& startupText)
{
	m_startupText = startupText;

	UpdateStartupText();
}


QString CExtLineEdit::GetStartupText() const
{
	return m_startupText;
}


void CExtLineEdit::SetIcon(const QIcon& icon)
{
	if (!icon.isNull()){
		int lineEditHeight = height();
		m_iconPtr->setPixmap(icon.pixmap(lineEditHeight - m_margins, lineEditHeight - m_margins));
		m_iconPtr->setVisible(true);
	}
	else{
		m_iconPtr->setPixmap(QPixmap());
		m_iconPtr->setVisible(false);
	}

	UpdateLayout();
}


void CExtLineEdit::AddWidget(QWidget* widgetPtr, int alignmentFlags)
{
	if (alignmentFlags != Qt::AlignLeft && alignmentFlags != Qt::AlignRight){
		if (widgetPtr->parentWidget() == this){
			widgetPtr->hide();
		}

		return;
	}
	
	if (widgetPtr->parentWidget() != this){
		widgetPtr->setParent(this);
	}

	widgetPtr->setStyleSheet("QWidget{border: 0px}");
	widgetPtr->setCursor(QCursor(Qt::ArrowCursor));

	SubWidgetInfo subWidgetInfo;
	subWidgetInfo.widget = widgetPtr;
	subWidgetInfo.margin = m_margins;
	subWidgetInfo.alignmentFlags = alignmentFlags;

	m_subWidgets.push_back(subWidgetInfo);

	UpdateLayout();
}


// protected methds

// reimplemented (QWidget)

void CExtLineEdit::resizeEvent(QResizeEvent* resizeEvent)
{
	BaseClass::resizeEvent(resizeEvent);

	UpdateLayout();
}


void CExtLineEdit::focusInEvent(QFocusEvent* focusEvent)
{
	BaseClass::focusInEvent(focusEvent);

	iqt::CSignalBlocker block(this);

	if (text() == m_startupText){
		clear();

		setStyleSheet(QString());
	}
}


// private slots

void CExtLineEdit::OnEditingFinished()
{
	UpdateStartupText();
}


void CExtLineEdit::OnTextChanged(const QString& /*text*/)
{
	if (!hasFocus()){
		UpdateStartupText();
	}
}


// private methods

void CExtLineEdit::UpdateLayout()
{
	int lineEditHeight = contentsRect().height();
	int contentsWidth = contentsRect().width();

	int subWidgetHeight = lineEditHeight - m_margins;
	int iconSize = (m_iconPtr->pixmap() != NULL) ? m_iconPtr->pixmap()->width() : 0;

	int lastLeftPos = iconSize > 0 ? iconSize + m_margins : 0;
	int lastRightPos = contentsWidth;

	for (int widgetIndex = 0; widgetIndex < m_subWidgets.count(); widgetIndex++){
		SubWidgetInfo& info = m_subWidgets[widgetIndex];

		info.widget->resize(info.widget->minimumSizeHint().width(), subWidgetHeight);
		if (info.alignmentFlags == Qt::AlignLeft){
			info.widget->move(lastLeftPos + m_margins, 1);

			lastLeftPos += info.widget->width() + m_margins;
		}

		if (info.alignmentFlags == Qt::AlignRight){
			lastRightPos -= (info.widget->width() + m_margins);
			
			info.widget->move(lastRightPos, 1);
		}
	}

	setTextMargins(lastLeftPos, 0, contentsWidth - lastRightPos, 0);
}


void CExtLineEdit::UpdateStartupText()
{
	if (!m_startupText.isEmpty() && text().isEmpty()){
		iqt::CSignalBlocker block(this);

		setText(m_startupText);

		setStyleSheet(QString("QLineEdit[text=\"%1\"]\n{\n color: rgb(192, 192,192)\n}").arg(m_startupText));
	}
	else{
		setStyleSheet(QString());	
	}
}


} // namespace iqtgui


