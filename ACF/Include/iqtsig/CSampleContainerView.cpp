#include "iqtsig/CSampleContainerView.h"


#include <QPainter>


namespace iqtsig
{


CSampleContainerView::CSampleContainerView(QWidget* parentPtr)
:	BaseClass(parentPtr),
	m_viewRange(0, 100)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CSampleContainerView::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass2::OnUpdate(updateFlags, updateParamsPtr);

	emit repaint();
}


// reimplemented (QWidget)

void CSampleContainerView::paintEvent(QPaintEvent* /*event*/)
{
	const isig::ISamplesContainer* samplesPtr = GetObjectPtr();
	if (samplesPtr != NULL){
		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing);

		int rectWidth = width();
		int rectHeight = height();

		int samplesCount = samplesPtr->GetSamplesCount();
		QPolygonF polyline(samplesCount);

		for (int i = 0; i < samplesCount; ++i){
			polyline[i].setX(double(i * rectWidth) / samplesCount);
			double sample = samplesPtr->GetSample(i);
			polyline[i].setY((1 - m_viewRange.GetAlphaFromValue(sample)) * rectHeight);
		}

		painter.drawPolyline(polyline);
	}
}


} // namespace iqtsig


