#include "iview/CColorShemaComp.h"


// ACF includes

#include <iostream>


namespace iview
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CColorShemaComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();	

	//pens set

	if (m_normalPenColorAttrPtr.IsValid()){
		SetPen(SP_NORMAL, QColor(*m_normalPenColorAttrPtr));		
	}	
	
	if (m_selectedPenColorAttrPtr.IsValid()){
		QPen& p = const_cast<QPen&>(GetPen(SP_SELECTED));
		p.setColor(QColor(*m_selectedPenColorAttrPtr));		
	}

	if (m_selectedPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_SELECTED));
		p.setWidthF(*m_selectedPenWidthAttrPtr);			
	}		

	if (m_selectedTickerPenColorAttrPtr.IsValid()){
		SetPen(SP_SELECTED_TICKER, QColor(*m_selectedTickerPenColorAttrPtr));
	}

	if (m_tickerPenColorAttrPtr.IsValid()){
		SetPen(SP_TICKER, QColor(*m_tickerPenColorAttrPtr));
	}

	if (m_importantPenColorAttrPtr.IsValid()){
		QPen& p = const_cast<QPen&>(GetPen(SP_IMPORTANT));
		p.setColor(QColor(*m_importantPenColorAttrPtr));		
	}

	if (m_importantPenWidthAttrPtr.IsValid()){		
		QPen& p = const_cast<QPen&>(GetPen(SP_IMPORTANT));
		p.setWidthF(*m_importantPenWidthAttrPtr);			
	}	

	if (m_orientDarkPenColorAttrPtr.IsValid()){
		SetPen(SP_ORIENT_DARK, QColor(*m_orientDarkPenColorAttrPtr));
	}

	if (m_orientBrightPenColorAttrPtr.IsValid()){
		SetPen(SP_ORIENT_BRIGHT, QColor(*m_orientBrightPenColorAttrPtr));
	}	

	// brushs set

	if (m_normalBrushColorAttrPtr.IsValid()){
		SetBrush(SB_NORMAL, QBrush(QColor(*m_normalBrushColorAttrPtr)));
	}

	if (m_selectedBrushColorAttrPtr.IsValid()){
		SetBrush(SB_SELECTED, QBrush(QColor(*m_selectedBrushColorAttrPtr)));
	}

	if (m_selectedTickerBrushColorAttrPtr.IsValid()){
		SetBrush(SB_SELECTED_TICKER, QBrush(QColor(*m_selectedTickerBrushColorAttrPtr)));
	}

	if (m_tickerBrushColorAttrPtr.IsValid()){
		SetBrush(SB_TICKER, QBrush(QColor(*m_tickerBrushColorAttrPtr)));
	}	

	if (m_importantBrushColorAttrPtr.IsValid()){
		SetBrush(SB_IMPORTANT, QBrush(QColor(*m_importantBrushColorAttrPtr)));
	}

	if (m_backgroundBrushColorAttrPtr.IsValid()){		
		SetBrush(SB_BACKGROUND, QBrush(QColor(*m_backgroundBrushColorAttrPtr)));
	}	

	if (m_halfTransparentBrushColorAttrPtr.IsValid()){
		QColor c(*m_halfTransparentBrushColorAttrPtr);
		c.setAlpha(50);
		SetBrush(SB_BACKGROUND, QBrush(c));
	}

	if (m_halfTransparent2BrushColorAttrPtr.IsValid()){
		QColor c(*m_halfTransparent2BrushColorAttrPtr);
		c.setAlpha(50);
		SetBrush(SB_BACKGROUND, QBrush(c));
	}

}



} // namespace iview



