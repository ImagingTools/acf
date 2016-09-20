#include <iqtgui/CModelDialogGuiComp.h>


// Qt includes
#include <QtGui/QIcon>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

int CModelDialogGuiComp::ExecuteDialog(IGuiObject* parentPtr)
{
	if (!m_workingDataFactoryCompPtr.IsValid() || !m_workingModelFactoryCompPtr.IsValid()){
		return QDialog::Rejected;
	}

	istd::IChangeable* sourceDataPtr = m_dataCompPtr.GetPtr();
	istd::IChangeable* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		sourceDataPtr = objectPtr;
	}

	if (sourceDataPtr == NULL){
		return QDialog::Rejected;
	}

	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, parentPtr));
	if (!dialogPtr.IsValid()){
		return QDialog::Rejected;
	}

	m_workingObjectPtr.SetPtr(m_workingDataFactoryCompPtr.CreateComponent());

	int retVal = QDialog::Rejected;

	istd::IChangeable* workingDataPtr = m_workingDataFactoryCompPtr.ExtractInterface(m_workingObjectPtr.GetPtr());
	if (workingDataPtr != NULL){
		if (workingDataPtr->CopyFrom(*sourceDataPtr)){
			imod::IModel* workingModelPtr = m_workingModelFactoryCompPtr.ExtractInterface(m_workingObjectPtr.GetPtr());
			Q_ASSERT(workingModelPtr != NULL);
			if (workingModelPtr != NULL){
				bool isAttached = workingModelPtr->AttachObserver(m_editorCompPtr.GetPtr());
				if (isAttached){
					retVal = dialogPtr->exec();

					if (retVal == QDialog::Accepted){
						sourceDataPtr->CopyFrom(*workingDataPtr);
					}

					// re-attach the original data model to the editor:
					workingModelPtr->DetachObserver(m_editorCompPtr.GetPtr());
				}
			}
		}
	}

	m_workingObjectPtr.Reset();

	return retVal;
}


} // namespace iqtgui


