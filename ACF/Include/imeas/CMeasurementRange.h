#ifndef imeas_CMeasurementRange_included
#define imeas_CMeasurementRange_included


#include "imeas/imeas.h"


#include "istd/CRange.h"


namespace imeas
{


/**
	\ingroup imeas

	Implementation of a measurement range.
*/
class CMeasurementRange
{
public:
	CMeasurementRange();

	bool IsValid() const;

	void SetLowerErrorLimit(double lowerErrorLimit);
	void SetUpperErrorLimit(double upperErrorLimit);
	void SetLowerWarningLimit(double lowerWarningLimit);
	void SetUpperWarningLimit(double upperWarningLimit);

	double GetLowerErrorLimit() const;
	double GetUpperErrorLimit() const;
	double GetLowerWarningLimit() const;
	double GetUpperWarningLimit() const;

	bool IsOk(double value) const;
	bool IsWarning(double value) const;
	bool IsError(double value) const;

	bool operator == (const CMeasurementRange& otherRange) const;
	bool operator != (const CMeasurementRange& otherRange) const;

private:
	istd::CRange m_warningRange;
	istd::CRange m_errorRange;
};


} // namespace imeas


#endif // !imeas_CMeasurementRange_included
