#ifndef iview_CColorSchemaComp_included
#define iview_CColorSchemaComp_included


// QT includes
#include <QtCore/QByteArray>
#include <QtCore/QString>

// ACF includes
#include <icomp/CComponentBase.h>
#include <iview/CColorSchema.h>


namespace iview
{

class CColorSchemaPenAttr: public icomp::CComponentBase, public CColorSchema
{
public:
	typedef icomp::CComponentBase BaseClass;	

	I_BEGIN_COMPONENT(CColorSchemaPenAttr);
		I_REGISTER_INTERFACE(IColorSchema);		
		I_ASSIGN(m_normalPenColorAttrPtr, "NormalPenColor", "Normal pen color", false, "green");
		I_ASSIGN(m_normalPenWidthAttrPtr, "NormalPenWidth", "Normal pen width", false, 1.5);
		I_ASSIGN(m_selectedPenColorAttrPtr, "SelectedPenColor", "Selected pen color", false, "yellow");
		I_ASSIGN(m_selectedPenWidthAttrPtr, "SelectedPenWidth", "Selected pen width", false, 1.5);
		I_ASSIGN(m_selectedTickerPenColorAttrPtr, "SelectedTickerPenColor", "Selected ticker pen color", false, "purple");
		I_ASSIGN(m_selectedTickerPenWidthAttrPtr, "SelectedTickerPenWidth", "Selected ticker pen width", false, 1);
		I_ASSIGN(m_tickerPenColorAttrPtr, "TickerPenColor", "Ticker pen color", false, "deepskyblue");
		I_ASSIGN(m_tickerPenWidthAttrPtr, "TickerPenWidth", "Ticker pen width", false, 1);
		I_ASSIGN(m_importantPenColorAttrPtr, "ImportantPenColor", "Important pen color", false, "fuchsia");
		I_ASSIGN(m_importantPenWidthAttrPtr, "ImportantPenWidth", "Important pen width", false, 1.5);
		I_ASSIGN(m_orientDarkPenColorAttrPtr, "OrientDarkPenColor", "OrientDark pen color", false, "black");
		I_ASSIGN(m_orientBrightPenColorAttrPtr, "OrientBrightPenColor", "OrientBright pen color", false, "white");
	I_END_COMPONENT;

protected:
	I_ATTR(QByteArray, m_normalPenColorAttrPtr);
	I_ATTR(double, m_normalPenWidthAttrPtr);
	I_ATTR(QByteArray, m_selectedPenColorAttrPtr);
	I_ATTR(double, m_selectedPenWidthAttrPtr);
	I_ATTR(QByteArray, m_selectedTickerPenColorAttrPtr);
	I_ATTR(double, m_selectedTickerPenWidthAttrPtr);
	I_ATTR(QByteArray, m_tickerPenColorAttrPtr);
	I_ATTR(double, m_tickerPenWidthAttrPtr);
	I_ATTR(QByteArray, m_importantPenColorAttrPtr);
	I_ATTR(double, m_importantPenWidthAttrPtr);
	I_ATTR(QByteArray, m_orientDarkPenColorAttrPtr);
	I_ATTR(QByteArray, m_orientBrightPenColorAttrPtr);
};


class CColorSchemaBrushAttr: public CColorSchemaPenAttr
{
public:
	typedef CColorSchemaPenAttr BaseClass;	

	I_BEGIN_COMPONENT(CColorSchemaBrushAttr);
		I_ASSIGN(m_normalBrushColorAttrPtr, "NormalBrushColor", "Normal brush color", false, "rosybrown");
		I_ASSIGN(m_selectedBrushColorAttrPtr, "SelectedBrushColor", "Selected brush color", false, "lightpink");	
		I_ASSIGN(m_selectedTickerBrushColorAttrPtr, "SelectedTickerBrushColor", "Selected ticker brush color", false, "lightcoral");
		I_ASSIGN(m_tickerBrushColorAttrPtr, "TickerBrushColor", "Ticker brush color", false, "mediumslateblue");
		I_ASSIGN(m_importantBrushColorAttrPtr, "ImportantBrushColor", "Important brush color", false, "blue");
	I_END_COMPONENT;

protected:
	I_ATTR(QByteArray, m_normalBrushColorAttrPtr);
	I_ATTR(QByteArray, m_selectedBrushColorAttrPtr);
	I_ATTR(QByteArray, m_selectedTickerBrushColorAttrPtr);
	I_ATTR(QByteArray, m_tickerBrushColorAttrPtr);
	I_ATTR(QByteArray, m_importantBrushColorAttrPtr);	
};


/**
	Implementation of a ColorSchema as a component.
*/
class CColorSchemaComp: public CColorSchemaBrushAttr
{
public:
	typedef CColorSchemaBrushAttr BaseClass;	

	I_BEGIN_COMPONENT(CColorSchemaComp);	
		I_ASSIGN(m_halfTransparentBrushColorAttrPtr, "HalfTransparentBrushColor", "Half-Transparent brush color", false, "mediumslateblue");
		I_ASSIGN(m_halfTransparent2BrushColorAttrPtr, "HalfTransparent2BrushColor", "Half-Transparent2 brush color", false, "mediumslateblue");
		I_ASSIGN(m_backgroundBrushColorAttrPtr, "BackgroundBrushColor", "Background brush color", false, "AppColor");
		I_ASSIGN(m_normalFontFamilyAttrPtr, "NormalFontFamily", "Name of family of normal font", false, "Helvetica");
		I_ASSIGN(m_normalFontSizeAttrPtr, "NormalFontSize", "Size of normal font", false, 10);
		I_ASSIGN(m_normalFontWeightAttrPtr, "NormalFontWeight", "Size of normal font", false, 50);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QByteArray, m_halfTransparentBrushColorAttrPtr);
	I_ATTR(QByteArray, m_halfTransparent2BrushColorAttrPtr);
	I_ATTR(QByteArray, m_backgroundBrushColorAttrPtr);
	I_ATTR(QString, m_normalFontFamilyAttrPtr);
	I_ATTR(int, m_normalFontSizeAttrPtr);
	I_ATTR(int, m_normalFontWeightAttrPtr);
};


} // namespace iview


#endif // !iview_CColorSchemaComp_included


