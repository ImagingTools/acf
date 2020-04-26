#ifndef iprm_IParameterStateProvider_included
#define iprm_IParameterStateProvider_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QList>


// ACF includes
#include <istd/IChangeable.h>


namespace iprm
{


class IParamsSet;


/**
	Provider of the state information for a parameter inside of the parameter set.
*/
class IParameterStateProvider: virtual public istd::IChangeable
{
public:
	enum StateType
	{
		/**
			Get the "edit" status of the parameter.
		*/
		ST_EDIT
	};

	enum ParameterState
	{
		PS_UNKNOWN,
		PS_TRUE,
		PS_FALSE
	};

	/**
		Return state for the given parameter.
	*/
	virtual ParameterState GetState(
				const iprm::IParamsSet& paramSet,
				const QByteArray& parameterId,
				StateType stateType) const = 0;
};


} // namespace iprm


#endif // !iprm_IParameterStateProvider_included


