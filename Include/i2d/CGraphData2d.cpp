#include <i2d/CGraphData2d.h>


// Qt includes
#include <QtCore/QObject>
#include <QtCore/qmath.h>
#include <limits>

// ACF includes
#include <istd/CClassInfo.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <i2d/ITransformation2d.h>


namespace i2d
{


// static constants
static const istd::IChangeable::ChangeSet s_addCurveChange(CGraphData2d::CF_GRAPH_DATA, istd::IChangeable::CF_ALL_DATA, QObject::tr("Add curve to graph"));
static const istd::IChangeable::ChangeSet s_removeCurveChange(CGraphData2d::CF_GRAPH_DATA, istd::IChangeable::CF_ALL_DATA, QObject::tr("Remove curve from graph"));
static const istd::IChangeable::ChangeSet s_clearCurvesChange(CGraphData2d::CF_GRAPH_DATA, istd::IChangeable::CF_ALL_DATA, QObject::tr("Clear all curves"));
static const istd::IChangeable::ChangeSet s_setXAxisLabelChange(CGraphData2d::CF_GRAPH_APPEARANCE, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set X-axis label"));
static const istd::IChangeable::ChangeSet s_setYAxisLabelChange(CGraphData2d::CF_GRAPH_APPEARANCE, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set Y-axis label"));
static const istd::IChangeable::ChangeSet s_setTitleChange(CGraphData2d::CF_GRAPH_APPEARANCE, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set graph title"));
static const istd::IChangeable::ChangeSet s_setXAxisRangeChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set X-axis range"));
static const istd::IChangeable::ChangeSet s_setYAxisRangeChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set Y-axis range"));
static const istd::IChangeable::ChangeSet s_setLegendVisibleChange(CGraphData2d::CF_GRAPH_DISPLAY_OPTIONS, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set legend visibility"));
static const istd::IChangeable::ChangeSet s_setGridVisibleChange(CGraphData2d::CF_GRAPH_DISPLAY_OPTIONS, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set grid visibility"));

// Archive tags for serialization
static const iser::CArchiveTag s_titleTag("Title", "Graph title", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_xAxisLabelTag("XAxisLabel", "X-axis label", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_yAxisLabelTag("YAxisLabel", "Y-axis label", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_legendVisibleTag("LegendVisible", "Legend visibility flag", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_gridVisibleTag("GridVisible", "Grid visibility flag", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_curvesTag("Curves", "Curves collection", iser::CArchiveTag::TT_NODE);
static const iser::CArchiveTag s_curveTag("Curve", "Curve data", iser::CArchiveTag::TT_NODE);
static const iser::CArchiveTag s_curveNameTag("Name", "Curve name", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_curveColorTag("Color", "Curve color", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_pointsTag("Points", "Points collection", iser::CArchiveTag::TT_NODE);
static const iser::CArchiveTag s_pointTag("Point", "Point data", iser::CArchiveTag::TT_NODE);
static const iser::CArchiveTag s_pointXTag("X", "Point X coordinate", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_pointYTag("Y", "Point Y coordinate", iser::CArchiveTag::TT_LEAF);


// public static methods

QByteArray CGraphData2d::GetTypeName()
{
	return istd::CClassInfo::GetName<CGraphData2d>();
}


// public methods

CGraphData2d::CGraphData2d()
:	m_isLegendVisible(true),
	m_isGridVisible(true)
{
}


void CGraphData2d::AddCurve(const Curve& curve)
{
	istd::CChangeNotifier notifier(*this, s_addCurveChange);
	m_curves.append(curve);
}


void CGraphData2d::RemoveCurve(int index)
{
	if (index >= 0 && index < m_curves.count()){
		istd::CChangeNotifier notifier(*this, s_removeCurveChange);
		m_curves.remove(index);
	}
}


void CGraphData2d::ClearCurves()
{
	if (!m_curves.isEmpty()){
		istd::CChangeNotifier notifier(*this, s_clearCurvesChange);
		m_curves.clear();
	}
}


void CGraphData2d::SetXAxisLabel(const QString& label)
{
	if (m_xAxisLabel != label){
		istd::CChangeNotifier notifier(*this, s_setXAxisLabelChange);
		m_xAxisLabel = label;
	}
}


void CGraphData2d::SetYAxisLabel(const QString& label)
{
	if (m_yAxisLabel != label){
		istd::CChangeNotifier notifier(*this, s_setYAxisLabelChange);
		m_yAxisLabel = label;
	}
}


void CGraphData2d::SetTitle(const QString& title)
{
	if (m_title != title){
		istd::CChangeNotifier notifier(*this, s_setTitleChange);
		m_title = title;
	}
}


istd::CRange CGraphData2d::GetXAxisRange() const
{
	if (m_xAxisRange.IsValid()){
		return m_xAxisRange;
	}
	
	istd::CRange xRange, yRange;
	CalculateAutoRanges(xRange, yRange);
	return xRange;
}


void CGraphData2d::SetXAxisRange(const istd::CRange& range)
{
	if (m_xAxisRange != range){
		istd::CChangeNotifier notifier(*this, s_setXAxisRangeChange);
		m_xAxisRange = range;
	}
}


istd::CRange CGraphData2d::GetYAxisRange() const
{
	if (m_yAxisRange.IsValid()){
		return m_yAxisRange;
	}
	
	istd::CRange xRange, yRange;
	CalculateAutoRanges(xRange, yRange);
	return yRange;
}


void CGraphData2d::SetYAxisRange(const istd::CRange& range)
{
	if (m_yAxisRange != range){
		istd::CChangeNotifier notifier(*this, s_setYAxisRangeChange);
		m_yAxisRange = range;
	}
}


void CGraphData2d::SetLegendVisible(bool visible)
{
	if (m_isLegendVisible != visible){
		istd::CChangeNotifier notifier(*this, s_setLegendVisibleChange);
		m_isLegendVisible = visible;
	}
}


void CGraphData2d::SetGridVisible(bool visible)
{
	if (m_isGridVisible != visible){
		istd::CChangeNotifier notifier(*this, s_setGridVisibleChange);
		m_isGridVisible = visible;
	}
}


// reimplemented (i2d::IObject2d)

CVector2d CGraphData2d::GetCenter() const
{
	CRectangle bbox = GetBoundingBox();
	if (bbox.IsValid()){
		return bbox.GetCenter();
	}
	return CVector2d(0.0, 0.0);
}


void CGraphData2d::MoveCenterTo(const CVector2d& position)
{
	// Graph data is typically not moved in space
	// This is a no-op for graph data
}


CRectangle CGraphData2d::GetBoundingBox() const
{
	istd::CRange xRange = GetXAxisRange();
	istd::CRange yRange = GetYAxisRange();
	
	if (xRange.IsValid() && yRange.IsValid()){
		return CRectangle(xRange, yRange);
	}
	
	return CRectangle();
}


bool CGraphData2d::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	// Transforming graph data is not typically supported
	return false;
}


bool CGraphData2d::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	// Transforming graph data is not typically supported
	return false;
}


bool CGraphData2d::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	// Transforming graph data is not typically supported
	return false;
}


bool CGraphData2d::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	// Transforming graph data is not typically supported
	return false;
}


// reimplemented (iser::IObject)

QByteArray CGraphData2d::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

bool CGraphData2d::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	if (!BaseClass::Serialize(archive)){
		return false;
	}

	// Serialize basic properties
	bool retVal = true;
	
	retVal = retVal && archive.BeginTag(s_titleTag);
	retVal = retVal && archive.Process(m_title);
	retVal = retVal && archive.EndTag(s_titleTag);
	
	retVal = retVal && archive.BeginTag(s_xAxisLabelTag);
	retVal = retVal && archive.Process(m_xAxisLabel);
	retVal = retVal && archive.EndTag(s_xAxisLabelTag);
	
	retVal = retVal && archive.BeginTag(s_yAxisLabelTag);
	retVal = retVal && archive.Process(m_yAxisLabel);
	retVal = retVal && archive.EndTag(s_yAxisLabelTag);
	
	retVal = retVal && archive.BeginTag(s_legendVisibleTag);
	retVal = retVal && archive.Process(m_isLegendVisible);
	retVal = retVal && archive.EndTag(s_legendVisibleTag);
	
	retVal = retVal && archive.BeginTag(s_gridVisibleTag);
	retVal = retVal && archive.Process(m_isGridVisible);
	retVal = retVal && archive.EndTag(s_gridVisibleTag);

	// Serialize curves using BeginMultiTag
	int curvesCount = m_curves.count();
	retVal = retVal && archive.BeginMultiTag(s_curvesTag, s_curveTag, curvesCount);
	
	// Adjust curves vector size when loading
	if (!archive.IsStoring() && retVal){
		m_curves.resize(curvesCount);
	}

	// Serialize each curve
	for (int i = 0; i < curvesCount; ++i){
		retVal = retVal && SerializeCurve(archive, m_curves[i]);
	}
	
	retVal = retVal && archive.EndTag(s_curvesTag);

	return retVal;
}


bool CGraphData2d::SerializeCurve(iser::IArchive& archive, Curve& curve)
{
	bool retVal = archive.BeginTag(s_curveTag);
	
	retVal = retVal && archive.BeginTag(s_curveNameTag);
	retVal = retVal && archive.Process(curve.name);
	retVal = retVal && archive.EndTag(s_curveNameTag);
	
	retVal = retVal && archive.BeginTag(s_curveColorTag);
	retVal = retVal && archive.Process(curve.color);
	retVal = retVal && archive.EndTag(s_curveColorTag);
	
	// Serialize points using BeginMultiTag
	int pointsCount = curve.points.count();
	retVal = retVal && archive.BeginMultiTag(s_pointsTag, s_pointTag, pointsCount);
	
	// Adjust points vector size when loading
	if (!archive.IsStoring() && retVal){
		curve.points.resize(pointsCount);
	}
	
	for (int j = 0; j < pointsCount; ++j){
		retVal = retVal && archive.BeginTag(s_pointTag);
		
		double x = curve.points[j].GetX();
		double y = curve.points[j].GetY();
		
		retVal = retVal && archive.BeginTag(s_pointXTag);
		retVal = retVal && archive.Process(x);
		retVal = retVal && archive.EndTag(s_pointXTag);
		
		retVal = retVal && archive.BeginTag(s_pointYTag);
		retVal = retVal && archive.Process(y);
		retVal = retVal && archive.EndTag(s_pointYTag);
		
		retVal = retVal && archive.EndTag(s_pointTag);
		
		if (!archive.IsStoring()){
			curve.points[j] = CVector2d(x, y);
		}
	}
	
	retVal = retVal && archive.EndTag(s_pointsTag);
	retVal = retVal && archive.EndTag(s_curveTag);
	
	return retVal;
}


// reimplemented (istd::IChangeable)

int CGraphData2d::GetSupportedOperations() const
{
	return BaseClass::GetSupportedOperations();
}


bool CGraphData2d::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	if (!BaseClass::CopyFrom(object, mode)){
		return false;
	}

	const CGraphData2d* graphPtr = dynamic_cast<const CGraphData2d*>(&object);
	if (graphPtr != NULL){
		m_curves = graphPtr->m_curves;
		m_xAxisLabel = graphPtr->m_xAxisLabel;
		m_yAxisLabel = graphPtr->m_yAxisLabel;
		m_title = graphPtr->m_title;
		m_xAxisRange = graphPtr->m_xAxisRange;
		m_yAxisRange = graphPtr->m_yAxisRange;
		m_isLegendVisible = graphPtr->m_isLegendVisible;
		m_isGridVisible = graphPtr->m_isGridVisible;
		return true;
	}

	return false;
}


istd::TUniqueInterfacePtr<istd::IChangeable> CGraphData2d::CloneMe(CompatibilityMode mode) const
{
	istd::TUniqueInterfacePtr<CGraphData2d> resultPtr(new CGraphData2d());
	if (resultPtr->CopyFrom(*this, mode)){
		return istd::TUniqueInterfacePtr<istd::IChangeable>(resultPtr.Release());
	}
	return istd::TUniqueInterfacePtr<istd::IChangeable>();
}


// private methods

void CGraphData2d::CalculateAutoRanges(istd::CRange& xRange, istd::CRange& yRange) const
{
	double minX = std::numeric_limits<double>::max();
	double maxX = std::numeric_limits<double>::lowest();
	double minY = std::numeric_limits<double>::max();
	double maxY = std::numeric_limits<double>::lowest();
	
	bool hasData = false;
	
	for (int i = 0; i < m_curves.count(); ++i){
		const Curve& curve = m_curves[i];
		for (int j = 0; j < curve.points.count(); ++j){
			const CVector2d& point = curve.points[j];
			minX = qMin(minX, point.GetX());
			maxX = qMax(maxX, point.GetX());
			minY = qMin(minY, point.GetY());
			maxY = qMax(maxY, point.GetY());
			hasData = true;
		}
	}
	
	if (hasData){
		// Add some padding (5%)
		double xPadding = (maxX - minX) * 0.05;
		double yPadding = (maxY - minY) * 0.05;
		
		if (xPadding == 0.0) xPadding = 1.0;
		if (yPadding == 0.0) yPadding = 1.0;
		
		xRange = istd::CRange(minX - xPadding, maxX + xPadding);
		yRange = istd::CRange(minY - yPadding, maxY + yPadding);
	}
	else{
		// Default ranges when no data
		xRange = istd::CRange(0.0, 10.0);
		yRange = istd::CRange(0.0, 10.0);
	}
}


} // namespace i2d
