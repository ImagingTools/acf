#ifndef istd_CRandomNumber_included
#define istd_CRandomNumber_included


namespace istd
{


class CRandomNumber
{
public:
	CRandomNumber();

	int GetNextInt(int minValue, int maxValue) const;
	double GetNextDouble(double minValue, double maxValue) const;

protected:
	double GetNextUnitary() const;
};


} // namespace istd


#endif // !istd_CRandomNumber_included


