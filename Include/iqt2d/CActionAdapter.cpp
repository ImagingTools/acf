#include <iqt2d/CActionAdapter.h>


namespace iqt2d
{


// public methods

CActionAdapter::CActionAdapter(QWidget& host)
	:m_hostPtr(&host)
{
}


QAction* CActionAdapter::AddAction(QAction& action)
{
	m_hostPtr->addAction(&action);

	Action* extActionPtr = dynamic_cast<Action*>(&action);
	if (extActionPtr){
		extActionPtr->OnActionAdded(*this);
	}

	return &action;
}

QAction* CActionAdapter::AddSeparator()
{
	QAction* separatorPtr = new QAction(m_hostPtr);
	separatorPtr->setSeparator(true);

	m_hostPtr->addAction(separatorPtr);
	return separatorPtr;
}


QWidget* CActionAdapter::GetWidget() const
{
	return m_hostPtr;
}


// public methods of the embedded class Action

CActionAdapter::Action::Action(const QString& text, QObject* parent): QAction(text, parent)
{
}


CActionAdapter::Action::Action(const QIcon& icon, const QString& text, QObject* parent): QAction(icon, text, parent)
{
}


void CActionAdapter::Action::OnActionAdded(CActionAdapter& /*adapter*/)
{
}


} // namespace iqt2d



