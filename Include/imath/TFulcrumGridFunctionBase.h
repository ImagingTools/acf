#ifndef imath_TFulcrumGridFunctionBase_included
#define imath_TFulcrumGridFunctionBase_included


// ACF includes
#include <istd/TCachedUpdateManagerWrap.h>
#include <imath/TIMathFunction.h>


namespace imath
{


/**
	Base class for interpolated functions based on fulcrums in multi-dimesional grid.
	This grid may be imagined as set of parallel layers for each dimension.
	Number of this layers and their position can be individually controlled.
	Plese note, that method SetDimensionsCount must be called manually to initialize number of input dimensions also for fixed-size arguments.
	\param	Argument	function argument type.
	\param	Result		function result type.
	\param	Fulcrums	array type used to stored fulcrums. It muss define following public typedefs
						\li \c ElementType	type of element stored in this array,
						\li \c IndexType	type of index used to address elements,
						\li \c SizesType	type using to represent array size. This type muss define public typedef \c IndexType.
*/
template <class Argument, class Result, class Fulcrums>
class TFulcrumGridFunctionBase:
			public TFulcrumGrid<Argument, Fulcrums>,
			public istd::TCachedUpdateManagerWrap<iser::ISerializable>,
			public TIMathFunction<Argument, Result>
{
public:
	typedef TFulcrumGrid<Argument, Fulcrums> BaseClass;
	typedef istd::TCachedUpdateManagerWrap<iser::ISerializable> BaseClass2;
	typedef TIMathFunction<Argument, Result> BaseClass3;

	typedef typename TIMathFunction<Argument, Result>::ArgumentType ArgumentType;

protected:
	// reimplemented (istd::TCachedUpdateManagerWrap)
	virtual bool CalculateCache(const ChangeSet& changeSet);
};



// reimplemented (istd::TCachedUpdateManagerWrap)

template <class Argument, class Result, class Fulcrums>
bool TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::CalculateCache(const istd::IChangeable::ChangeSet& changeSet)
{
	bool retVal = BaseClass2::CalculateCache(changeSet);

	if (changeSet.Contains(CF_SORT_LAYERS)){
		SortFulcrums();
	}

	return retVal;
}


} // namespace imath


#endif // !imath_TFulcrumGridFunctionBase_included


