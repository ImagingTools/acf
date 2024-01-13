#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <imath/IUnitInfo.h>


namespace imath
{


/**
	General implementation of the IUnitInfo interface.
*/
class CGeneralUnitInfo:
			virtual public IUnitInfo,
			virtual public iser::ISerializable
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

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	//	reimplemented (imath::IUnitInfo)
	virtual int GetUnitType() const override;
	virtual QString GetUnitName() const override;
	virtual double GetDisplayMultiplicationFactor() const override;
	virtual istd::CRange GetValueRange() const override;
	virtual const imath::IDoubleManip& GetValueManip() const override;

private:
	int m_type;
	QString m_name;
	double m_displayMultFactor;
	istd::CRange m_range;
	const imath::IDoubleManip* m_valueManipPtr;
};


} // namespace imath


