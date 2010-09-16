#ifndef iqtproc_CAcquisitonProcessorGuiComp_included
#define iqtproc_CAcquisitonProcessorGuiComp_included


// Qt includes
#include <QTimer>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iprm/IParamsSet.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "isys/ITimer.h"

#include "iproc/IProcessor.h"

#include "iqtproc/Generated/ui_CAcquisitonProcessorGuiComp.h"


namespace iqtproc
{


class CProcessorControlGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CProcessorControlGuiComp, QWidget>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CProcessorControlGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CProcessorControlGuiComp)
		I_ASSIGN(m_inputDataCompPtr, "InputData", "Input data that will be processed", false, "InputData");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Output data object", true, "OutputData");
		I_ASSIGN(m_outputDataLoaderCompPtr, "OutputDataLoader", "Saves processing data to file", false, "OutputDataLoader");
		I_ASSIGN(m_processorCompPtr, "Processor", "Processor object used to convert input to output data", true, "Processor");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
	I_END_COMPONENT

	CProcessorControlGuiComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected slots:
	void on_DataAcquisitionButton_clicked();
	void on_LiveDataAcquisitionButton_toggled(bool checked);
	void on_SaveDataButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

protected:
	bool DoDataAcquisition();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(istd::IChangeable, m_inputDataCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(iser::IFileLoader, m_outputDataLoaderCompPtr);

	I_REF(iproc::IProcessor, m_processorCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);

	I_ATTR(double, m_liveIntervalAttrPtr);

	QTimer m_timer;
};


} // namespace iqtproc


#endif // !iqtproc_CAcquisitonProcessorGuiComp_included


