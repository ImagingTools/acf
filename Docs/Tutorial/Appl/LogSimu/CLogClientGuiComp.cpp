#include "CLogClientGuiComp.h"


// STD incldues
#include <time.h>

// Qt includes
#include <QVBoxLayout>

// ACF includes
#include "ibase/CMessage.h"


// protected

void CLogClientGuiComp::run()
{
	int timeIntervall = 10; // ms

	if (m_intervallAttrPtr.IsValid()){
		timeIntervall = m_intervallAttrPtr->GetValue() * 1000;
	}

	while(true){
		int category = rand() % 4;

		if(m_logCompPtr.IsValid()){
			m_logCompPtr->AddMessage(new ibase::CMessage(ibase::IMessage::MessageCategory(category), MI_TEST_MESSAGE, "Test message", "Test source"));
		}

		QThread::msleep(timeIntervall);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CLogClientGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_logGuiCompPtr.IsValid()){
		QVBoxLayout* logLayout = NULL;
		logLayout = new QVBoxLayout(GetWidget());
	
		m_logGuiCompPtr->CreateGui(GetWidget());
	}

	int timeIntervall = 10; // ms

	if (m_intervallAttrPtr.IsValid()){
		timeIntervall = m_intervallAttrPtr->GetValue() * 1000;
	}

	srand(time(NULL));

	QThread::start(QThread::TimeCriticalPriority);
}

