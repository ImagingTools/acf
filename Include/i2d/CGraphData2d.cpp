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
static const istd::IChangeable::ChangeSet s_addCurveChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Add curve to graph"));
static const istd::IChangeable::ChangeSet s_removeCurveChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Remove curve from graph"));
static const istd::IChangeable::ChangeSet s_clearCurvesChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Clear all curves"));
static const istd::IChangeable::ChangeSet s_setXAxisLabelChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set X-axis label"));
static const istd::IChangeable::ChangeSet s_setYAxisLabelChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set Y-axis label"));
static const istd::IChangeable::ChangeSet s_setTitleChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set graph title"));
static const istd::IChangeable::ChangeSet s_setXAxisRangeChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set X-axis range"));
static const istd::IChangeable::ChangeSet s_setYAxisRangeChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set Y-axis range"));
static const istd::IChangeable::ChangeSet s_setLegendVisibleChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set legend visibility"));
static const istd::IChangeable::ChangeSet s_setGridVisibleChange(IObject2d::CF_OBJECT_POSITION, istd::IChangeable::CF_ALL_DATA, QObject::tr("Set grid visibility"));


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
	if (!BaseClass::Serialize(archive)){
		return false;
	}

	// Serialize basic properties
	archive.AddString("Title", m_title);
	archive.AddString("XAxisLabel", m_xAxisLabel);
	archive.AddString("YAxisLabel", m_yAxisLabel);
	archive.AddBool("LegendVisible", m_isLegendVisible);
	archive.AddBool("GridVisible", m_isGridVisible);

	// Serialize curves count
	int curvesCount = m_curves.count();
	archive.AddInt("CurvesCount", curvesCount);

	// Serialize each curve
	for (int i = 0; i < curvesCount; ++i){
		QString curvePrefix = QString("Curve%1_").arg(i);
		const Curve& curve = m_curves[i];
		
		archive.AddString(curvePrefix + "Name", curve.name);
		archive.AddColor(curvePrefix + "Color", curve.color);
		
		int pointsCount = curve.points.count();
		archive.AddInt(curvePrefix + "PointsCount", pointsCount);
		
		for (int j = 0; j < pointsCount; ++j){
			QString pointPrefix = curvePrefix + QString("Point%1_").arg(j);
			archive.AddDouble(pointPrefix + "X", curve.points[j].GetX());
			archive.AddDouble(pointPrefix + "Y", curve.points[j].GetY());
		}
	}

	return true;
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
