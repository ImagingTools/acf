#include <iqt2d/CIntParamAction.h>

// Qt includes
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif


namespace iqt2d
{


CIntParamAction::CIntParamAction(const QIcon& icon, const QString& text, int minValue, int maxValue, int value, QObject* parentPtr)
:	BaseClass(icon, text, parentPtr)
{
	Init(minValue, maxValue, value);
}


CIntParamAction::CIntParamAction(const QString& text, int minValue, int maxValue, int value, QObject* parentPtr)
:	BaseClass(text, parentPtr)
{
	Init(minValue, maxValue, value);
}


// protected methods

void CIntParamAction::Init(int minValue, int maxValue, int value)
{
	m_spinEditor = new QSpinBox(NULL);
	m_spinEditor->setRange(minValue, maxValue);
	m_spinEditor->setValue(value);

	m_widgetActionPtr = new QWidgetAction(this);
	m_widgetActionPtr->setDefaultWidget(m_spinEditor);
}


// reimplemented (CActionAdapter::Action)

void CIntParamAction::OnActionAdded(CActionAdapter& adapter)
{
	// add widget actions to menus only
	if (dynamic_cast<QMenu*>(adapter.GetWidget())){
		adapter.AddAction(*m_widgetActionPtr);
	}
}


} // namespace iqt2d


