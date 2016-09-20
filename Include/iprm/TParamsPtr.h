#ifndef iprm_TParamsPtr_included
#define iprm_TParamsPtr_included


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QStringList>

// ACF includes
#include <istd/TPointerBase.h>
#include <iattr/TAttribute.h>
#include <icomp/TAttributeMember.h>
#include <icomp/TReferenceMember.h>
#include <iprm/IParamsSet.h>


namespace iprm
{


/**
	Help pointer wrapper for management of a parameter from the parameter set.
*/
template <class ParameterInterace>
class TParamsPtr: public istd::TPointerBase<const ParameterInterace>
{
public:
	typedef istd::TPointerBase<const ParameterInterace> BaseClass;

	TParamsPtr(const ParameterInterace* ptr = NULL);

	/**
		Construct and initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Construct and initialize the pointer with the component parameters.
		\param	parameterSetPtr			parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	TParamsPtr(	const IParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<ParameterInterace>& defaultRef,
				bool isObligatory = true);

	/**
		Initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	void Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Initialize the pointer with the component parameters.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	void Init(	const IParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<ParameterInterace>& defaultRef,
				bool isObligatory = true);
};


// public methods

template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(const ParameterInterace* ptr)
:	BaseClass(ptr)
{
}


template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory)
{
	Init(parameterSetPtr, parameterId, isObligatory);
}


template <class ParameterInterace>
TParamsPtr<ParameterInterace>::TParamsPtr(
			const IParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<ParameterInterace>& defaultRef,
			bool isObligatory)
{
	Init(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory);
}


template <class ParameterInterace>
void TParamsPtr<ParameterInterace>::Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory)
{
	Q_UNUSED(isObligatory);

	if ((parameterSetPtr != NULL) && !parameterId.isEmpty()){
		const iser::ISerializable* parameterPtr = parameterSetPtr->GetParameter(parameterId);

		BaseClass::SetPtr(dynamic_cast<const ParameterInterace*>(parameterPtr));

#if QT_VERSION >= 0x040800
		I_IF_DEBUG(
			if (!BaseClass::IsValid() && isObligatory){
				iprm::IParamsSet::Ids existingParamIds = parameterSetPtr->GetParamIds();
				QStringList existingIds;
				for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
					existingIds.push_back(*index);
				}

				QString idList = existingIds.join(", ");

				if (parameterPtr == NULL){
					qDebug("Parameter %s was not found in the parameter set. Following parameter IDs are registered: %s", qPrintable(parameterId), qPrintable(idList));
				}
				else{
					qDebug("Parameter %s was found in the parameter set, but it doesn't implement the required interface: %s", qPrintable(parameterId), qPrintable(typeid(ParameterInterace).name()));				
				}
			}
		)
#endif
	}
	else{
		BaseClass::Reset();
	}
}


template <class ParameterInterace>
void TParamsPtr<ParameterInterace>::Init(
			const IParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<ParameterInterace>& defaultRef,
			bool isObligatory)
{
	Q_UNUSED(isObligatory);

	BaseClass::Reset();

	if (parameterSetPtr != NULL){
		if (parameterIdAttribute.IsValid()){
			const iser::ISerializable* paramPtr = parameterSetPtr->GetParameter(*parameterIdAttribute);
			const ParameterInterace* paramImplPtr = dynamic_cast<const ParameterInterace*>(paramPtr);
			BaseClass::SetPtr(paramImplPtr);

#if QT_VERSION >= 0x050000
			I_IF_DEBUG(
				if ((paramImplPtr == NULL) && (paramPtr != NULL)){
					qDebug("Parameter %s in parameter set is not compatible, should be %s", qPrintable(*parameterIdAttribute), qPrintable(istd::CClassInfo::GetName<ParameterInterace>()));
				}
			)
#endif
		}
	}

	if (!BaseClass::IsValid() && defaultRef.IsValid()){
		BaseClass::SetPtr(defaultRef.GetPtr());
	}

#if QT_VERSION >= 0x040800
	I_IF_DEBUG(
		if (!BaseClass::IsValid() && isObligatory){
			QString debugMessage;
			if (parameterIdAttribute.IsValid()){
				iprm::IParamsSet::Ids existingParamIds = parameterSetPtr->GetParamIds();
				QStringList existingIds;
				for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
					existingIds.push_back(*index);
				}

				QString idList = existingIds.join(", ");

				debugMessage = QString("Parameter %1 was not found in the parameter set and no default parameter is active. Following parameter IDs are registered: %2").arg(QString(*parameterIdAttribute)).arg(idList);
			}
			else{
				debugMessage = QString("Parameter was not specified and no default parameter is active");
			}
			qDebug(qPrintable(debugMessage));
		}
	)
#endif
}


} // namespace iprm


#endif // !iprm_TParamsPtr_included


