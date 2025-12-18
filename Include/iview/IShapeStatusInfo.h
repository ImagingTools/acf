#ifndef iview_IShapeStatusInfo_included
#define iview_IShapeStatusInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>


namespace iview
{


/**
	Interface for access local bitmap information.
*/
class IShapeStatusInfo: virtual public istd::IChangeable
{
public:
	/**
		Get information text for the current position.
	*/
	virtual QString GetInfoText() const = 0;

	/**
		Set information text for the current position.
	*/
	virtual void SetInfoText(const QString& infoText) = 0;
};


} // namespace iview


#endif // !iview_IShapeStatusInfo_included


