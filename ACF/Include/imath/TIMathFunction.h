#ifndef imat_TIMathFuntion_included
#define imat_TIMathFuntion_included


namespace imath
{


template <class ArgumentType, class ResultType>
class TIMathFuntion: virtual public istd::IPolymorphic
{
public:
	/**
		Get function value for specified argument value.
		\param	argument	function argument.
		\param	result		function value for specified argument.
		\return	true if result was correctly calculated.
	*/
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const = 0;
	/**
		Get function value for specified argument value.
		This overloaded menthod is provided only for convinence.
		\param	argument	function argument.
		\return				function value for specified argument.
	*/
	virtual ResultType GetValueAt(const ArgumentType& argument) const = 0;
};


// standard templatization

typedef TIMathFuntion<double, double> CDoubleFunction;


} // namespace imath


#endif //!imat_TIMathFuntion_included


