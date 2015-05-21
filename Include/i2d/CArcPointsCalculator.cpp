#include "i2d/CArcPointsCalculator.h"


// Qt includes
#include <qmath.h>


namespace i2d
{


// public static methods

bool CArcPointsCalculator::GetArcPoints(
			int xCenter,
			int yCenter,
			int radius,
			int startAngle,
			int endAngle,
			Coordinates& xPoints,
			Coordinates& yPoints)
{
	xPoints.clear();
	yPoints.clear();

	// no radius - no points
	if (radius <= 0){
		return false;
	}

	// calculate start & end angles
	int start = startAngle % 360;
	int end = endAngle % 360;
	if (start < 0) 
		start += 360;
	if (end < 0) 
		end += 360;

	// to do: complete circle?
	if (start == end){
		return false;
	}

	// temp points buffer
	std::vector<int> xBuffer, yBuffer;
	xBuffer.reserve(radius);
	yBuffer.reserve(radius);

	// 1st point
	xBuffer.push_back(0);
	yBuffer.push_back(radius);

	// calculate 1/8 of the arc
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	while (x < y){
		if (f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		xBuffer.push_back(x);
		yBuffer.push_back(y);
	};

	// arc segment is 45 grad
	int bufferSize = int(xBuffer.size());
	int bufferLastIndex = bufferSize - 1;
	float gradStep = float(bufferSize) / 45.0;

	// calculate outputs
	xPoints.reserve(radius*4);
	yPoints.reserve(radius*4);

	bool isFinished = false;
	while (!isFinished)
	{
		// sector 1
		if (start >= 0 && start < 45){
			int startIndex = gradStep * start;
			int endIndex = bufferSize;
			if (end >= 0 && end < 45 && start <= end){
				endIndex = gradStep * end;
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter + yBuffer[i];
				int py = yCenter - xBuffer[i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 45;
		}

		// sector 2
		if (start >= 45 && start < 90){
			int startIndex = gradStep * (start - 45);
			int endIndex = bufferSize;
			if (end >= 45 && end < 90 && start <= end) {
				endIndex = gradStep * (end - 45);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter + xBuffer[bufferLastIndex - i];
				int py = yCenter - yBuffer[bufferLastIndex - i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 90;
		}

		// sector 3
		if (start >= 90 && start < 135){
			int startIndex = gradStep * (start - 90);
			int endIndex = bufferSize;
			if (end >= 90 && end < 135 && start <= end) {
				endIndex = gradStep * (end - 90);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter - xBuffer[i];
				int py = yCenter - yBuffer[i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 135;
		}

		// sector 4
		if (start >= 135 && start < 180){
			int startIndex = gradStep * (start - 135);
			int endIndex = bufferSize;
			if (end >= 135 && end < 180 && start <= end) {
				endIndex = gradStep * (end - 135);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter - yBuffer[bufferLastIndex - i];
				int py = yCenter - xBuffer[bufferLastIndex - i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 180;
		}

		// sector 5
		if (start >= 180 && start < 225){
			int startIndex = gradStep * (start - 180);
			int endIndex = bufferSize;
			if (end >= 180 && end < 225 && start <= end) {
				endIndex = gradStep * (end - 180);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter - yBuffer[i];
				int py = yCenter + xBuffer[i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 225;
		}

		// sector 6
		if (start >= 225 && start < 270){
			int startIndex = gradStep * (start - 225);
			int endIndex = bufferSize;
			if (end >= 225 && end < 270 && start <= end) {
				endIndex = gradStep * (end - 225);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter - xBuffer[bufferLastIndex - i];
				int py = yCenter + yBuffer[bufferLastIndex - i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 270;
		}

		// sector 7
		if (start >= 270 && start < 315){
			int startIndex = gradStep * (start - 270);
			int endIndex = bufferSize;
			if (end >= 270 && end < 315 && start <= end) {
				endIndex = gradStep * (end - 270);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter + xBuffer[i];
				int py = yCenter + yBuffer[i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 315;
		}

		// sector 8
		if (start >= 315 && start < 360){
			int startIndex = gradStep * (start - 315);
			int endIndex = bufferSize;
			if (end >= 315 && end < 360 && start <= end) {
				endIndex = gradStep * (end - 315);
				isFinished = true;
			}

			for (int i = startIndex; i < endIndex; i++){
				int px = xCenter + yBuffer[bufferLastIndex - i];
				int py = yCenter + xBuffer[bufferLastIndex - i];
				xPoints.push_back(px);
				yPoints.push_back(py);
			}

			if (isFinished)
				break;

			start = 360;
		}

		// if not finished: repeat
		start = 0;
	};

	return true;
}	


} // namespace i2d


