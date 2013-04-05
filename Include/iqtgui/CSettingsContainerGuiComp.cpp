#include "iqtgui/CSettingsContainerGuiComp.h"


// Qt includes
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


namespace iqtgui
{


// protected methods

// reimplemented (CContainerGuiBase)

bool CSettingsContainerGuiComp::CreateMainGui(QSize iconSize)
{
	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr == NULL){
		return false;
	}

	QHBoxLayout* mainLayoutPtr = new QHBoxLayout(widgetPtr);
	mainLayoutPtr->setContentsMargins(0, 0, 0, 0);

	m_menuListPtr = new QListWidget(widgetPtr);
	m_stackedWidgetPtr = new QStackedWidget(widgetPtr);

	if (*m_menuPositionAttrPtr == 0){
		mainLayoutPtr->addWidget(m_menuListPtr);
		mainLayoutPtr->addWidget(m_stackedWidgetPtr);
	}
	else{
		mainLayoutPtr->addWidget(m_stackedWidgetPtr);
		mainLayoutPtr->addWidget(m_menuListPtr);
	}

	m_menuListPtr->setIconSize(iconSize);
	m_menuListPtr->setUniformItemSizes(true);
	m_menuListPtr->setAlternatingRowColors(*m_alternateColorsAttrPtr);
	m_menuListPtr->setFrameStyle(QFrame::NoFrame);

	if (m_fixedWidthAttrPtr.IsValid()){
		int width = qMax(iconSize.width(), *m_fixedWidthAttrPtr);
		m_menuListPtr->setFixedWidth(width);
	}

	connect(m_menuListPtr, SIGNAL(currentRowChanged(int)), m_stackedWidgetPtr, SLOT(setCurrentIndex(int)));

	return true;
}


int CSettingsContainerGuiComp::CreatePageGui(const QString& name, QSize /*iconSize*/, iqtgui::IGuiObject* guiPtr)
{
	if (guiPtr == NULL || m_stackedWidgetPtr == NULL || m_menuListPtr == NULL){
		return -1;
	}

	if (!guiPtr->CreateGui(m_stackedWidgetPtr)){
		return -1;
	}

	m_menuListPtr->addItem(new QListWidgetItem(name));

	return m_stackedWidgetPtr->addWidget(guiPtr->GetWidget());
}


void CSettingsContainerGuiComp::UpdateItem(int index, const QIcon& icon, const QString& toolTip)
{
	Q_ASSERT(m_menuListPtr != NULL);

	QListWidgetItem* itemPtr = m_menuListPtr->item(index);
	if (itemPtr != NULL){
		itemPtr->setIcon(icon);
		itemPtr->setToolTip(toolTip);
	}
}


bool CSettingsContainerGuiComp::SetSelectedIndex(int index)
{
	Q_ASSERT(m_menuListPtr != NULL);
	Q_ASSERT(m_stackedWidgetPtr != NULL);
		
	if (m_menuListPtr != NULL){
		m_menuListPtr->setCurrentRow(index);

		return true;
	}

	return false;
}


bool CSettingsContainerGuiComp::SetEnabled(int /*index*/, bool /*isEnabled*/)
{
	// not implemented
	return true;
}


} // namespace iqtgui
