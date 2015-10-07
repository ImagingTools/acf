#include "iqt2d/CIntParamAction.h"


namespace iqt2d
{


CIntParamAction::CIntParamAction(const QIcon& icon, const QString& text, int minValue, int maxValue, int value, QObject* parentPtr)
:	QAction(icon, text, parentPtr)
{
	Init(minValue, maxValue, value);
}


CIntParamAction::CIntParamAction(const QString& text, int minValue, int maxValue, int value, QObject* parentPtr)
:	QAction(text, parentPtr)
{
	Init(minValue, maxValue, value);
}


void CIntParamAction::Init(int minValue, int maxValue, int value)
{
	m_spinEditor = new QSpinBox(NULL);
	m_spinEditor->setRange(minValue, maxValue);
	m_spinEditor->setValue(value);

	m_widgetActionPtr = new QWidgetAction(this);
	m_widgetActionPtr->setDefaultWidget(m_spinEditor);
}


} // namespace iqt2d


