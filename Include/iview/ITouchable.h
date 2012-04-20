#ifndef iview_ITouchable_included
#define iview_ITouchable_included


#include "istd/IPolymorphic.h"

#include "iview/iview.h"


namespace iview
{


/**
	This interface describes all untransparent GUI objects, which can be touched.
*/
class ITouchable: virtual public istd::IPolymorphic
{
public:

	/**
		Enumeration for possible shape touch states.
	*/
	enum TouchState
	{
		/**
			Nothing is touched.
		*/
		TS_NONE,
		
		/**
			Inactive element is touched.
		*/
		TS_INACTIVE,
		
		/**
			Ticker is touched.
		*/
		TS_TICKER,
		
		/**
			Draggable element is touched.
		*/
		TS_DRAGGABLE,
		
		/**
			Other element is touched.
		*/
		TS_OTHER,
		
		/**
			Last value in this enum.
		*/
		TS_LAST = TS_OTHER
	};

	/**
		Check, if any shape is touched.
		When shape is touched, it return also pointer to this shape.
		\return	touch state for the shape, or iview::IInteractiveShape::TS_NONE, when nothing found.
	*/
	virtual TouchState IsTouched(istd::CIndex2d position) const = 0;

	/**
		Get description to shape at specified position.
	*/
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const = 0;
};


} // namespace iview


#endif // !iview_ITouchable_included


