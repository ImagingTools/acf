#ifndef i2d_TObject2dCompWrap_included
#define i2d_TObject2dCompWrap_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "i2d/ICalibration2d.h"
#include "i2d/IObject2d.h"


namespace i2d
{


/**
	Base class for all components implementing 2D objects.
	The template parameter \c BaseObject2d should be set to non-component 2D object implementing interface \c i2d::IObject2d.
*/
template <class BaseObject2d>
class TObject2dCompWrap: public icomp::CComponentBase, public BaseObject2d
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef BaseObject2d BaseClass2;

	I_BEGIN_COMPONENT(TObject2dCompWrap);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Calibration associated with this 2d object", false, "Calibration");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class BaseObject2d>
void TObject2dCompWrap<BaseObject2d>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	BaseClass2::SetCalibration(m_calibrationCompPtr.GetPtr());
}


} // namespace i2d


#endif // !i2d_TObject2dCompWrap_included


