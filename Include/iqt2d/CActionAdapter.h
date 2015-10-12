#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif


namespace iqt2d
{


class CActionAdapter
{
public:
	CActionAdapter(QWidget& host);

	virtual QAction* AddAction(QAction& action);
	virtual QAction* AddSeparator();
	virtual QWidget* GetWidget() const;

	class Action: public QAction
	{
	public:
		Action(const QString& text, QObject* parent);
		Action(const QIcon& icon, const QString& text, QObject* parent);

		virtual void OnActionAdded(CActionAdapter& /*adapter*/);
	};

protected:
	QWidget* m_hostPtr;
};


} // namespace iqt2d


