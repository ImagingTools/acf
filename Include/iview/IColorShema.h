#ifndef iview_IColorShema_included
#define iview_IColorShema_included


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>

// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CIndex2d.h"
#include "i2d/CRect.h"


class QPainter;


namespace iview
{



/**
	Defines set of standard pens, brushes and simple management of unions.
*/
class IColorShema: virtual public istd::IPolymorphic
{
public:
    enum StandardPen{
		SP_NORMAL,
		SP_SELECTED,
		SP_SELECTED_TICKER,
		SP_TICKER,
		SP_IMPORTANT,
		SP_ORIENT_DARK,
		SP_ORIENT_BRIGHT,
        SP_NORMAL_DASH,
		SP_SELECTED_DASH,
        SP_GUIDELINE1,
        SP_GUIDELINE2,
        SP_GUIDELINE3,
		SP_BLACK,
		SP_WHITE,
		SP_RED,
		SP_GREEN,
		SP_BLUE,
		SP_CYAN,
		SP_YELLOW,
		SP_MAGENTA,
		SP_LAST = SP_MAGENTA
	};

	enum StandardBrush{
		SB_NORMAL,
		SB_SELECTED,
		SB_TICKER,
		SB_SELECTED_TICKER,
		SB_IMPORTANT,
		SB_BACKGROUND,
		SB_HALF_TRANSPARENT,
		SB_HALF_TRANSPARENT2,
        SB_TRANSPARENT,
		SB_LAST = SB_TRANSPARENT
	};

    enum StandardFont{
		SF_NORMAL = 0,
		SF_LAST = SF_NORMAL
	};

	enum StandardPensSet{
		SPS_NORMAL = 0,
		SPS_LAST = SPS_NORMAL
	};

	/**	Defines a type of ticker.
	*/
	enum TickerType{
		/**	Small ticker.
		*/
		TT_SMALL,
		/**	Inactive small ticker.
		*/
		TT_INACTIVE_SMALL,
		/**	Selected inactive small ticker.
		*/
		TT_SELECTED_INACTIVE_SMALL,
		/**	Normal (default active) ticker.
		*/
		TT_NORMAL,
		/**	Inactive ticker.
		*/
		TT_INACTIVE,
		/**	Selected inactive ticker.
		*/
		TT_SELECTED_INACTIVE,
		/**	Ticker used to move object.
		*/
		TT_MOVE,
		/** Ticker used to move center pins
		*/
		TT_MOVE_CENTER,
		/**	Ticker used to add node.
		*/
		TT_INSERT,
		/**	Ticker used to delete node.
		*/
		TT_DELETE,
		/**	Ticker used to rotate object.
		*/
		TT_ROTATE,
		/**	Ticker used to skew object.
		*/
		TT_SKEW,
		/**	Ticker used to checkbox, when it is on.
		*/
		TT_CHECKBOX_ON,
		/**	Ticker used to checkbox, when it is off.
		*/
		TT_CHECKBOX_OFF,
		/**	Max value.
		*/
		TT_LAST = TT_CHECKBOX_OFF
	};

	typedef QVector<const QPen*> PensPtrSet;

    virtual void Assign(const IColorShema& colorShema) = 0;
    virtual const QPen& GetPen(StandardPen penType) const = 0;
    virtual void SetPen(StandardPen penType, const QPen& pen) = 0;
	virtual const QBrush& GetBrush(StandardBrush brushType) const = 0;
	virtual void SetBrush(StandardBrush brushType, const QBrush& brush) = 0;
    virtual const QFont& GetFont(StandardFont fontType) const = 0;
    virtual void SetFont(StandardFont fontType, const QFont& font) = 0;
	virtual const PensPtrSet& GetPensPtrSet(StandardPensSet pensSetType) const = 0;
    virtual void SetPensPtrSet(StandardPensSet pensSetType, const PensPtrSet& pensSet) = 0;
   
	/**
		Draw a ticker.
	*/
	virtual void DrawTicker(QPainter& drawContext, istd::CIndex2d point, TickerType tickerType) const = 0;
	
	/**
		Get bounding box of ticker in position (0, 0).
	*/
	virtual const i2d::CRect& GetTickerBox(TickerType tickerType) const = 0;
	
	/**
		Return width of line.
		This value is used to check, if user touched a line.
		It doesn't depend on screen transform.
	*/
	virtual int GetLogicalLineWidth() const = 0;
};


inline iview::IColorShema::TickerType operator|(iview::IColorShema::TickerType tickerType1, iview::IColorShema::TickerType tickerType2)
{
	return (iview::IColorShema::TickerType)(int(tickerType1) | int(tickerType2));
}


} // namespace iview


#endif // !iview_IColorShema_included


