#ifndef iprm_CLinearAdjustParamsComp_included
#define iprm_CLinearAdjustParamsComp_included


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "iprm/ILinearAdjustParams.h"
#include "iprm/ILinearAdjustConstraints.h"


namespace iprm
{


/**
	Basic implementation of component of linear adjust parameters.
*/
class CLinearAdjustParamsComp:
			public icomp::CComponentBase,
			virtual public ILinearAdjustParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLinearAdjustParamsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(ILinearAdjustParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints describing allowed value ranges", false, "Constraints");
		I_ASSIGN(m_defaultScaleAttrPtr, "DefaultScale", "Default value of scale factor", true, 1);
		I_ASSIGN(m_defaultOffsetAttrPtr, "DefaultOffset", "Default value of offset factor", true, 0);
	I_END_COMPONENT;

	CLinearAdjustParamsComp();

	// reimplemented (iprm::ILinearAdjustParams)
	virtual const ILinearAdjustConstraints* GetAdjustConstraints() const;
	virtual double GetScaleFactor() const;
	virtual void SetScaleFactor(double scale);
	virtual double GetOffsetFactor() const;
	virtual void SetOffsetFactor(double offset);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_REF(ILinearAdjustConstraints, m_constraintsCompPtr);
	I_ATTR(double, m_defaultScaleAttrPtr);
	I_ATTR(double, m_defaultOffsetAttrPtr);

	double m_scale;
	double m_offset;
};


} // namespace iprm


#endif // !iprm_CLinearAdjustParamsComp_included


