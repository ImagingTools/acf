#ifndef imat_TIMathFunction_included
#define imat_TIMathFunction_included


#include "istd/IPolymorphic.h"


namespace imath
{


template <typename Argument, typename Result>
class TIMathFunction: virtual public istd::IPolymorphic
{
public:
	typedef Argument ArgumentType;
	typedef Result ResultType;

	/**
		Get function value for specified argument value.
		\param	argument	function argument.
		\param	result		function value for specified argument.
		\return	true if result was correctly calculated.
	*/
	virtual bool GetValueAt(const Argument& argument, Result& result) const = 0;
	/**
		Get function value for specified argument value.
		This overloaded menthod is provided only for convinence.
		\param	argument	function argument.
		\return				function value for specified argument.
	*/
	virtual Result GetValueAt(const Argument& argument) const = 0;
};


// standard templatization

typedef TIMathFunction<double, double> IDoubleFunction;


} // namespace imath


#endif // !imat_TIMathFunction_included


