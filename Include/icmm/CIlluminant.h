#pragma once


// ACF includes
#include <icmm/IIlluminant.h>


namespace icmm
{


class CIlluminant: virtual public IIlluminant
{
public:
	CIlluminant(
				StandardIlluminant illuminantType = StandardIlluminant::D65,
				const QString& illuminantName = "D65",
				const icmm::CVarColor& whitePoint = icmm::CVarColor());
	CIlluminant(const CIlluminant& other);

	// reimplemented (IWhitePointProvider)
	virtual icmm::CVarColor GetWhitePoint() const override;

	// reimplemented (IIluminant)
	virtual void SetWhitePoint(const icmm::CVarColor& whitePoint) override;
	virtual QString GetIllumninantName() const override;
	virtual void SetIllumninantName(const QString& illuminantName) override;
	virtual StandardIlluminant GetIllumninantType() const override;
	virtual void SetIllumninantType(const StandardIlluminant& illuminantType) override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const istd::IChangeable& other) const override;

private:
	StandardIlluminant m_illuminantType = StandardIlluminant::D65;
	QString m_illuminantName = "D65";
	icmm::CVarColor m_whitePoint;
};


} // namespace icmm


