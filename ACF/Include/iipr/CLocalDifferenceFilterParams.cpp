#include "iipr/CLocalDifferenceFilterParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CLocalDifferenceFilterParams::CLocalDifferenceFilterParams()
	:m_alpha(1),
	m_beta(1.0),
	m_gamma(0)
{
}


void CLocalDifferenceFilterParams::SetAlpha(int alpha)
{
	m_alpha = alpha;
}


int CLocalDifferenceFilterParams::GetAlpha() const
{
	return m_alpha;
}


void CLocalDifferenceFilterParams::SetBeta(double beta)
{
	m_beta = beta;
}


double CLocalDifferenceFilterParams::GetBeta() const
{
	return m_beta;
}


void CLocalDifferenceFilterParams::SetGamma(int gamma)
{
	m_gamma = gamma;
}


int CLocalDifferenceFilterParams::GetGamma() const
{
	return m_gamma;
}


// reimplemented (iser::ISerializable)

bool CLocalDifferenceFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag alphaTag("Alpha", "Alpha");
	bool retVal = archive.BeginTag(alphaTag);
	retVal = retVal && archive.Process(m_alpha);
	retVal = retVal && archive.EndTag(alphaTag);

	static iser::CArchiveTag betaTag("Beta", "Beta");
	retVal = retVal && archive.BeginTag(betaTag);
	retVal = retVal && archive.Process(m_beta);
	retVal = retVal && archive.EndTag(betaTag);

	static iser::CArchiveTag gammaTag("Beta", "Beta");
	retVal = retVal && archive.BeginTag(gammaTag);
	retVal = retVal && archive.Process(m_gamma);
	retVal = retVal && archive.EndTag(gammaTag);

	return retVal;
}


} // namespace iipr

