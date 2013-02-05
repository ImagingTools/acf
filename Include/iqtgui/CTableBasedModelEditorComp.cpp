#include "iqtgui/CTableBasedModelEditorComp.h"


// Qt includes
#include <QtGui/QHeaderView>


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

	horizontalHeaderPtr->setResizeMode(QHeaderView::Stretch);

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


