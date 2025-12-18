#include <iview/CShapeStatusInfoComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iview/IDisplay.h>


namespace iview
{


// public methods

// reimplemented (iview::IShapeStatusInfo)

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



