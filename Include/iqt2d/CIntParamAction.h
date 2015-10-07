#ifndef iqt2d_CIntParamAction_included
#define iqt2d_CIntParamAction_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QSpinBox>
#else
#include <QtGui/QAction>
#include <QtGui/QWidgetAction>
#include <QtGui/QSpinBox>
#endif


namespace iqt2d
{


class CIntParamAction: public QAction
{
public:
	CIntParamAction(const QIcon& icon, const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);
	CIntParamAction(const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);

	int GetValue() const
	{
		Q_ASSERT(m_spinEditor);

		if (m_spinEditor){
			return m_spinEditor->value();
		}

		return 0;
	}

	QSpinBox* GetEditor() const
	{
		Q_ASSERT(m_spinEditor);

		return m_spinEditor;
	}

	QWidgetAction* GetWidgetAction() const
	{
		Q_ASSERT(m_widgetActionPtr);

		return m_widgetActionPtr;
	}

protected:
	void Init(int minValue, int maxValue, int value);

	QWidgetAction* m_widgetActionPtr;
	QSpinBox* m_spinEditor;
};


} // namespace iqt2d


#endif //!iqt2d_CIntParamAction_included


