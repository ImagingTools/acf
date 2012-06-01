#include "iview/CShapeStatusInfoComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iview
{


// public methods

CShapeStatusInfoComp::CShapeStatusInfoComp()
	:m_logicalPosition(0, 0),
	m_pixelPosition(0, 0)
{
}


// reimplemented (IShapeStatusInfo)

i2d::CVector2d CShapeStatusInfoComp::GetLogicalPosition() const
{
	return m_logicalPosition;
}


void CShapeStatusInfoComp::SetLogicalPosition(const i2d::CVector2d& logicalPosition)
{
	if (m_logicalPosition != logicalPosition){
		istd::CChangeNotifier changePtr(this);

		m_logicalPosition = logicalPosition;
	}
}


i2d::CVector2d CShapeStatusInfoComp::GetPixelPosition() const
{
	return m_pixelPosition;
}


void CShapeStatusInfoComp::SetPixelPosition(const i2d::CVector2d& pixelPosition)
{
	if (m_pixelPosition != pixelPosition){
		istd::CChangeNotifier changePtr(this);

		m_pixelPosition = pixelPosition;
	}
}


QString CShapeStatusInfoComp::GetInfoText() const
{
	return m_infoText;
}


void CShapeStatusInfoComp::SetInfoText(const QString& infoText)
{
	if (m_infoText != infoText){
		istd::CChangeNotifier changePtr(this);

		m_infoText = infoText;
	}
}


} // namespace iview



