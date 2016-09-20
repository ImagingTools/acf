#include <iqtgui/CTableBasedModelEditorComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#else
#include <QtGui/QHeaderView>
#endif


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CTableBasedModelEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Q_ASSERT(m_showHorizontalHeaderAttrPtr.IsValid());

	QTableView* viewPtr = GetQtWidget();
	Q_ASSERT(viewPtr != NULL);

	QHeaderView* horizontalHeaderPtr = viewPtr->horizontalHeader();
	Q_ASSERT(horizontalHeaderPtr != NULL);

#if QT_VERSION >= 0x050000
	horizontalHeaderPtr->setSectionResizeMode(QHeaderView::Stretch);
#else
	horizontalHeaderPtr->setResizeMode(QHeaderView::Stretch);
#endif

	if (!*m_showHorizontalHeaderAttrPtr){
		horizontalHeaderPtr->hide();
	}

	QHeaderView* verticalHeaderPtr = viewPtr->verticalHeader();
	Q_ASSERT(verticalHeaderPtr != NULL);
	if (!*m_showVerticalHeaderAttrPtr){
		verticalHeaderPtr->hide();
	}
}


} // namespace iqtgui


