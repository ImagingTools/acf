#ifndef CLogClientGuiComp_included
#define CLogClientGuiComp_included


// Qt includes
#include <QThread>

// ACF includes
#include "ibase/IMessageContainer.h"

#include "iqtgui/TGuiComponentBase.h"


class CLogClientGuiComp: public iqtgui::TGuiComponentBase<QWidget>, public QThread
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLogClientGuiComp);
		I_ASSIGN(m_intervallAttrPtr, "AddIntervall", "Time intervall for adding a message", true, 0.5);
		I_ASSIGN(m_logCompPtr, "Log", "Log", true, "Log");
		I_ASSIGN(m_logGuiCompPtr, "Log", "Log GUI", true, "LogGui");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_TEST_MESSAGE = 0x32320a0
	};

protected:
	 virtual void run();

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_REF(ibase::IMessageContainer, m_logCompPtr)
	I_REF(iqtgui::IGuiObject, m_logGuiCompPtr)
	I_ATTR(double, m_intervallAttrPtr)
};


#endif // !CLogClientGuiComp_included