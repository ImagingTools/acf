#ifndef iprm_CParamsSet_included
#define iprm_CParamsSet_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TOptInterfacePtr.h>
#include <istd/TPointerVector.h>
#include <imod/CModelUpdateBridge.h>
#include <iprm/IParamsSet.h>


namespace iprm
{


/**
	Basic implementation of interface IParamsSet.
*/
class CParamsSet: virtual public IParamsSet
{
public:
	struct ParameterInfo
	{
		ParameterInfo(const QByteArray& parameterId, iser::ISerializable* parameterPtr)
		{
			this->parameterPtr.SetOptionalPtr(parameterPtr);
			this->parameterId = parameterId;
		}

		ParameterInfo(const QByteArray& parameterId, iser::ISerializableUniquePtr& parameterPtr)
		{
			this->parameterPtr.SetPtr(parameterPtr);
			this->parameterId = parameterId;
		}

		QByteArray parameterId;
		istd::TOptInterfacePtr<iser::ISerializable> parameterPtr;
	};
	typedef istd::TPointerVector<ParameterInfo> ParameterInfos;

	explicit CParamsSet(const IParamsSet* slaveSetPtr = NULL);

	/**
		Get slave parameter set.
		Slave parameter set will be used for non editable parameter query.
		If no slave parameter set is defined, it returns NULL.
	*/
	const IParamsSet* GetSlaveSet() const;
	/**
		Set slave parameter set.
		Slave parameter set will be used for non editable parameter query.
		\param	slaveSetPtr		slave parameter set, or NULL, if no set is used.
	*/
	void SetSlaveSet(const IParamsSet* slaveSetPtr);

	/**
		Set editable parameter in this set.
		Editable parameters are stored in set directly, the non editable in slave sets.
	*/
	virtual bool SetEditableParameter(const QByteArray& id, iser::ISerializable* parameterPtr, bool releaseFlag = false);

	/**
		Set editable parameter in this set.
		Editable parameters are stored in set directly, the non editable in slave sets.
	*/
	virtual bool SetEditableParameter(const QByteArray& id, iser::ISerializableUniquePtr& parameterPtr);

	/**
		Get access to all parameters.
	*/
	const ParameterInfos& GetParameterInfos() const;

	/**
		Get ID of this parameters type.
	*/
	const QByteArray& GetParametersTypeId() const;
	/**
		Set ID of this parameters type.
	*/
	void SetParametersTypeId(const QByteArray& id);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const override;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const override;
	virtual iser::ISerializable* GetEditableParameter(const QByteArray& id) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;
	virtual quint32 GetMinimalVersion(int versionId) const override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	ParameterInfo* FindParameterInfo(const QByteArray& parameterId) const;

	ParameterInfos m_params;

	QByteArray m_paramsTypeId;

	const IParamsSet* m_slaveSetPtr;

	imod::CModelUpdateBridge m_updateBridge;
};


// inline methods

inline const IParamsSet* CParamsSet::GetSlaveSet() const
{
	return m_slaveSetPtr;
}


inline void CParamsSet::SetSlaveSet(const IParamsSet* slaveSetPtr)
{
	m_slaveSetPtr = slaveSetPtr;
}


} // namespace iprm


#endif // !iprm_CParamsSet_included


