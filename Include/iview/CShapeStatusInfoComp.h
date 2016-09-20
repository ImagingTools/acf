#ifndef iview_CShapeStatusInfoComp_included
#define iview_CShapeStatusInfoComp_included


// ACF includes
#include <icomp/CComponentBase.h>

#include <iview/IShapeStatusInfo.h>


namespace iview
{


/**
	Standard implementation of IShapeStatusInfo interface.
*/
class CShapeStatusInfoComp: public icomp::CComponentBase, virtual public IShapeStatusInfo
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_BEGIN_COMPONENT(CShapeStatusInfoComp);
		I_REGISTER_INTERFACE(IShapeStatusInfo);
	I_END_COMPONENT;

	CShapeStatusInfoComp();

	// reimplemented (iview::IShapeStatusInfo)
	virtual i2d::CVector2d GetLogicalPosition() const;
	virtual void SetLogicalPosition(const i2d::CVector2d& logicalPosition);
	virtual i2d::CVector2d GetPixelPosition() const;
	virtual void SetPixelPosition(const i2d::CVector2d& pixelPosition);
	virtual QString GetInfoText() const;
	virtual void SetInfoText(const QString& infoText);

private:
	i2d::CVector2d m_logicalPosition;
	i2d::CVector2d m_pixelPosition;
	QString m_infoText;
};


} // namespace iview


#endif // !iview_CShapeStatusInfoComp_included


