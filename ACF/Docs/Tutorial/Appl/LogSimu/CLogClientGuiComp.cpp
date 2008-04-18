#include "CLogClientGuiComp.h"

#include <time.h>


#include "ibase/CMessage.h"


// protected slots

void CLogClientGuiComp::OnTimer()
{
	int category = rand() % 4;

	if(m_logCompPtr.IsValid()){
		m_logCompPtr->AddMessage(new ibase::CMessage(ibase::IMessage::MessageCategory(category), "Test message", "Test source"));
	}
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CLogClientGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));

	if (m_logGuiCompPtr.IsValid()){
		m_logGuiCompPtr->CreateGui(GetWidget());
	}

	int timeIntervall = 1000; // ms

	if (m_intervallAttrPtr.IsValid()){
		timeIntervall = m_intervallAttrPtr->GetValue() * 1000;
	}

	m_timer.start(timeIntervall);

	srand(time(NULL));
}

