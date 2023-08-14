#ifndef i2d_TObject2dCompWrap_included
#define i2d_TObject2dCompWrap_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <i2d/ICalibration2d.h>
#include <i2d/IObject2d.h>
#include <i2d/IObject2dProvider.h>

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
		I_ASSIGN(m_defaultObjectProviderCompPtr, "DefaultObject2dProvider", "Default object prototype provider", false, "DefaultObject2dProvider");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	I_REF(i2d::ICalibration2d, m_calibrationCompPtr);
	I_REF(i2d::IObject2dProvider, m_defaultObjectProviderCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class BaseObject2d>
void TObject2dCompWrap<BaseObject2d>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	BaseClass2::SetCalibration(m_calibrationCompPtr.GetPtr(), false);

	if (m_defaultObjectProviderCompPtr.IsValid()) {
		const i2d::IObject2d* defaultGeomPtr = m_defaultObjectProviderCompPtr->GetObject2d();
		if (defaultGeomPtr != nullptr) {
			BaseClass2::CopyFrom(*defaultGeomPtr);
		}

		const i2d::ICalibration2d* calibrationPtr = BaseClass2::GetCalibration();

		if (calibrationPtr != NULL) {
			BaseClass2::InvTransform(*calibrationPtr);
			BaseClass2::SetCalibration(calibrationPtr);
		}
	}
}


} // namespace i2d


#endif // !i2d_TObject2dCompWrap_included


