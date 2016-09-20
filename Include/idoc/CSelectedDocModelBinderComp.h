#ifndef idoc_CSelectedDocModelBinderComp_included
#define idoc_CSelectedDocModelBinderComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>
#include <iprm/INameParam.h>
#include <ifile/IFileNameParam.h>
#include <idoc/IDocumentManager.h>


namespace idoc
{


/**
	Conditional model-observer binder depending on actual selection.
	This component allows to work-around of problems with observing of selected document or view.
	It connects model with observer only if some document (or view) is currently selected.
	Please note, that there is no assumption that connected model must be the same as observed model.
*/
class CSelectedDocModelBinderComp:
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<IDocumentManager>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CSelectedDocModelBinderComp);
		I_REGISTER_SUBELEMENT(SelectedDocumentInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedDocumentInfo, iprm::INameParam, ExtractSelectedDocumentInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedDocumentInfo, ifile::IFileNameParam, ExtractSelectedDocumentInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedDocumentInfo, imod::IModel, ExtractSelectedDocumentInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectedDocumentInfo, istd::IChangeable, ExtractSelectedDocumentInfo);
		I_ASSIGN(m_observedObjectCompPtr, "ActivationObject", "Object will be observed if it is active", true, "ActivationObject");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager used to check which object is active", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN(m_modelCompPtr, "Model", "Model to be connected", true, "Model");
		I_ASSIGN_MULTI_0(m_observersCompPtr, "Observers", "Observers for the model", true);
	I_END_COMPONENT;

	CSelectedDocModelBinderComp();

protected:
	void TryConnectObservers();
	void TryDisconnectObservers();

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(icomp::IComponent, m_observedObjectCompPtr);
	I_REF(IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(imod::IModel, m_modelCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractSelectedDocumentInfo(CSelectedDocModelBinderComp& component)
	{
		return &component.m_selectedDocumentInfo;
	}

	class DocumentInfo:
				virtual public iprm::INameParam,
				virtual public ifile::IFileNameParam
	{
	public:
		// reimplemented (iprm::INameParam)
		virtual const QString& GetName() const;
		virtual void SetName(const QString& name);
		virtual bool IsNameFixed() const;

		// reimplemented (ifile::IFileNameParam)
		virtual int GetPathType() const;
		virtual const QString& GetPath() const;
		virtual void SetPath(const QString& path);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		QString name;
		QString filePath;
	};

	bool m_isActive;

	typedef QMap<imod::IObserver*, imod::IModel*> ModelObserverMap;
	ModelObserverMap m_connectedMap;

	imod::TModelWrap<DocumentInfo> m_selectedDocumentInfo;
};


} // namespace idoc


#endif // !idoc_CSelectedDocModelBinderComp_included


