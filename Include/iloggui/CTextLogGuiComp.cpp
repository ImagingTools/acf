#include "iloggui/CTextLogGuiComp.h"


//#define PERFORMANCE_TEST


// Qt includes
#include <QtGui/QTextDocument>
#include <QtGui/QTextTable>

#ifdef PERFORMANCE_TEST
#include <QtCore/QElapsedTimer>
#endif


namespace iloggui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CTextLogGuiComp::UpdateGui(int /*updateFlags*/)
{
	UpdateFilters();

	DoFilter();
}


// reimplemented (CGuiComponentBase)

void CTextLogGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_tableFormat.setBorder(0);
	m_tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
	m_tableFormat.setCellPadding(2);
	m_tableFormat.setCellSpacing(0);

	m_okCellFormat.setRightPadding(8);

	connect(SeverityFilterCB, SIGNAL(currentIndexChanged(int)), this, SLOT(DoFilter()));
	connect(SourceFilterCB, SIGNAL(currentIndexChanged(int)), this, SLOT(DoFilter()));
	connect(FilterText, SIGNAL(textChanged(QString)), this, SLOT(DoFilter()));
}


void CTextLogGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


void CTextLogGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	SeverityFilterCB->setItemIcon(0, GetCategoryIcon(istd::IInformationProvider::IC_NONE));
	SeverityFilterCB->setItemIcon(1, GetCategoryIcon(istd::IInformationProvider::IC_INFO));
	SeverityFilterCB->setItemIcon(2, GetCategoryIcon(istd::IInformationProvider::IC_WARNING));
	SeverityFilterCB->setItemIcon(3, GetCategoryIcon(istd::IInformationProvider::IC_ERROR));
}


// private slots

void CTextLogGuiComp::DoFilter()
{
	GenerateDocument(
		SeverityFilterCB->currentIndex(),
		SourceFilterCB->currentIndex() == 0 ? "" : SourceFilterCB->currentText(), 
		FilterText->text());
}


// private members

void CTextLogGuiComp::UpdateFilters()
{
	SourceFilterCB->blockSignals(true);

	SourceFilterCB->clear();
	SourceFilterCB->addItem(tr("Any"));

	ilog::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		QSet<QString> sources;
		const ilog::IMessageContainer::Messages messages = objectPtr->GetMessages();
		int messagesCount = messages.count();
		for (int i = 0; i < messagesCount; i++){
			const ilog::IMessageConsumer::MessagePtr messagePtr = messages.at(i);
			sources.insert(messagePtr->GetInformationSource());
		}

		if (!sources.isEmpty()){
			SourceFilterCB->addItems(sources.toList());
		}
	}

	SourceFilterCB->blockSignals(false);
}


void CTextLogGuiComp::GenerateDocument(int severityFilter, const QString& sourceFilter, const QString& textFilter)
{
#ifdef PERFORMANCE_TEST
	QElapsedTimer timer;
	timer.start();
#endif

	LogEditor->setUpdatesEnabled(false);

	LogEditor->setPlainText("");

	ilog::IMessageContainer* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		LogEditor->setUpdatesEnabled(true);

		return;
	}

	const ilog::IMessageContainer::Messages messages = objectPtr->GetMessages();
	int messagesCount = messages.count();

	QTextDocument* documentPtr = LogEditor->document();

	QTextCursor textCursor(documentPtr);
	textCursor.beginEditBlock();

	QTextTable* tablePtr = textCursor.insertTable(1, 3, m_tableFormat);

	for (int i = 0; i < messagesCount; i++){
		int column = 0;

		const ilog::IMessageConsumer::MessagePtr messagePtr = messages.at(i);

		// filter the message
		int category = messagePtr->GetInformationCategory();
		if (category < severityFilter){
			continue;
		}

		if (!sourceFilter.isEmpty() && sourceFilter != messagePtr->GetInformationSource()){
			continue;
		}

		QString text(messagePtr->GetInformationDescription());
		if (!textFilter.isEmpty() && text.indexOf(textFilter, 0, Qt::CaseInsensitive) < 0){
			continue;
		}

		QTextTableCellFormat& cellFormat = m_okCellFormat;

		int tableRow = tablePtr->rows()-1;

		// category
		QImage categoryIcon = GetCategoryImage(category);		
		InsertImage(tablePtr->cellAt(tableRow, column++), categoryIcon);

		// timestamp
		InsertText(tablePtr->cellAt(tableRow, column++), messagePtr->GetInformationTimeStamp().toString(), cellFormat);

		// text
		InsertText(tablePtr->cellAt(tableRow, column++), messagePtr->GetInformationDescription(), cellFormat);

		// add message row
		tablePtr->appendRows(1);
	}

	textCursor.endEditBlock();

#ifdef PERFORMANCE_TEST
	textCursor.movePosition(QTextCursor::Start);
	textCursor.insertText(QString("Time: %1 ms").arg(timer.elapsed()));
#endif

	LogEditor->setUpdatesEnabled(true);
}


void CTextLogGuiComp::InsertImage(QTextTableCell cell, const QImage& image)
{
	cell.firstCursorPosition().insertImage(image);
}


void CTextLogGuiComp::InsertText(QTextTableCell cell, const QString& text, const QTextTableCellFormat& cellFormat)
{
	cell.setFormat(cellFormat);

	cell.firstCursorPosition().insertText(text);
}


QIcon CTextLogGuiComp::GetCategoryIcon(int category) const
{
	static QIcon logIcon(":/Icons/Log");
	static QIcon infoIcon(":/Icons/Info.svg");
	static QIcon warningIcon(":/Icons/Warning.svg");
	static QIcon errorIcon(":/Icons/Error.svg");

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return logIcon;
	}
}


QImage CTextLogGuiComp::GetCategoryImage(int category) const
{
	static QImage logImage(":/Icons/Log");
	static QImage infoImage(":/Icons/Info.svg");
	static QImage warningImage(":/Icons/Warning.svg");
	static QImage errorImage(":/Icons/Error.svg");

	static bool initialized = false;
	if (!initialized){
		initialized = true;

		errorImage = errorImage.scaled(16,16);
		warningImage = warningImage.scaled(16,16);
		infoImage = infoImage.scaled(16,16);
		logImage = logImage.scaled(16,16);
	}

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoImage;

	case istd::IInformationProvider::IC_WARNING:
		return warningImage;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorImage;

	default:
		return logImage;
	}
}


QString CTextLogGuiComp::GetCategoryText(int category) const
{
	switch (category){
		case istd::IInformationProvider::IC_INFO:
			return "Info";
		case istd::IInformationProvider::IC_WARNING:
			return "Warning";
		case istd::IInformationProvider::IC_ERROR:
			return "Error";
		case istd::IInformationProvider::IC_CRITICAL:
			return "Critical";
	}

	return "";
}


} // namespace iloggui


