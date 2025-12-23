#ifndef i2d_CGraphData2d_included
#define i2d_CGraphData2d_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtGui/QColor>

// ACF includes
#include <i2d/CObject2dBase.h>
#include <i2d/CVector2d.h>
#include <i2d/CRectangle.h>


namespace i2d
{


/**
	Data model for a 2D graph plot supporting multiple curves.
	Each curve is a series of 2D points with associated properties like name and color.
	The graph also maintains axis ranges, labels, and legend information.
*/
class CGraphData2d: public CObject2dBase
{
public:
	enum ChangeFlags
	{
		CF_GRAPH_DATA = 0x7c95a1,		///< Graph data (curves, points) changed
		CF_GRAPH_APPEARANCE,			///< Graph appearance (colors, names, labels) changed
		CF_GRAPH_DISPLAY_OPTIONS		///< Display options (legend, grid visibility) changed
	};

	/**
		Structure representing a single curve in the graph.
	*/
	struct Curve
	{
		QString name;					///< Name of the curve (for legend)
		QVector<CVector2d> points;		///< Points defining the curve
		QColor color;					///< Color for drawing the curve
		
		Curve() : color(Qt::blue) {}
	};

	static QByteArray GetTypeName();

	CGraphData2d();

	/**
		Get the number of curves in the graph.
	*/
	int GetCurvesCount() const;

	/**
		Get a curve by index.
	*/
	const Curve& GetCurve(int index) const;

	/**
		Get reference to a curve by index.
		Use with care and trigger notifications manually if needed.
	*/
	Curve& GetCurveRef(int index);

	/**
		Add a new curve to the graph.
	*/
	void AddCurve(const Curve& curve);

	/**
		Remove a curve by index.
	*/
	void RemoveCurve(int index);

	/**
		Clear all curves.
	*/
	void ClearCurves();

	/**
		Get the X-axis label.
	*/
	const QString& GetXAxisLabel() const;

	/**
		Set the X-axis label.
	*/
	void SetXAxisLabel(const QString& label);

	/**
		Get the Y-axis label.
	*/
	const QString& GetYAxisLabel() const;

	/**
		Set the Y-axis label.
	*/
	void SetYAxisLabel(const QString& label);

	/**
		Get the graph title.
	*/
	const QString& GetTitle() const;

	/**
		Set the graph title.
	*/
	void SetTitle(const QString& title);

	/**
		Get X-axis range. If not set, returns auto-calculated range.
	*/
	istd::CRange GetXAxisRange() const;

	/**
		Set X-axis range. Set to invalid range for auto-calculation.
	*/
	void SetXAxisRange(const istd::CRange& range);

	/**
		Get Y-axis range. If not set, returns auto-calculated range.
	*/
	istd::CRange GetYAxisRange() const;

	/**
		Set Y-axis range. Set to invalid range for auto-calculation.
	*/
	void SetYAxisRange(const istd::CRange& range);

	/**
		Check if legend should be shown.
	*/
	bool IsLegendVisible() const;

	/**
		Set whether legend should be shown.
	*/
	void SetLegendVisible(bool visible = true);

	/**
		Check if grid should be shown.
	*/
	bool IsGridVisible() const;

	/**
		Set whether grid should be shown.
	*/
	void SetGridVisible(bool visible = true);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const CVector2d& position) override;
	virtual CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	/**
		Calculate automatic axis ranges based on all curve points.
	*/
	void CalculateAutoRanges(istd::CRange& xRange, istd::CRange& yRange) const;

	/**
		Serialize a single curve.
	*/
	bool SerializeCurve(iser::IArchive& archive, Curve& curve);

	QVector<Curve> m_curves;
	QString m_xAxisLabel;
	QString m_yAxisLabel;
	QString m_title;
	istd::CRange m_xAxisRange;		///< User-defined X range (invalid means auto)
	istd::CRange m_yAxisRange;		///< User-defined Y range (invalid means auto)
	bool m_isLegendVisible;
	bool m_isGridVisible;
};


// inline methods

inline int CGraphData2d::GetCurvesCount() const
{
	return m_curves.count();
}


inline const CGraphData2d::Curve& CGraphData2d::GetCurve(int index) const
{
	return m_curves.at(index);
}


inline CGraphData2d::Curve& CGraphData2d::GetCurveRef(int index)
{
	return m_curves[index];
}


inline const QString& CGraphData2d::GetXAxisLabel() const
{
	return m_xAxisLabel;
}


inline const QString& CGraphData2d::GetYAxisLabel() const
{
	return m_yAxisLabel;
}


inline const QString& CGraphData2d::GetTitle() const
{
	return m_title;
}


inline bool CGraphData2d::IsLegendVisible() const
{
	return m_isLegendVisible;
}


inline bool CGraphData2d::IsGridVisible() const
{
	return m_isGridVisible;
}


} // namespace i2d


#endif // !i2d_CGraphData2d_included
