#include "iview/CColorShema.h"


// Qt includes
#include <QtGui/QApplication>
#include <QtGui/QPalette>


namespace iview
{


CColorShema::CColorShema()
{
	for (StandardPensSet penSetEnum = IColorShema::SPS_NORMAL; penSetEnum <= IColorShema::SPS_LAST;){
    	m_pensSets[penSetEnum] = NULL;

		CreatePenSet(penSetEnum, NORMAL_SET_SIZE);

		penSetEnum = IColorShema::StandardPensSet(int(penSetEnum) + 1);
    }

	m_pens[SP_NORMAL] = QColor(0, 255, 0);
	m_pens[SP_SELECTED] = QColor(255, 255, 0);
	m_pens[SP_SELECTED].setWidthF(1.5);
	m_pens[SP_TICKER] = QColor(0, 192, 255);
	m_pens[SP_SELECTED_TICKER] = QColor(128, 0, 128);
	m_pens[SP_IMPORTANT] = QColor(255, 0, 255);
	m_pens[SP_IMPORTANT].setWidthF(1.5);
	m_pens[SP_ORIENT_DARK] = QColor(0, 0, 0);
	m_pens[SP_ORIENT_BRIGHT] = QColor(255, 255, 255);
	m_pens[SP_NORMAL_DASH] = QPen(QColor(0, 255, 255), 0, Qt::DashLine);
	m_pens[SP_SELECTED_DASH] = QPen(QColor(255, 255, 0), 0, Qt::DashLine);

	m_pens[SP_GUIDELINE1] = QColor(255, 0, 0, 80);
	m_pens[SP_GUIDELINE2] = QColor(255, 0, 0, 60);
	m_pens[SP_GUIDELINE3] = QColor(255, 0, 0, 30);

	m_pens[SP_BLACK] = QColor(0, 0, 0);
	m_pens[SP_WHITE] = QColor(255, 255, 255);
	m_pens[SP_RED] = QColor(255, 0, 0);
	m_pens[SP_GREEN] = QColor(0, 255, 0);
	m_pens[SP_BLUE] = QColor(0, 0, 255);
	m_pens[SP_CYAN] = QColor(0, 255, 255);
	m_pens[SP_YELLOW] = QColor(255, 255, 0);
	m_pens[SP_MAGENTA] = QColor(255, 0, 255);

	m_brushes[SB_NORMAL] = QBrush(QColor(128, 96, 96));
	m_brushes[SB_SELECTED] = QBrush(QColor(255, 128, 128));
	m_brushes[SB_TICKER] = QBrush(QColor(128, 128, 255));
	m_brushes[SB_SELECTED_TICKER] = QBrush(QColor(255, 128, 128));
	m_brushes[SB_IMPORTANT] = QBrush(QColor(0, 0, 255));
	m_brushes[SB_BACKGROUND] = qApp->palette().color(QPalette::Window);
	m_brushes[SB_HALF_TRANSPARENT] = QBrush(QColor(255, 0, 0, 50));
	m_brushes[SB_HALF_TRANSPARENT2] = QBrush(QColor(0, 0, 255, 50));
	m_brushes[SB_TRANSPARENT] = QBrush(QColor(0, 0, 0), Qt::NoBrush);

	m_fonts[iview::IColorShema::SF_NORMAL] = QFont("Arial");

	static const int normalSetSize = 16;
	m_pensSets[SPS_NORMAL] = new QPen[normalSetSize];
	I_ASSERT(m_pensSets[SPS_NORMAL] != NULL);
	m_pensPtrSets[SPS_NORMAL].resize(normalSetSize);
	for (int penIndex = 0; penIndex < normalSetSize; ++penIndex){
		double balance = double(penIndex) / normalSetSize;
		int red = 180 - balance * 128;
		int green = 180 - balance * 128;
		int blue = 192 - balance * 128;
		m_pensSets[SPS_NORMAL][penIndex] = QColor(red, green, blue);
		m_pensPtrSets[SPS_NORMAL][penIndex] = &m_pensSets[SPS_NORMAL][penIndex];
	}
}


CColorShema::~CColorShema()
{
	for(IColorShema::StandardPensSet penSetEnum = IColorShema::SPS_NORMAL; penSetEnum <= IColorShema::SPS_LAST;){
    	DeletePenSet(penSetEnum);
		penSetEnum = IColorShema::StandardPensSet(int(penSetEnum) + 1);
	}
}


void CColorShema::SetPensPtrSet(IColorShema::StandardPensSet pensSetType, const PensPtrSet& pensSet)
{
	I_ASSERT((int)pensSetType >= 0);
	I_ASSERT((int)pensSetType <= (int)IColorShema::SPS_LAST);

	DeletePenSet(pensSetType);
    
	int size = int(pensSet.size());
    
	CreatePenSet(pensSetType, size);
	
	for (int i = 0; i < size; i++){
    	m_pensSets[pensSetType][i] = *pensSet[i];
	}
}


void CColorShema::CreatePenSet(IColorShema::StandardPensSet pensSetType, int size)
{
	I_ASSERT((int)pensSetType >= 0);
	I_ASSERT((int)pensSetType <= (int)IColorShema::SPS_LAST);
    I_ASSERT(m_pensSets[pensSetType] == NULL);

	if (size == 0){
    	return;
	}

    m_pensSets[pensSetType] = new QPen[size];
    m_pensPtrSets[pensSetType].resize(size);

	for (int penIndex = 0; penIndex < size; ++penIndex){
        m_pensPtrSets[pensSetType][penIndex] = &m_pensSets[pensSetType][penIndex];
    }
}


void CColorShema::DeletePenSet(IColorShema::StandardPensSet pensSetType)
{
	I_ASSERT((int)pensSetType >= 0);
	I_ASSERT((int)pensSetType <= (int)IColorShema::SPS_LAST);
    
    if (m_pensSets[pensSetType] != NULL){
		delete[] m_pensSets[pensSetType];
    
		m_pensSets[pensSetType] = NULL;
	}

    m_pensPtrSets[pensSetType].clear();
}


} // namespace iview


