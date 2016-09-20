#ifndef imath_IUnitInfo_included
#define imath_IUnitInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TRange.h>
#include <imath/IDoubleManip.h>


namespace imath
{


/**
	Common interface to get information about a measurement unit.
*/
class IUnitInfo: virtual public istd::IPolymorphic
{
public:
	/**
		General type of unit.
	*/
	enum UnitType
	{
		/**
			Unit type is unknown.
		*/
		UT_UNKNOWN,
		/**
			Physical unit like volt or amper.
		*/
		UT_PHYSICAL,
		/**
			Technical unit without physical dimension like pixel.
		*/
		UT_TECHNICAL,
		/**
			Unit relative to something others (for example %).
		*/
		UT_RELATIVE,
		/**
			Integer number counter, it represents number of occurence of something.
		*/
		UT_COUNTER
	};

	/**
		Get type of unit.
	*/
	virtual int GetUnitType() const = 0;

	/**
		Get name of unit.
	*/
	virtual QString GetUnitName() const = 0;

	/**
		Get multiplication factor used to calculate user input values.
	*/
	virtual double GetDisplayMultiplicationFactor() const = 0;

	/**
		Get range of possible values.
		If invalid range is returned, there is no restrictions.
	*/
	virtual istd::CRange GetValueRange() const = 0;

	/**
		Return number manipulator object for numeric value.
	*/
	virtual const imath::IDoubleManip& GetValueManip() const = 0;
};


} // namespace imath


#endif // !imath_IUnitInfo_included


