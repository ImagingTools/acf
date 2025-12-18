#ifndef iview_CColorSchemaBase_included
#define iview_CColorSchemaBase_included


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include <iview/IColorSchema.h>


namespace iview
{


class CColorSchemaBase: public IColorSchema
{
public:
	enum
	{
		TICKER_SIZE = 4,
		SMALL_TICKER_SIZE = 2,
		LOGICAL_LINE_WIDTH = 4
	};

	// reimplemented (iview::IColorSchema)
	virtual void Assign(const IColorSchema& colorSchema) override;
	virtual void DrawTicker(QPainter& drawContext, istd::CIndex2d point, IColorSchema::TickerType tickerType) const override;
	virtual const i2d::CRect& GetTickerBox(IColorSchema::TickerType tickerType) const override;
	virtual int GetLogicalLineWidth() const override;

protected:
	// static attributes
	static i2d::CRect s_tickerDrawBoxes[(int)TT_LAST + (int)1];
	static i2d::CRect s_tickerBoundingBoxes[(int)TT_LAST + (int)1];
};


// public inline methods

// reimplemented (iview::IColorSchema)

inline int CColorSchemaBase::GetLogicalLineWidth() const
{
	return LOGICAL_LINE_WIDTH;
}


} // namespace iview


#endif // !iview_CColorSchemaBase_included



