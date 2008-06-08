#include "iqtsig/CSampleContainerView.h"


#include <QPainter>


namespace iqtsig
{


CSampleContainerView::CSampleContainerView(QWidget* parentPtr)
:	BaseClass(parentPtr)
{
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
			polyline[i].setY(samplesPtr->GetSample(i) * rectHeight / 100.0);
		}

		painter.drawPolyline(polyline);
	}
}


} // namespace iqtsig


