#ifndef CLogClientGuiComp_included
#define CLogClientGuiComp_included


// Qt includes
#include <QTimer>

// ACF includes
#include "ibase/IMessageContainer.h"

#include "iqt/TGuiComponentBase.h"


class CLogClientGuiComp: public iqt::TGuiComponentBase<QWidget>
{
	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLogClientGuiComp)
		I_ASSIGN(m_intervallAttrPtr, "AddIntervall", "Time intervall for adding a message", true, 0.5)
		I_ASSIGN(m_logCompPtr, "Log", "Log", true, "Log")
		I_ASSIGN(m_logGuiCompPtr, "Log", "Log GUI", true, "LogGui")
	I_END_COMPONENT

	enum MessageId
	{
		MI_TEST_MESSAGE = 0x32320a0
	};

protected slots:
	 void OnTimer();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_REF(ibase::IMessageContainer, m_logCompPtr)
	I_REF(iqt::IGuiObject, m_logGuiCompPtr)
	I_ATTR(double, m_intervallAttrPtr)

private:
	QTimer m_timer;
};


#endif // !CLogClientGuiComp_included