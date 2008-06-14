#include "iqt/CSnapImageGuiComp.h"


// Qt includes
#include <QMessageBox>


namespace iqt
{


CSnapImageGuiComp::CSnapImageGuiComp()
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (icomp::IComponent)

void CSnapImageGuiComp::OnComponentCreated()
{
	m_timer.setInterval(int(*m_liveIntervalAttrPtr * 1000));
}


// protected slots

void CSnapImageGuiComp::on_SnapImageButton_clicked()
{
	LiveImageButton->setChecked(false);

	SnapImage();
}


void CSnapImageGuiComp::on_LiveImageButton_toggled(bool checked)
{
	if (checked){
		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CSnapImageGuiComp::on_SaveImageButton_clicked()
{
	if (m_bitmapLoaderCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		if (m_bitmapLoaderCompPtr->SaveToFile(*m_bitmapCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
	}
}


void CSnapImageGuiComp::on_LoadParamsButton_clicked()
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


void CSnapImageGuiComp::on_SaveParamsButton_clicked()
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


void CSnapImageGuiComp::OnTimerReady()
{
	SnapImage();
}


// protected methods

bool CSnapImageGuiComp::SnapImage()
{
	if (m_bitmapAcquisitionCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		int taskId = m_bitmapAcquisitionCompPtr->BeginTask(m_paramsSetCompPtr.GetPtr(), NULL, m_bitmapCompPtr.GetPtr());
		if (taskId >= 0){
			return m_bitmapAcquisitionCompPtr->WaitTaskFinished(-1, 1) != icam::IBitmapAcquisition::TS_INVALID;
		}
	}

	return false;
}


// reimplemented (iqt::CGuiComponentBase)

void CSnapImageGuiComp::OnGuiCreated()
{
	bool hasBitmap = m_bitmapCompPtr.IsValid();
	bool hasSnap = m_bitmapAcquisitionCompPtr.IsValid();

	SnapImageButton->setVisible(hasBitmap && hasSnap);
	LiveImageButton->setVisible(hasBitmap && hasSnap);
	SaveImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid());
	SaveImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid());

	bool areParamsEditable = false;
	if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	if (m_bitmapModelCompPtr.IsValid() && m_bitmapGuiCompPtr.IsValid() && m_bitmapObserverCompPtr.IsValid()){
		m_bitmapModelCompPtr->AttachObserver(m_bitmapObserverCompPtr.GetPtr());
		m_bitmapGuiCompPtr->CreateGui(ImageViewFrame);
	}

	ParamsGB->setVisible(
				m_paramsSetCompPtr.IsValid() &&
				(areParamsEditable || m_paramsLoaderCompPtr.IsValid()));
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	ParamsFrame->setVisible(m_paramsSetCompPtr.IsValid() && areParamsEditable);
	ImageViewFrame->setVisible(hasBitmap && hasSnap && m_bitmapGuiCompPtr.IsValid() && m_bitmapObserverCompPtr.IsValid());
}


void CSnapImageGuiComp::OnGuiDestroyed()
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

	if (		m_bitmapModelCompPtr.IsValid() &&
				m_bitmapObserverCompPtr.IsValid() &&
				m_bitmapModelCompPtr->IsAttached(m_bitmapObserverCompPtr.GetPtr())){
		m_bitmapModelCompPtr->DetachObserver(m_bitmapObserverCompPtr.GetPtr());
	}

	if (m_bitmapGuiCompPtr.IsValid() && m_bitmapGuiCompPtr->IsGuiCreated()){
		m_bitmapGuiCompPtr->DestroyGui();
	}
}


} // namespace iqt


