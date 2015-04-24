#include "iview/CShapeStatusInfoComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "iview/IDisplay.h"


namespace iview
{


// public methods

CShapeStatusInfoComp::CShapeStatusInfoComp()
	:m_logicalPosition(0, 0),
	m_pixelPosition(0, 0)
{
}


// reimplemented (iview::IShapeStatusInfo)

i2d::CVector2d CShapeStatusInfoComp::GetLogicalPosition() const
{
	return m_logicalPosition;
}


void CShapeStatusInfoComp::SetLogicalPosition(const i2d::CVector2d& logicalPosition)
{
	if (m_logicalPosition != logicalPosition){
		static const istd::IChangeable::ChangeSet changeSet(IDisplay::CS_CONSOLE, i2d::IObject2d::CF_OBJECT_POSITION);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

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
		static const istd::IChangeable::ChangeSet changeSet(IDisplay::CS_CONSOLE);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

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
		static const istd::IChangeable::ChangeSet changeSet(IDisplay::CS_CONSOLE);
		istd::CChangeNotifier notifier(this, &changeSet);
		Q_UNUSED(notifier);

		m_infoText = infoText;
	}
}


} // namespace iview



