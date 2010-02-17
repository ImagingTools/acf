#ifndef iqtgui_IDropConsumer_included
#define iqtgui_IDropConsumer_included


// Qt includes
#include <QMimeData>
#include <QEvent>


// ACF includes
#include "istd/IPolymorphic.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


/**
	Interface for objects which can receive dragged objects
*/
class IDropConsumer: virtual public istd::IPolymorphic
{
public:
	/**
		Get the list of the possible mime IDs, which are accepted by the consumer.
	*/
	virtual QStringList GetAcceptedMimeIds() const = 0;

	/**
		Notification about drop action finish.
	*/
	virtual void OnDropFinished(const QMimeData& mimeData, QEvent* eventPtr) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IDropConsumer_included


