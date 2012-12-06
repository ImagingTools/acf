#ifndef iview_CColorShemaComp_included
#define iview_CColorShemaComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iview/CColorShema.h"


namespace iview
{


class CColorShemaCompAttr: public icomp::CComponentBase, public CColorShema
{
public:
	typedef icomp::CComponentBase BaseClass;	

	I_BEGIN_COMPONENT(CColorShemaCompAttr);
		I_REGISTER_INTERFACE(IColorShema);		
		I_ASSIGN(m_normalPenColorAttrPtr, "NormalPenColor", "Normal pen color", false, "green");
		I_ASSIGN(m_selectedPenColorAttrPtr, "SelectedPenColor", "Selected pen color", false, "yellow");
		I_ASSIGN(m_selectedPenWidthAttrPtr, "SelectedPenWidth", "Selected pen width", false, 1.5);
		I_ASSIGN(m_selectedTickerPenColorAttrPtr, "SelectedTickerPenColor", "Selected ticker pen color", false, "purple");
		I_ASSIGN(m_tickerPenColorAttrPtr, "TickerPenColor", "Ticker pen color", false, "deepskyblue");
		I_ASSIGN(m_importantPenColorAttrPtr, "ImportantPenColor", "Important pen color", false, "fuchsia");
		I_ASSIGN(m_importantPenWidthAttrPtr, "ImportantPenWidth", "Important pen width", false, 1.5);
		I_ASSIGN(m_orientDarkPenColorAttrPtr, "OrientDarkPenColor", "OrientDark pen color", false, "black");
		I_ASSIGN(m_orientBrightPenColorAttrPtr, "OrientBrightPenColor", "OrientBright pen color", false, "white");

		I_ASSIGN(m_normalBrushColorAttrPtr, "NormalBrushColor", "Normal brush color", false, "rosybrown");
		I_ASSIGN(m_selectedBrushColorAttrPtr, "SelectedBrushColor", "Selected brush color", false, "lightpink");	
		I_ASSIGN(m_selectedTickerBrushColorAttrPtr, "SelectedTickerBrushColor", "Selected ticker brush color", false, "lightcoral");
		I_ASSIGN(m_tickerBrushColorAttrPtr, "TickerBrushColor", "Ticker brush color", false, "mediumslateblue");
		I_ASSIGN(m_importantBrushColorAttrPtr, "ImportantBrushColor", "Important brush color", false, "blue");
	I_END_COMPONENT;
	
	
protected:

	I_ATTR(QString, m_normalPenColorAttrPtr);
	I_ATTR(QString, m_selectedPenColorAttrPtr);
	I_ATTR(double, m_selectedPenWidthAttrPtr);
	I_ATTR(QString, m_selectedTickerPenColorAttrPtr);
	I_ATTR(QString, m_tickerPenColorAttrPtr);
	I_ATTR(QString, m_importantPenColorAttrPtr);
	I_ATTR(double, m_importantPenWidthAttrPtr);
	I_ATTR(QString, m_orientDarkPenColorAttrPtr);
	I_ATTR(QString, m_orientBrightPenColorAttrPtr);

	I_ATTR(QString, m_normalBrushColorAttrPtr);
	I_ATTR(QString, m_selectedBrushColorAttrPtr);
	I_ATTR(QString, m_selectedTickerBrushColorAttrPtr);
	I_ATTR(QString, m_tickerBrushColorAttrPtr);
	I_ATTR(QString, m_importantBrushColorAttrPtr);	
	
};

/**
	Implementation of a ColorShema as a component.
*/

class CColorShemaComp: public CColorShemaCompAttr
{
public:
	typedef CColorShemaCompAttr BaseClass;	

	I_BEGIN_COMPONENT(CColorShemaComp);	
		I_ASSIGN(m_halfTransparentBrushColorAttrPtr, "HalfTransparentBrushColor", "Half-Transparent brush color", false, "mediumslateblue");
		I_ASSIGN(m_halfTransparent2BrushColorAttrPtr, "HalfTransparent2BrushColor", "Half-Transparent2 brush color", false, "mediumslateblue");
		I_ASSIGN(m_backgroundBrushColorAttrPtr, "BackgroundBrushColor", "Background brush color", false, "AppColor");
	I_END_COMPONENT;
	
	
protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:	

	I_ATTR(QString, m_halfTransparentBrushColorAttrPtr);
	I_ATTR(QString, m_halfTransparent2BrushColorAttrPtr);
	I_ATTR(QString, m_backgroundBrushColorAttrPtr);
	
};


} // namespace iview


#endif // !iview_CColorShemaComp_included


