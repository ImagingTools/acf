#ifndef iview_CColorSchema_included
#define iview_CColorSchema_included


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include <iview/CColorSchemaBase.h>


namespace iview
{


/**
	Implementation of the color schema object.
	This class extends class iview::CColorSchemaBase with implementation of
	storage of pens, brushes and fonts.
*/
class CColorSchema: public CColorSchemaBase
{
public:
	enum
	{
		/**
			Standard size of pens set.
		*/
		NORMAL_SET_SIZE = 16
	};

	CColorSchema();

	// reimplemented IColorSchema
	virtual const QFont& GetFont(IColorSchema::StandardFont fontType) const override;
	virtual void SetFont(iview::IColorSchema::StandardFont fontType, const QFont& font) override;
	virtual const QPen& GetPen(IColorSchema::StandardPen penType) const override;
	virtual void SetPen(iview::IColorSchema::StandardPen penType, const QPen& pen) override;
	virtual const QBrush& GetBrush(IColorSchema::StandardBrush brushType) const override;
	virtual void SetBrush(iview::IColorSchema::StandardBrush brushType, const QBrush& brush) override;

protected:
	QPen m_pens[int(IColorSchema::SP_LAST) + 1];
	QBrush m_brushes[int(IColorSchema::SB_LAST) + 1];
	QFont m_fonts[int(IColorSchema::SF_LAST) + 1];
};



inline void CColorSchema::SetPen(IColorSchema::StandardPen penType, const QPen& pen)
{
	m_pens[penType] = pen;
}


inline void CColorSchema::SetBrush(IColorSchema::StandardBrush brushType, const QBrush& brush)
{
	m_brushes[brushType] = brush;
}


inline void CColorSchema::SetFont(IColorSchema::StandardFont fontType, const QFont& font)
{
	m_fonts[fontType] = font;
}


inline const QFont& CColorSchema::GetFont(IColorSchema::StandardFont fontType) const
{
	return m_fonts[fontType];
}


inline const QPen& CColorSchema::GetPen(IColorSchema::StandardPen penType) const
{
	return m_pens[penType];
}


inline const QBrush& CColorSchema::GetBrush(IColorSchema::StandardBrush brushType) const
{
	return m_brushes[brushType];
}


} // namespace iview


#endif // !iview_CColorSchema_included


