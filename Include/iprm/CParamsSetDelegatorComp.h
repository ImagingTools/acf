#ifndef iprm_CDelegatedParamsSetComp_included
#define iprm_CDelegatedParamsSetComp_included


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <iprm/IParamsSet.h>
#include <icomp/CComponentBase.h>


namespace iprm
{


/** 
	Delegator of the parameter set interface.
*/
class CParamsSetDelegatorComp: 
			public icomp::CComponentBase,
			protected imod::CModelUpdateBridge,
			virtual public iprm::IParamsSet
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CModelUpdateBridge BaseClass2;

	I_BEGIN_COMPONENT(CParamsSetDelegatorComp);
		I_REGISTER_INTERFACE(iprm::IParamsSet);
		I_REGISTER_INTERFACE(iser::IObject);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_slaveParamsSetCompPtr, "SlaveParamsSet", "Delegated parameters set", true, "SlaveParamsSet");
		I_ASSIGN(m_typeIdAttrPtr, "TypeId", "Type ID if differs from slave object", false, "Default");
		I_ASSIGN(m_modelCompPtr, "SlaveChangesModel", "Model to intercept changes from (can be differ from SlaveParamsSet)", false, "SlaveChangesModel");
	I_END_COMPONENT;

	CParamsSetDelegatorComp();

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::IParamsSet, m_slaveParamsSetCompPtr);
	I_ATTR(QByteArray, m_typeIdAttrPtr);
	I_REF(imod::IModel, m_modelCompPtr);
};


} // namespace iprm


#endif // !iprm_CDelegatedParamsSetComp_included


