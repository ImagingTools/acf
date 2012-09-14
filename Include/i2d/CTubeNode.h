#ifndef i2d_CTubeNode_included
#define i2d_CTubeNode_included


// ACF includes
#include "istd/TRange.h"

#include "iser/ISerializable.h"

#include "i2d/i2d.h"


namespace i2d
{


class CTubeNode: public iser::ISerializable
{
public:
	CTubeNode();

	/**
		Get left and right aoi value.
		Left aoi should be negative and right positive.
	*/
	const istd::CRange& GetTubeRange() const;
	void SetTubeRange(const istd::CRange& tubeRange);

	/**
		Set this object to be linear interpolated between first and second parameter.
		\param	first	first node parameter.
		\param	second	second node parameter.
		\param	alpha	when 0 result equals first parameter, when 1 equals second.
		For other values is linear interpolated between this two edge parameters.
	*/
	virtual void SetInterpolated(const CTubeNode& first, const CTubeNode& second, double alpha);

	// reimplemented (qcom::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_tubeRange;
};


} // namespace i2d


#endif // !i2d_CTubeNode_included


