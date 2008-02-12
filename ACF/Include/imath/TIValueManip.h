#ifndef imath_TIValueManip_included
#define imath_TIValueManip_included



#include <string>

#include "istd/IPolymorphic.h"

#include "imath/imath.h"


namespace imath
{


/**	Interface for all classes providing simply arithmetic functions on predefined
 *		data type.
 */
template <class ValueType>
class TIValueManip: public istd::IPolymorphic
{
public:
	/**
		Get the nearest value rounded used this arithmetik.
	*/
	virtual ValueType GetRounded(const ValueType& value) const = 0;
	/**
		Compare two values and check, if there are equal.
		\param	value1	first value.
		\param	value2	second value.
	 */
	virtual bool IsEqual(const ValueType& value1, const ValueType& value2) const = 0;
	/**
		Compare two values and check, if first one is smaller as the second.
		\param	value1	first value.
		\param	value2	second value.
	 */
	virtual bool IsSmaller(const ValueType& value1, const ValueType& value2) const = 0;
	/**
		Compare two values and check, if first one is smaller or equal as the second.
		\param	value1	first value.
		\param	value2	second value.
	 */
	virtual bool IsSmallerEqual(const ValueType& value1, const ValueType& value2) const = 0;
	/**
		Get this value as string.
		\param	value	value will be converted.
	 */
	virtual ::std::string GetString(const ValueType& value) const = 0;
	/**
		Get value converted from string.
		\param	text	input string.
		\param	result	result value.
		\return			true if success.
	 */
	virtual bool GetParsed(const ::std::string& text, ValueType& result) const = 0;
	/**
		Return biggest value smaller that specified one.
	 */
	virtual ValueType GetSmallerValue(const ValueType& value) const = 0;
	/**
		Return smallest value bigger that specified one.
	 */
	virtual ValueType GetBiggerValue(const ValueType& value) const = 0;
};


typedef TIValueManip<float> IFloatManip;
typedef TIValueManip<double> IDoubleManip;


} // namespace imath


#endif //!imath_TIValueManip_included


