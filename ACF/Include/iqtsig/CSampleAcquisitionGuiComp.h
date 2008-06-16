#ifndef iqtsig_CSampleAcquisitionGuiComp_included
#define iqtsig_CSampleAcquisitionGuiComp_included


// Qt includes
#include <QTimer>

#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"
#include "imod/TModelWrap.h"

#include "iprm/IParamsSet.h"

#include "isig/ISamplesProcessor.h"
#include "isig/CSamplesContainer.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiCompBase.h"

#include "iqtsig/CSampleContainerView.h"

#include "iqtsig/Generated/ui_CSampleAcquisitionGuiComp.h"


namespace iqtsig
{


class CSampleAcquisitionGuiComp: public iqt::TDesignerGuiCompBase<Ui::CSampleAcquisitionGuiComp, QWidget>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiCompBase<Ui::CSampleAcquisitionGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSampleAcquisitionGuiComp)
		I_ASSIGN(m_sampleAcquisitionCompPtr, "SampleAcquisition", "Sample acquisiotion object used to fill sample container", true, "SampleAcquisition");
		I_ASSIGN(m_sampleLoaderCompPtr, "SampleLoader", "Saves sample to file", false, "SampleLoader");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_minDisplayedValueAttrPtr, "MinDispValue", "Minimal displayed value", true, 0);
		I_ASSIGN(m_maxDisplayedValueAttrPtr, "MaxDispValue", "Maximal displayed value", true, 100);
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
		I_ASSIGN(m_acquisitionTimeoutAttrPtr, "AcquisitionTimeout", "Timeout of single acquisition in seconds", false, 1);
		I_ASSIGN(m_startOnInitAttrPtr, "StartOnInit", "If it true acquisition start on initialization", true, false);
	I_END_COMPONENT

	CSampleAcquisitionGuiComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected slots:
	void on_StartButton_toggled(bool checked);
	void on_SaveSampleButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

protected:
	bool AcquireSample();

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(isig::ISamplesProcessor, m_sampleAcquisitionCompPtr);

	I_REF(iser::IFileLoader, m_sampleLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);

	I_ATTR(double, m_minDisplayedValueAttrPtr);
	I_ATTR(double, m_maxDisplayedValueAttrPtr);
	I_ATTR(double, m_liveIntervalAttrPtr);
	I_ATTR(double, m_acquisitionTimeoutAttrPtr);
	I_ATTR(bool, m_startOnInitAttrPtr);

	imod::TModelWrap<isig::CSamplesContainer> m_samples;
	CSampleContainerView m_samplesView;

	QTimer m_timer;
};


} // namespace iqtsig


#endif // !iqtsig_CSampleAcquisitionGuiComp_included


