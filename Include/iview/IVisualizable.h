#ifndef iview_IVisualisable_included
#define iview_IVisualisable_included


#include <istd/IPolymorphic.h>

#include <iview/iview.h>


class QPainter;


namespace iview
{


class IDisplay;


class IVisualizable: virtual public istd::IPolymorphic
{
public:
	/**
		Check, if this object is visible.
	*/
	virtual bool IsVisible() const = 0;
	
	/**
		Check if this object can be connected to this display.
	*/
	virtual bool IsDisplayAccepted(const IDisplay& display) const = 0;
	
	/**
		Called after display is connected.
		It can be called only, when display is accepted (\sa IsDisplayAccepted()).
		\param	displayPtr	an observer object, which should be informed about all shape changes.
	*/
	virtual void OnConnectDisplay(IDisplay* displayPtr) = 0;
	
	/**
		Called after display is disconnected.
		Note, that this display must be previously connected.
		\param	displayPtr	a disconnected observer object.
	*/
	virtual void OnDisconnectDisplay(IDisplay* displayPtr) = 0;
	
	/**	
		Draw this shape using draw context.
	*/
	virtual void Draw(QPainter& drawContext) const = 0;

	/**
		It informs about changes in display.
		It is called e.g. after view transformation change.
		\return	true, if this change follow shape changes.
	*/
	virtual bool OnDisplayChange(const istd::IChangeable::ChangeSet& changeSet) = 0;
};


} // namespace iview


#endif // !iview_IVisualisable_included


