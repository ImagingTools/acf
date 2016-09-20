#pragma once


// ACF includes
#include <ilog/IMessageConsumer.h>


namespace iprm
{


class IParamsSet;


/**
	Interface for consistency checking of a parameter set.
*/
class IParamsSetValidator: virtual public istd::IPolymorphic
{
public:
	typedef QSet<QByteArray> Ids;

	/**
		Get list of parameter type IDs which can be checked by the validator.
	*/
	virtual Ids	GetSupportedTypeIds() const = 0;

	/**
		Return \c true if the parameter set is consistent or \c false otherwise.
		\param paramsSet	Parameter set to be validated
		\param validationMessagesConsumerPtr (optional) Consumer for validation messages.
	*/
	virtual bool IsParamsSetConsistent(const IParamsSet& paramsSet, ilog::IMessageConsumer* validationMessagesConsumerPtr = NULL) const = 0;

};


} // namespace iprm


