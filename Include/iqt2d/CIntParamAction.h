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

// ACF includes
#include "iqt2d/CActionAdapter.h"


namespace iqt2d
{


class CIntParamAction: public CActionAdapter::Action
{
public:
	typedef CActionAdapter::Action BaseClass;

	CIntParamAction(const QIcon& icon, const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);
	CIntParamAction(const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);

	int GetValue() const;
	void SetValue(int x);

	QSpinBox* GetEditor() const;
	QWidgetAction* GetWidgetAction() const;

protected:
	// reimplemented (CActionAdapter::Action)
	virtual void OnActionAdded(CActionAdapter& adapter);

protected:
	void Init(int minValue, int maxValue, int value);

	QWidgetAction* m_widgetActionPtr;
	QSpinBox* m_spinEditor;
};


inline int CIntParamAction::GetValue() const
{
	Q_ASSERT(m_spinEditor);

	if (m_spinEditor){
		return m_spinEditor->value();
	}

	return 0;
}


inline void CIntParamAction::SetValue(int x)
{
	Q_ASSERT(m_spinEditor);

	if (m_spinEditor){
		m_spinEditor->setValue(x);
	}
}


inline QSpinBox* CIntParamAction::GetEditor() const
{
	Q_ASSERT(m_spinEditor);

	return m_spinEditor;
}


inline QWidgetAction* CIntParamAction::GetWidgetAction() const
{
	Q_ASSERT(m_widgetActionPtr);

	return m_widgetActionPtr;
}


} // namespace iqt2d


#endif //!iqt2d_CIntParamAction_included


