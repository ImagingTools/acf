// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QLabel>
#include <QtGui/QApplication>
#endif

// ACF includes
#include "iwidgets/CMultiPageWidget.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);

	QApplication app(argc, argv);

	iwidgets::CMultiPageWidget emptyContainer;
	emptyContainer.setWindowTitle("Empty container");
	emptyContainer.showMaximized();

	iwidgets::CMultiPageWidget tabContainer(NULL, iwidgets::CMultiPageWidget::DT_TAB_WIDGET);
	tabContainer.setWindowTitle("Tab container");

	QLabel page1("Page 1");
	tabContainer.InsertPage(&page1, "Page 1");
	tabContainer.SetPageIcon(0, QIcon(":/Icons/StateOk"));

	QLabel page2("Page 2");
	tabContainer.InsertPage(&page2, "Page 2");
	tabContainer.SetPageIcon(1, QIcon(":/Icons/StateWarning"));

	QLabel page3("Page 3");
	tabContainer.InsertPage(&page3, "Page 3");
	tabContainer.SetPageIcon(2, QIcon(":/Icons/StateInvalid"));

	tabContainer.showMaximized();

	return app.exec();
}

