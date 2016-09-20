#ifndef imath_TISurjectFunction_included
#define imath_TISurjectFunction_included


#include <imath/TIMathFunction.h>


namespace imath
{


/**
	Template interface for any surjective mathematical function.
	A function is surjective if for every y in the codomain Y
	there is at least one x in the domain X such that f(x) = y.
*/
template <typename Argument, typename Result>
class TISurjectFunction: virtual public TIMathFunction<Argument, Result>
{
public:
	/**
		Get inverted function value.
		\param	argument	function argument.
		\param	result		function value for specified argument.
		\return	true if result was correctly calculated.
	*/
	virtual bool GetInvValueAt(const Result& argument, Argument& result) const = 0;
	/**
		Get inverted function value.
		This overloaded menthod is provided only for convinence.
		\param	argument	function argument.
		\return				function value for specified argument.
	*/
	virtual Argument GetInvValueAt(const Result& argument) const = 0;
};


// standard templatization

typedef TISurjectFunction<double, double> ISurjectDoubleFunction;


} // namespace imath


#endif // !imath_TISurjectFunction_included


