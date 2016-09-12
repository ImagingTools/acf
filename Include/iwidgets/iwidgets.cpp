#include <iwidgets/iwidgets.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QtWidgets/QWidget>
	#include <QtWidgets/QLayoutItem>
	#include <QtWidgets/QLayout>
#else
	#include <QtGui/QWidget>
	#include <QtGui/QLayoutItem>
	#include <QtGui/QLayout>
#endif


namespace iwidgets
{


void ClearLayout(QLayout* layout)
{
	while (QLayoutItem* item = layout->takeAt(0))
	{
		QWidget* widgetPtr = item->widget();
		if (widgetPtr != NULL) {
			delete widgetPtr;
		}
		
		QLayout* childLayoutPtr = item->layout();
		if (childLayoutPtr != NULL){
			ClearLayout(childLayoutPtr);
		}

		delete item;
	}
}


} // namespace iwidgets


