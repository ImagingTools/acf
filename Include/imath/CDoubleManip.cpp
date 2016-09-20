#include <imath/CDoubleManip.h>


namespace imath
{


CDoubleManip::CDoubleManip()
:	m_pointChar('.')
{
}


// reimplemented (imath::IDoubleManip)

int CDoubleManip::GetPrecision() const
{
	return 15;	// approximated precision of type double with the leading digit before the point
}


// reimplemented (imath::TIValueManip)

QString CDoubleManip::GetString(const double& value) const
{
	return QString::number(value);
}


bool CDoubleManip::GetParsed(const QString& text, double& result) const
{
	bool isOk;

	result = text.toDouble(&isOk);

	return isOk;
}


} // namespace imath


