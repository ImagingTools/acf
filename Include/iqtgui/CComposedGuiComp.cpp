#include "iqtgui/CComposedGuiComp.h"


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#endif


namespace iqtgui
{


// public methods

// reimplemented (TRestorableGuiWrap)

void CComposedGuiComp::OnRestoreSettings(const QSettings& settings)
{
	Q_ASSERT(IsGuiCreated());

	CMultiPageWidget* widgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());

	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(widgetPtr->GetContainerWidgetPtr());
	if (splitterPtr != NULL){
		// preserve overriding of splitter orientation:
		Qt::Orientation splitterOrientation = splitterPtr->orientation();

		QByteArray splitterState = settings.value(GetSettingsKey()).toByteArray();

		splitterPtr->restoreState(splitterState);

		splitterPtr->setOrientation(splitterOrientation);
	}
}


void CComposedGuiComp::OnSaveSettings(QSettings& settings) const
{
	Q_ASSERT(IsGuiCreated());

	CMultiPageWidget* widgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());

	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(widgetPtr->GetContainerWidgetPtr());
	if (splitterPtr != NULL){
		QByteArray splitterState = splitterPtr->saveState();

		settings.setValue(GetSettingsKey(), splitterState);
	}
}


// protected methods

// reimplemented (iqtgui::CMultiPageGuiCompBase)

int CComposedGuiComp::GetPagesCount() const
{
	return m_guisCompPtr.GetCount();
}


iqtgui::IGuiObject* CComposedGuiComp::GetPageGuiComponent(int pageIndex) const
{
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(pageIndex < m_guisCompPtr.GetCount());

	return m_guisCompPtr[pageIndex];
}


int CComposedGuiComp::GetDesignType() const
{
	return *m_designTypeAttrPtr;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CComposedGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CMultiPageWidget* widgetPtr = dynamic_cast<CMultiPageWidget*>(GetWidget());
	Q_ASSERT(widgetPtr != NULL);

	bool isFlat = *m_flatViewAttrPtr;

	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(widgetPtr->GetContainerWidgetPtr());
	if (tabWidgetPtr != NULL){
		if (isFlat){
			tabWidgetPtr->setTabShape(QTabWidget::Triangular);
		}

		tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));
	}

	// TODO: possible remove it!
	QToolBox* toolBoxPtr = dynamic_cast<QToolBox*>(widgetPtr->GetContainerWidgetPtr());
	if (toolBoxPtr != NULL){
		widgetPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	}

	QSplitter* splitterPtr = dynamic_cast<QSplitter*>(widgetPtr->GetContainerWidgetPtr());
	if (splitterPtr != NULL){
		if (isFlat){
			splitterPtr->setOpaqueResize(false);
		}
	}
}


// private methods

QString CComposedGuiComp::GetSettingsKey() const
{
	QString settingsKey = "Splitter";
	if (m_settingsKeyAttrPtr.IsValid()){
		settingsKey = *m_settingsKeyAttrPtr + QString("/") + settingsKey;
	}

	return settingsKey;
}


} // namespace iqtgui


