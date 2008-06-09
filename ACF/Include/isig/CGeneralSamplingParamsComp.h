#ifndef isig_CGeneralSamplingParamsComp_included
#define isig_CGeneralSamplingParamsComp_included


#include "icomp/CComponentBase.h"

#include "isig/ISamplingParams.h"


namespace isig
{


/**
	General implementation of sampling parameters.
	In this implementation the boundaries are fix defined using component attributes.
*/
class CGeneralSamplingParamsComp: public icomp::CComponentBase, virtual public isig::ISamplingParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CGeneralSamplingParamsComp)
		I_REGISTER_INTERFACE(iser::ISerializable)
		I_REGISTER_INTERFACE(isig::ISamplingParams)
		I_ASSIGN(m_minIntervalAttrPtr, "MinInterval", "Minimal interval of single sample", true, 0.001);
		I_ASSIGN(m_maxIntervalAttrPtr, "MaxInterval", "Maximal interval of single sample", true, 1.000);
		I_ASSIGN(m_isSingleModeSupportedAttrPtr, "IsSingleModeSupported", "Active if single mode should be supported", true, true);
		I_ASSIGN(m_isPeriodicModeSupportedAttrPtr, "IsPeriodicModeSupported", "Active if periodic mode should be supported", true, true);
		I_ASSIGN(m_isSynchronizedModeSupportedAttrPtr, "IsSynchronizedModeSupported", "Active if synchronized mode should be supported", true, true);
	I_END_COMPONENT

	CGeneralSamplingParamsComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (isig::ISamplingParams)
	virtual istd::CRange GetIntervalRange() const;
	virtual double GetInterval() const;
	virtual void SetInterval(double value);
	virtual bool IsSamplingModeSupported(int mode) const;
	virtual int GetSamplingMode() const;
	virtual bool SetSamplingMode(int mode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_interval;
	int m_samplingMode;

	I_ATTR(double, m_minIntervalAttrPtr);
	I_ATTR(double, m_maxIntervalAttrPtr);
	I_ATTR(bool, m_isSingleModeSupportedAttrPtr);
	I_ATTR(bool, m_isPeriodicModeSupportedAttrPtr);
	I_ATTR(bool, m_isSynchronizedModeSupportedAttrPtr);
};


} // namespace isig


#endif // !isig_CGeneralSamplingParamsComp_included


