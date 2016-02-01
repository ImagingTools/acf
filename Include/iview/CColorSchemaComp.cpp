#include "iview/CColorSchemaComp.h"


// STL includes
#include <iostream>


// Qt includes
#include <QtGui/QColor>


namespace iview
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CColorSchemaComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();	

	// pens set

	if (m_normalPenColorAttrPtr.IsValid()){
		SetPen(SP_NORMAL, QColor(QString(*m_normalPenColorAttrPtr)));
	}

	if (m_normalPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_NORMAL));
		p.setWidthF(*m_normalPenWidthAttrPtr);			
	}
	
	if (m_selectedPenColorAttrPtr.IsValid()){
		QPen& p = const_cast<QPen&>(GetPen(SP_SELECTED));
		p.setColor(QString(*m_selectedPenColorAttrPtr));		
	}

	if (m_selectedPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_SELECTED));
		p.setWidthF(*m_selectedPenWidthAttrPtr);			
	}	

	if (m_selectedTickerPenColorAttrPtr.IsValid()){
		SetPen(SP_SELECTED_TICKER, QColor(QString(*m_selectedTickerPenColorAttrPtr)));
	}

	if (m_selectedTickerPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_SELECTED_TICKER));
		p.setWidthF(*m_selectedTickerPenWidthAttrPtr);			
	}
	
	if (m_tickerPenColorAttrPtr.IsValid()){
		SetPen(SP_TICKER, QColor(QString(*m_tickerPenColorAttrPtr)));
	}

	if (m_tickerPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_TICKER));
		p.setWidthF(*m_tickerPenWidthAttrPtr);			
	}

	if (m_importantPenColorAttrPtr.IsValid()){
		QPen& p = const_cast<QPen&>(GetPen(SP_IMPORTANT));
		p.setColor(QString(*m_importantPenColorAttrPtr));		
	}

	if (m_importantPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_IMPORTANT));
		p.setWidthF(*m_importantPenWidthAttrPtr);			
	}

	if (m_orientDarkPenColorAttrPtr.IsValid()){
		SetPen(SP_ORIENT_DARK, QColor(QString(*m_orientDarkPenColorAttrPtr)));
	}

	if (m_orientBrightPenColorAttrPtr.IsValid()){
		SetPen(SP_ORIENT_BRIGHT, QColor(QString(*m_orientBrightPenColorAttrPtr)));
	}

	// brushes set

	if (m_normalBrushColorAttrPtr.IsValid()){
		SetBrush(SB_NORMAL, QBrush(QColor(QString(*m_normalBrushColorAttrPtr))));
	}

	if (m_selectedBrushColorAttrPtr.IsValid()){
		SetBrush(SB_SELECTED, QBrush(QColor(QString(*m_selectedBrushColorAttrPtr))));
	}

	if (m_selectedTickerBrushColorAttrPtr.IsValid()){
		SetBrush(SB_SELECTED_TICKER, QBrush(QColor(QString(*m_selectedTickerBrushColorAttrPtr))));
	}

	if (m_tickerBrushColorAttrPtr.IsValid()){
		SetBrush(SB_TICKER, QBrush(QColor(QString(*m_tickerBrushColorAttrPtr))));
	}

	if (m_importantBrushColorAttrPtr.IsValid()){
		SetBrush(SB_IMPORTANT, QBrush(QColor(QString(*m_importantBrushColorAttrPtr))));
	}

	if (m_backgroundBrushColorAttrPtr.IsValid()){		
		SetBrush(SB_BACKGROUND, QBrush(QColor(QString(*m_backgroundBrushColorAttrPtr))));
	}

	if (m_halfTransparentBrushColorAttrPtr.IsValid()){
		QColor c;
		c.setNamedColor(QString(*m_halfTransparentBrushColorAttrPtr));
		c.setAlpha(50);
		SetBrush(SB_BACKGROUND, QBrush(c));
	}

	if (m_halfTransparent2BrushColorAttrPtr.IsValid()){
		QColor c;
		c.setNamedColor(QString(*m_halfTransparent2BrushColorAttrPtr));
		c.setAlpha(50);
		SetBrush(SB_BACKGROUND, QBrush(c));
	}

	if (m_normalFontFamilyAttrPtr.IsValid()){
		m_fonts[SF_NORMAL].setFamily(*m_normalFontFamilyAttrPtr);
	}

	if (m_normalFontSizeAttrPtr.IsValid()){
		m_fonts[SF_NORMAL].setPointSize(*m_normalFontSizeAttrPtr);
	}
	else{
		m_fonts[SF_NORMAL].setPointSizeF(m_pens[SP_NORMAL].widthF() * 5);
	}

	if (m_normalFontWeightAttrPtr.IsValid()){
		m_fonts[SF_NORMAL].setWeight(*m_normalFontWeightAttrPtr);
	}
}


} // namespace iview


