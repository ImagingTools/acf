#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>

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
template <class ParamsSet, class ParameterInterace>
class TParamsPtrBase: public istd::TPointerBase<ParameterInterace>
{
public:
	typedef istd::TPointerBase<ParameterInterace> BaseClass;
	using NonConstParameterInterface = std::remove_const_t<ParameterInterace>;

	TParamsPtrBase(ParameterInterace* ptr = NULL);

	/**
		Construct and initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	TParamsPtrBase(ParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Construct and initialize the pointer with the component parameters.
		\param	parameterSetPtr			parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	TParamsPtrBase(ParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<NonConstParameterInterface>& defaultRef,
				bool isObligatory = true);

	/**
		Initialize the pointer with the given parameter set and parameter ID.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterId		ID of parameter in the set.
		\param	isObligatory	indicate, that this parameter is obligatory.
	*/
	void Init(ParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true);

	/**
		Initialize the pointer with the component parameters.
		\param	parameterSetPtr	parameter set, parameter will be taken from this set.
		\param	parameterIdAttribute	component attribute storing ID of parameter in the set.
		\param	defaultRef				component reference used if parameter is not found in the set.
		\param	isObligatory			indicate, that this parameter is obligatory.
	*/
	void Init(ParamsSet* parameterSetPtr,
				const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
				const icomp::TReferenceMember<NonConstParameterInterface>& defaultRef,
				bool isObligatory = true);
};


// public methods

template <class ParamsSet, class ParameterInterace>
TParamsPtrBase<ParamsSet, ParameterInterace>::TParamsPtrBase(ParameterInterace* ptr)
:	BaseClass(ptr)
{
}


template <class ParamsSet, class ParameterInterace>
TParamsPtrBase<ParamsSet, ParameterInterace>::TParamsPtrBase(
	ParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory)
{
	Init(parameterSetPtr, parameterId, isObligatory);
}


template <class ParamsSet, class ParameterInterace>
TParamsPtrBase<ParamsSet, ParameterInterace>::TParamsPtrBase(
			ParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<NonConstParameterInterface>& defaultRef,
			bool isObligatory)
{
	Init(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory);
}


template <class ParamsSet, class ParameterInterace>
void TParamsPtrBase<ParamsSet, ParameterInterace>::Init(
	ParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory)
{
	Q_UNUSED(isObligatory);
	Q_STATIC_ASSERT(std::is_const<ParamsSet>{} == std::is_const<ParameterInterace>{});

	BaseClass::Reset();

	if ((parameterSetPtr != NULL) && !parameterId.isEmpty()){
		const iser::ISerializable* constParameterPtr = nullptr;

		if constexpr (std::is_const<ParamsSet>{}){
			constParameterPtr = parameterSetPtr->GetParameter(parameterId);
			BaseClass::SetPtr(dynamic_cast<ParameterInterace*>(constParameterPtr));
		}
		else{
			iser::ISerializable* parameterPtr = parameterSetPtr->GetEditableParameter(parameterId);
			BaseClass::SetPtr(dynamic_cast<ParameterInterace*>(parameterPtr));

			constParameterPtr = parameterPtr;
		}

#if QT_VERSION >= 0x040800
		I_IF_DEBUG(
			if (!BaseClass::IsValid() && isObligatory){
				iprm::IParamsSet::Ids existingParamIds = parameterSetPtr->GetParamIds();
				QStringList existingIds;
				for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
					existingIds.push_back(*index);
				}

				QString idList = existingIds.join(", ");

				if (constParameterPtr == NULL){
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


template <class ParamsSet, class ParameterInterace>
void TParamsPtrBase<ParamsSet, ParameterInterace>::Init(
			ParamsSet* parameterSetPtr,
			const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
			const icomp::TReferenceMember<NonConstParameterInterface>& defaultRef,
			bool isObligatory)
{
	Q_UNUSED(isObligatory);
	Q_STATIC_ASSERT(std::is_const<ParamsSet>{} == std::is_const<ParameterInterace>{});

	BaseClass::Reset();

	if (parameterIdAttribute.IsValid()){
		if (parameterSetPtr != NULL){
			const iser::ISerializable* constParameterPtr = nullptr;

			if constexpr (std::is_const<ParamsSet>{}){
				constParameterPtr = parameterSetPtr->GetParameter(*parameterIdAttribute);
				BaseClass::SetPtr(dynamic_cast<ParameterInterace*>(constParameterPtr));
			}
			else{
				iser::ISerializable* parameterPtr = parameterSetPtr->GetEditableParameter(*parameterIdAttribute);
				BaseClass::SetPtr(dynamic_cast<ParameterInterace*>(parameterPtr));

				constParameterPtr = parameterPtr;
			}

#if QT_VERSION >= 0x050000
#ifndef QT_NO_DEBUG
			if ((!BaseClass::IsValid()) && (constParameterPtr != NULL)){
				qDebug("Parameter %s in parameter set is not compatible, should be %s", qPrintable(*parameterIdAttribute), qPrintable(istd::CClassInfo::GetName<ParameterInterace>()));
			}
#endif
#endif
		}
#if QT_VERSION >= 0x050000
#ifndef QT_NO_DEBUG
		else if (isObligatory){
			qDebug("Parameter set unavailable, cannot extract obligatory parameter %s", qPrintable(*parameterIdAttribute));
		}
#endif
#endif
	}

	if (!BaseClass::IsValid() && defaultRef.IsValid()){
		BaseClass::SetPtr(defaultRef.GetPtr());
	}

#if QT_VERSION >= 0x040800
#ifndef QT_NO_DEBUG
	if (!BaseClass::IsValid() && isObligatory){
		QString debugMessage;

		if (parameterSetPtr == NULL){
			debugMessage = QString("There is no parameter set and no default parameter is active");
		}
		else if (parameterIdAttribute.IsValid()){
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

		qDebug() << debugMessage;
	}
#endif
#endif
}


template <class ParameterInterace>
class TParamsPtr: public TParamsPtrBase<const IParamsSet, const ParameterInterace>{
public:
	typedef TParamsPtrBase<const IParamsSet, const ParameterInterace> BaseClass;

	TParamsPtr(const ParameterInterace* ptr = NULL)
		:BaseClass(ptr)
	{
	}

	TParamsPtr(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true)
		:BaseClass(parameterSetPtr, parameterId, isObligatory)
	{
	}

	TParamsPtr(
		const IParamsSet* parameterSetPtr,
		const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
		const icomp::TReferenceMember<ParameterInterace>& defaultRef,
		bool isObligatory = true)
		:BaseClass(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory)
	{
	}

	void Init(const IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true)
	{
		BaseClass::Init(parameterSetPtr, parameterId, isObligatory);
	}

	void Init(const IParamsSet* parameterSetPtr,
		const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
		const icomp::TReferenceMember<ParameterInterace>& defaultRef,
		bool isObligatory = true)
	{
		BaseClass::Init(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory);
	}
};


template <class ParameterInterace>
class TEditableParamsPtr : public TParamsPtrBase<IParamsSet, ParameterInterace>
{
public:
	typedef TParamsPtrBase<IParamsSet, ParameterInterace> BaseClass;

	TEditableParamsPtr(ParameterInterace* ptr = NULL)
		:BaseClass(ptr)
	{
	}

	TEditableParamsPtr(IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true)
		:BaseClass(parameterSetPtr, parameterId, isObligatory)
	{
	}

	TEditableParamsPtr(
		IParamsSet* parameterSetPtr,
		const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
		const icomp::TReferenceMember<ParameterInterace>& defaultRef,
		bool isObligatory = true)
		:BaseClass(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory)
	{
	}

	void Init(IParamsSet* parameterSetPtr, const QByteArray& parameterId, bool isObligatory = true)
	{
		BaseClass::Init(parameterSetPtr, parameterId, isObligatory);
	}

	void Init(IParamsSet* parameterSetPtr,
		const icomp::TAttributeMember<iattr::CIdAttribute>& parameterIdAttribute,
		const icomp::TReferenceMember<ParameterInterace>& defaultRef,
		bool isObligatory = true)
	{
		BaseClass::Init(parameterSetPtr, parameterIdAttribute, defaultRef, isObligatory);
	}
};


} // namespace iprm


