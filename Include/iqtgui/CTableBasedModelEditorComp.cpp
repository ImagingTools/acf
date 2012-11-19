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

	I_ASSERT(m_showHorizontalHeaderAttrPtr.IsValid());

	QTableView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);

	QHeaderView* horizontalHeaderPtr = viewPtr->horizontalHeader();
	I_ASSERT(horizontalHeaderPtr != NULL);

	horizontalHeaderPtr->setResizeMode(QHeaderView::Stretch);

	if (!*m_showHorizontalHeaderAttrPtr){
		horizontalHeaderPtr->hide();
	}

	QHeaderView* verticalHeaderPtr = viewPtr->verticalHeader();
	I_ASSERT(verticalHeaderPtr != NULL);
	if (!*m_showVerticalHeaderAttrPtr){
		verticalHeaderPtr->hide();
	}
}


} // namespace iqtgui


