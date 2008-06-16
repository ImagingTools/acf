#include "iqtsig/CSampleAcquisitionGuiComp.h"


// Qt includes
#include <QMessageBox>


namespace iqtsig
{


CSampleAcquisitionGuiComp::CSampleAcquisitionGuiComp()
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (icomp::IComponent)

void CSampleAcquisitionGuiComp::OnComponentCreated()
{
	m_timer.setInterval(int(*m_liveIntervalAttrPtr * 1000));
}


// protected slots

void CSampleAcquisitionGuiComp::on_StartButton_toggled(bool checked)
{
	if (checked){
		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CSampleAcquisitionGuiComp::on_SaveSampleButton_clicked()
{
	if (m_sampleLoaderCompPtr.IsValid() && (m_samples.GetSamplesCount() > 0)){
		if (m_sampleLoaderCompPtr->SaveToFile(m_samples, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save sample"));
		}
	}
}


void CSampleAcquisitionGuiComp::on_LoadParamsButton_clicked()
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


void CSampleAcquisitionGuiComp::on_SaveParamsButton_clicked()
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


void CSampleAcquisitionGuiComp::OnTimerReady()
{
	AcquireSample();
}


// protected methods

bool CSampleAcquisitionGuiComp::AcquireSample()
{
	if (m_sampleAcquisitionCompPtr.IsValid()){
		double timeout = -1;
		if (m_acquisitionTimeoutAttrPtr.IsValid()){
			timeout = *m_acquisitionTimeoutAttrPtr;
		}

		int taskId = m_sampleAcquisitionCompPtr->BeginTask(m_paramsSetCompPtr.GetPtr(), NULL, &m_samples);
		if (taskId >= 0){
			return m_sampleAcquisitionCompPtr->WaitTaskFinished(taskId, timeout) != isig::ISamplesProcessor::TS_INVALID;
		}
	}

	return false;
}


// reimplemented (iqt::CGuiComponentBase)

void CSampleAcquisitionGuiComp::OnGuiCreated()
{
	bool canAcquire = m_sampleAcquisitionCompPtr.IsValid();

	StartButton->setVisible(canAcquire);
	StartButton->setChecked(*m_startOnInitAttrPtr);
	StopButton->setVisible(canAcquire);
	StopButton->setChecked(!*m_startOnInitAttrPtr);

	SaveSampleButton->setVisible(canAcquire && m_sampleLoaderCompPtr.IsValid());

	bool areParamsEditable = false;
	if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	double minDisplayedValue = istd::Min(*m_minDisplayedValueAttrPtr, *m_maxDisplayedValueAttrPtr);
	double maxDisplayedValue = istd::Max(*m_minDisplayedValueAttrPtr, *m_maxDisplayedValueAttrPtr);
	m_samplesView.SetDisplayedRange(istd::CRange(minDisplayedValue, maxDisplayedValue));

	m_samples.AttachObserver(&m_samplesView);

	QLayout* viewLayoutPtr = SampleViewFrame->layout();
	if (viewLayoutPtr != NULL){
		viewLayoutPtr->addWidget(&m_samplesView);
	}

	ParamsGB->setVisible(
				m_paramsSetCompPtr.IsValid() &&
				(areParamsEditable || m_paramsLoaderCompPtr.IsValid()));
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	ParamsFrame->setVisible(m_paramsSetCompPtr.IsValid() && areParamsEditable);
	SampleViewFrame->setVisible(canAcquire);
}


void CSampleAcquisitionGuiComp::OnGuiDestroyed()
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

	QLayout* viewLayoutPtr = SampleViewFrame->layout();
	if (viewLayoutPtr != NULL){
		viewLayoutPtr->removeWidget(&m_samplesView);
	}

	m_samples.DetachObserver(&m_samplesView);
}


} // namespace iqtsig


