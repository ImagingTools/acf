#ifndef iqtproc_TSupplierGuiCompBase_included
#define iqtproc_TSupplierGuiCompBase_included


#include "iser/IFileLoader.h"

#include "imod/IObserver.h"

#include "iprm/IParamsSet.h"

#include "iproc/IIdManager.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqtproc/iqtproc.h"


namespace iqtproc
{


template <class UI, class Model, class WidgetType = QWidget>
class TSupplierGuiCompBase: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<UI, Model> >
{
public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<UI, Model> > BaseClass;

	I_BEGIN_BASE_COMPONENT(TSupplierGuiCompBase);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetExtenderCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_idManagerCompPtr, "IdManager", "ID manager used to generate next processing ID", false, "IdManager");
	I_END_COMPONENT;

	TSupplierGuiCompBase();

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

protected:
	/**
		Check, if parameters are correct connected to GUI and can be editable.
	*/
	bool AreParamsEditable() const;

	/**
		Check if parameter loading is supported.
	*/
	bool IsLoadParamsSupported() const;

	/**
		Check if parameter storing is supported.
	*/
	bool IsSaveParamsSupported() const;

	/**
		Load parameters from external source (file).
	*/
	bool LoadParams();

	/**
		Save parameters for example to file.
	*/
	bool SaveParams();

	/**
		Do inspection test.
	*/
	bool DoTest();

	/**
		Return connected ID manager.
	*/
	iproc::IIdManager* GetIdManager() const;

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// abstract methods
	/**
		Get parameters widget object.
		Extern paremeters GUI will attach to this widget.
		This widget should have some layout.
	*/
	virtual QWidget* GetParamsWidget() const = 0;

private:
	I_REF(iser::IFileLoader, m_bitmapLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iqt::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);
	I_REF(iqt2d::ISceneExtender, m_paramsSetExtenderCompPtr);
	I_REF(iproc::IIdManager, m_idManagerCompPtr);

	bool m_areParamsEditable;
};


// public methods

template <class UI, class Model, class WidgetType>
TSupplierGuiCompBase<UI, Model, WidgetType>::TSupplierGuiCompBase()
:	m_areParamsEditable(false)
{
}


// reimplemented (iqt2d::ISceneExtender)

template <class UI, class Model, class WidgetType>
void TSupplierGuiCompBase<UI, Model, WidgetType>::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


template <class UI, class Model, class WidgetType>
void TSupplierGuiCompBase<UI, Model, WidgetType>::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// protected methods

template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::AreParamsEditable() const
{
	return m_areParamsEditable;
}


template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::IsLoadParamsSupported() const
{
	const Model* supplierPtr = GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(paramsPtr, NULL, true, false)){
				return true;
			}
		}
	}

	return false;
}


template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::IsSaveParamsSupported() const
{
	const Model* supplierPtr = GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->IsOperationSupported(paramsPtr, NULL, false, true)){
				return true;
			}
		}
	}

	return false;
}


template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::LoadParams()
{
	Model* supplierPtr = GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->LoadFromFile(*paramsPtr, "") != iser::IFileLoader::StateFailed){
				return true;
			}
			else{
				QMessageBox::information(
							NULL,
							QObject::tr("Error"),
							QObject::tr("Cannot load parameters"));
			}
		}
	}

	return false;
}


template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::SaveParams()
{
	const Model* supplierPtr = GetObjectPtr();
	if ((supplierPtr != NULL) && m_paramsLoaderCompPtr.IsValid()){
		const iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
		if (paramsPtr != NULL){
			if (m_paramsLoaderCompPtr->SaveToFile(*paramsPtr, "") != iser::IFileLoader::StateFailed){
				return true;
			}
			else{
				QMessageBox::information(
							NULL,
							QObject::tr("Error"),
							QObject::tr("Cannot save parameters"));
			}
		}
	}

	return false;
}


template <class UI, class Model, class WidgetType>
bool TSupplierGuiCompBase<UI, Model, WidgetType>::DoTest()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				m_idManagerCompPtr.IsValid() &&
				m_idManagerCompPtr->GetCurrentId(objectId)){
		supplierPtr->BeginNextObject(objectId);
		supplierPtr->EnsureWorkFinished(objectId);

		return supplierPtr->GetWorkStatus(objectId) < iproc::ISupplier::WS_ERROR;
	}

	return false;
}


template <class UI, class Model, class WidgetType>
iproc::IIdManager* TSupplierGuiCompBase<UI, Model, WidgetType>::GetIdManager() const
{
	return m_idManagerCompPtr.GetPtr();
}


// reimplemented (iqt::TGuiObserverWrap)

template <class UI, class Model, class WidgetType>
void TSupplierGuiCompBase<UI, Model, WidgetType>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	Model* supplierPtr = GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetModelParametersSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	m_areParamsEditable = false;
	QWidget* paramsWidget = GetParamsWidget();
	if (paramsWidget != NULL){
		if ((paramsModelPtr != NULL) && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
			paramsModelPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
			m_paramsSetGuiCompPtr->CreateGui(paramsWidget);

			m_areParamsEditable = true;
		}

		paramsWidget->setVisible(m_areParamsEditable);
	}
}


template <class UI, class Model, class WidgetType>
void TSupplierGuiCompBase<UI, Model, WidgetType>::OnGuiModelDetached()
{
	Model* supplierPtr = GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetModelParametersSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	if (		m_paramsSetObserverCompPtr.IsValid() &&
				(paramsModelPtr != NULL) &&
				paramsModelPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		paramsModelPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}

	m_areParamsEditable = false;

	BaseClass::OnGuiModelDetached();
}


} // namespace iqtproc


#endif // !iqtproc_TSupplierGuiCompBase_included


