#include "iqtproc/CProcessorControlGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "istd/TDelPtr.h"


namespace iqtproc
{


CProcessorControlGuiComp::CProcessorControlGuiComp()
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (icomp::IComponent)

void CProcessorControlGuiComp::OnComponentCreated()
{
	I_ASSERT(m_liveIntervalAttrPtr.IsValid());

	m_timer.setInterval(int(*m_liveIntervalAttrPtr * 1000));
}


// protected slots

void CProcessorControlGuiComp::on_DataAcquisitionButton_clicked()
{
	LiveDataAcquisitionButton->setChecked(false);

	DoDataAcquisition();
}


void CProcessorControlGuiComp::on_LiveDataAcquisitionButton_toggled(bool checked)
{
	if (checked){
		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CProcessorControlGuiComp::on_SaveDataButton_clicked()
{
	if (m_outputDataLoaderCompPtr.IsValid() && m_outputDataCompPtr.IsValid()){
		if (m_outputDataLoaderCompPtr->SaveToFile(*m_outputDataCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
	}
}


void CProcessorControlGuiComp::on_LoadParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->LoadFromFile(*m_paramsSetCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot load parameters"));
		}
	}
}


void CProcessorControlGuiComp::on_SaveParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->SaveToFile(*m_paramsSetCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save parameters"));
		}
	}
}


void CProcessorControlGuiComp::OnTimerReady()
{
	DoDataAcquisition();
}


// protected methods

bool CProcessorControlGuiComp::DoDataAcquisition()
{
	if (m_processorCompPtr.IsValid() && m_outputDataCompPtr.IsValid()){
		int taskId = m_processorCompPtr->BeginTask(
					m_paramsSetCompPtr.GetPtr(),
					m_inputDataCompPtr.GetPtr(),
					m_outputDataCompPtr.GetPtr());
		if (taskId >= 0){
			return m_processorCompPtr->WaitTaskFinished(-1, 1) != iproc::IProcessor::TS_INVALID;
		}
	}

	return false;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProcessorControlGuiComp::OnGuiCreated()
{
	bool hasProcessingData = m_outputDataCompPtr.IsValid();
	bool hasProcessor = m_processorCompPtr.IsValid();

	DataAcquisitionButton->setVisible(hasProcessingData && hasProcessor);
	LiveDataAcquisitionButton->setVisible(hasProcessingData && hasProcessor);
	SaveDataButton->setVisible(hasProcessingData && m_outputDataLoaderCompPtr.IsValid());

	bool areParamsEditable = false;
	if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	ParamsGB->setVisible(
				m_paramsSetCompPtr.IsValid() &&
				(areParamsEditable || m_paramsLoaderCompPtr.IsValid()));
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	ParamsFrame->setVisible(m_paramsSetCompPtr.IsValid() && areParamsEditable);
}


void CProcessorControlGuiComp::OnGuiDestroyed()
{
	m_timer.stop();

	if (		m_paramsSetModelCompPtr.IsValid() &&
				m_paramsSetObserverCompPtr.IsValid() &&
				m_paramsSetModelCompPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		m_paramsSetModelCompPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}
}


} // namespace iqtproc


