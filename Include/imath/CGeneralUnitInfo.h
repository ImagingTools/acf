#ifndef imath_CGeneralUnitInfo_included
#define imath_CGeneralUnitInfo_included


// ACF includes
#include <iser/IArchive.h>
#include <imath/IUnitInfo.h>


namespace imath
{


/**
	General implementation of the IUnitInfo interface.
*/
class CGeneralUnitInfo: virtual public IUnitInfo
{
public:
	CGeneralUnitInfo(
				int type = UT_UNKNOWN,
				const QString& name = "",
				double displayMultFactor = 1.0,
				const istd::CRange& range = istd::CRange::GetInvalid(),
				const imath::IDoubleManip* valueManipPtr = NULL);

	void SetUnitType(int type);
	void SetUnitName(const QString& name);
	void SetDisplayMultiplicationFactor(double factor);
	void SetValueRange(const istd::CRange& range);

	virtual bool Serialize(iser::IArchive& archive);

	//	reimplemented (imath::IUnitInfo)
	virtual int GetUnitType() const;
	virtual QString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	int m_type;
	QString m_name;
	double m_displayMultFactor;
	istd::CRange m_range;
	const imath::IDoubleManip* m_valueManipPtr;
};


} // namespace imath


#endif // !imath_CGeneralUnitInfo_included


