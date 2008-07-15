#ifndef iimg_CHistogram_included
#define iimg_CHistogram_included


#include <vector>


#include "i2d/CRectangle.h"

#include "iimg/IBitmap.h"


namespace iimg
{


/**	
	Implementation of image histogram.
*/
class CHistogram: public istd::IPolymorphic
{
public:
	typedef float CounterType;
	typedef std::vector<CounterType> HistogramData;
	typedef std::pair<int /*index*/, CounterType /*value*/> HistogramItem;

	CHistogram();
	CHistogram(const iimg::IBitmap& input, const i2d::CRectangle* areaPtr = NULL);

	bool IsValid() const;
	void Reset();

	void CalculateFromBitmap(const iimg::IBitmap& input, const i2d::CRectangle* areaPtr = NULL);
	CounterType& operator[](int index);

	bool operator == (const iimg::CHistogram& other) const;
	bool operator != (const iimg::CHistogram& other) const;

	double GetDataMaximum(int intervallFrom = 0, int intervallTo = 255) const;
	double GetDataMinimum(int intervallFrom = 0, int intervallTo = 255) const;
	double GetDataAverage(int intervallFrom = 0, int intervallTo = 255) const;
	double GetDataVariance(int intervallFrom = 0, int intervallTo = 255) const;

	HistogramItem GetHistogramMaximum(int intervallFrom = 0, int intervallTo = 255) const;
	HistogramItem GetHistogramMinimum(int intervallFrom = 0, int intervallTo = 255) const;
	double GetHistogramAverage(int intervallFrom = 0, int intervallTo = 255) const;

	iimg::CHistogram& Smooth(int iterationCount, const std::vector<int> & kernel);
	iimg::CHistogram& Clipp(double threshold, int intervallFrom = 0, int intervallTo = 255);

private:
	HistogramData m_histogramData;
	bool m_isValid;
};


} // namespace iimg


#endif // !iimg_CHistogram_included

