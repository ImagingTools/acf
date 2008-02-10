#ifndef CMeasurementRange_included
#define CMeasurementRange_included


#include "imeas/imeas.h"


#include "istd/IPolymorphic.h"


namespace imeas
{


/**
	\ingroup imeas

	Implementation of a measurement range.
*/
class CMeasurementRange: virtual public istd::IPolymorphic
{
public:
	CMeasurementRange();

	virtual bool IsValid() const;

	virtual void SetLowerErrorLimit(double lowerErrorLimit);
	virtual void SetUpperErrorLimit(double upperErrorLimit);
	virtual void SetLowerWarningLimit(double lowerWarningLimit);
	virtual void SetUpperWarningLimit(double upperWarningLimit);

	virtual double GetLowerErrorLimit() const;
	virtual double GetUpperErrorLimit() const;
	virtual double GetLowerWarningLimit() const;
	virtual double GetUpperWarningLimit() const;

	virtual bool IsOk(double value) const;
	virtual bool IsWarning(double value) const;
	virtual bool IsError(double value) const;

private:
	double m_lowerErrorLimit;
	double m_upperErrorLimit;
	double m_lowerWarningLimit;
	double m_upperWarningLimit;
};


} // namespace imeas


#endif // !CMeasurementRange_included
