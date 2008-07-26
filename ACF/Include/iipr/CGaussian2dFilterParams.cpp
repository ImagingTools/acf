#include "iipr/CGaussian2dFilterParams.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CGaussian2dFilterParams::CGaussian2dFilterParams()
:	m_sigma1(1.0),
	m_sigma2(1.0),
	m_angle(0.0)
{
}


double CGaussian2dFilterParams::GetSigma1() const
{
	return m_sigma1;
}


void CGaussian2dFilterParams::SetSigma1(double sigma1)
{
	if (m_sigma1 != sigma1){
		istd::CChangeNotifier changeNotifier(this);

		m_sigma1 = sigma1;
	}
}

 
double CGaussian2dFilterParams::GetSigma2() const
{
	return m_sigma2;
}


void CGaussian2dFilterParams::SetSigma2(double sigma2)
{
	if (m_sigma2 != sigma2){
		istd::CChangeNotifier changeNotifier(this);

		m_sigma2 = sigma2;
	}
}


double CGaussian2dFilterParams::GetAngle() const
{
	return m_angle;
}


void CGaussian2dFilterParams::SetAngle(double angle)
{
	if (m_angle != angle){
		istd::CChangeNotifier changeNotifier(this);

		m_angle = angle;
	}
}


// reimplemented (iser::ISerializable)

bool CGaussian2dFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag sigma1Tag("Sigma1", "Sigma1");
	bool retVal = archive.BeginTag(sigma1Tag);
	retVal = retVal && archive.Process(m_sigma1);
	retVal = retVal && archive.EndTag(sigma1Tag);

	static iser::CArchiveTag sigma2Tag("Sigma2", "Sigma2");
	retVal = retVal && archive.BeginTag(sigma2Tag);
	retVal = retVal && archive.Process(m_sigma2);
	retVal = retVal && archive.EndTag(sigma2Tag);

	static iser::CArchiveTag angleTag("Angle", "Angle");
	retVal = retVal && archive.BeginTag(angleTag);
	retVal = retVal && archive.Process(m_angle);
	retVal = retVal && archive.EndTag(angleTag);

	return retVal;
}


} // namespace iipr

