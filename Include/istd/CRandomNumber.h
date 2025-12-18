#ifndef istd_CRandomNumber_included
#define istd_CRandomNumber_included


namespace istd
{


/**
	Helper class for calculation of a random number.
*/
class CRandomNumber
{
public:
	CRandomNumber();

	/**
		Get new random integer value in the given range.
	*/
	int GetNextInt(int minValue, int maxValue) const;

	/**
		Get new random real value in the given range.
	*/
	double GetNextDouble(double minValue, double maxValue) const;

protected:
	double GetNextUnitary() const;
};


} // namespace istd


#endif // !istd_CRandomNumber_included


