#include "iimg/CHistogram.h"


#include "ibase/CSize.h"


namespace iimg
{


CHistogram::CHistogram()
{
	Reset();
}



CHistogram::CHistogram(const iimg::IBitmap& input, const i2d::CRectangle* areaPtr)
{
	Reset();

	CalculateFromBitmap(input, areaPtr);
}


void CHistogram::CalculateFromBitmap(const iimg::IBitmap& input, const i2d::CRectangle* areaPtr)
{
	Reset();

	ibase::CSize inputSize(input.GetImageSize());
	inputSize = inputSize - ibase::CSize(1, 1);

	if (!inputSize.IsValid()){
		m_isValid = false;
		return;
	}

	i2d::CRectangle realArea = i2d::CRectangle(inputSize);
	if (areaPtr != NULL){
		realArea  = areaPtr->GetIntersection(i2d::CRectangle(inputSize));
	}

	if (realArea.IsEmpty() || !realArea.IsValid()){
		m_isValid = false;
		return;
	}

	int leftArea = int(realArea.GetLeft());
	int topArea = int(realArea.GetTop());
	int bottomArea = int(realArea.GetBottom());
	int rightArea = int(realArea.GetRight());

	HistogramData::iterator begIter = m_histogramData.begin();
	for (int y = topArea; y <= bottomArea; y++){
		I_BYTE* lineDataBeg = (I_BYTE*)input.GetLinePtr(y) + leftArea;
		I_BYTE* lineDataEnd = (I_BYTE*)input.GetLinePtr(y) + rightArea;

		while (lineDataBeg <= lineDataEnd){
			I_BYTE pixelValue = *lineDataBeg++;
			HistogramData::iterator posIter = begIter + pixelValue;
			(*posIter)++;
		}
	}
}


CHistogram::CounterType& CHistogram::operator[](int index)
{
	I_ASSERT(index >= 0 && index < (int)m_histogramData.size());

	return  m_histogramData[index];
}


bool CHistogram::IsValid() const
{
	return m_isValid;
}


void CHistogram::Reset()
{
	m_histogramData.resize(256, 0);

	m_isValid = true;
}


bool CHistogram::operator == (const CHistogram& other) const
{
	if(m_histogramData.size() != other.m_histogramData.size()){
		return false;
	}

	HistogramData::const_iterator begIter = m_histogramData.begin();
	HistogramData::const_iterator begIter2 = other.m_histogramData.begin();

	while (begIter != m_histogramData.end()){
		if (*begIter != *begIter2){
			return false;
		}
		++begIter;
		++begIter2;
	}

	return true;
}


bool CHistogram::operator != (const CHistogram& other) const
{
	return !operator == (other);
}


double CHistogram::GetDataMaximum(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	double dataMaximum = 0;
	while (begIter != endIter){
		if (*endIter != 0){
			dataMaximum = endIter - begIter;
			break;
		}

		endIter--;
	}

	return dataMaximum;
}


double CHistogram::GetDataMinimum(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	double dataMinimum = 0;

	while (begIter != endIter){
		if (*begIter != 0){
			dataMinimum = begIter - m_histogramData.begin();
			break;
		}
		begIter++;
	}

	return dataMinimum;
}


double CHistogram::GetDataAverage(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	double dataAverage = 0;
	CHistogram::CounterType sumIndex = 0;

	while (begIter != endIter){
		CounterType histValue = *begIter;
		int index = begIter - m_histogramData.begin();
		dataAverage += histValue*index;
		sumIndex += histValue;
		begIter++;
	}

	dataAverage /= (double)sumIndex;

	return dataAverage;
}
	

double CHistogram::GetDataVariance(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	double dataAverage = GetDataAverage(intervallFrom, intervallTo);
	double dataVariance = 0.0;
	double histogramSum = 0.0;

	while (begIter != endIter){
		CounterType histValue = *begIter;
		histogramSum += histValue;
		begIter++;
	}

	begIter = m_histogramData.begin() + intervallFrom;
	if (histogramSum == 0){
		return 0.0;
	}

	while (begIter != endIter){
		CounterType histValue = *begIter;
		int index = begIter - m_histogramData.begin();
		dataVariance += (index - dataAverage) * (index - dataAverage) * histValue/histogramSum;
		begIter++;
	}
	
	return sqrt(dataVariance);
}


CHistogram::HistogramItem CHistogram::GetHistogramMaximum(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	CounterType histMaximum = *begIter;
	int index = intervallFrom;

	while (begIter != endIter){
		if (*begIter > histMaximum){
			histMaximum = *begIter;
			index = begIter - m_histogramData.begin();
		}
		begIter++;
	}

	return HistogramItem(index, histMaximum);
}


CHistogram::HistogramItem CHistogram::GetHistogramMinimum(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	CounterType histMinimum = *begIter;
	int index = intervallFrom;

	while (begIter != endIter){
		if (*begIter < histMinimum){
			histMinimum = *begIter;
			index = begIter - m_histogramData.begin();
		}
		begIter++;
	}

	return HistogramItem(index, histMinimum);
}


double CHistogram::GetHistogramAverage(int intervallFrom, int intervallTo) const
{
	I_ASSERT(intervallFrom <= intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::const_iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::const_iterator endIter = m_histogramData.begin() + intervallTo + 1;
	
	double histAverage = 0;
	int range = endIter - begIter;

	while (begIter != endIter){
		histAverage += *begIter;
		begIter++;
	}

	histAverage /= double(range);

	return histAverage;
}


iimg::CHistogram& CHistogram::Smooth(int iterationCount, const std::vector<int> & kernel)
{
	I_ASSERT(kernel.size() % 2 != 0);

	CHistogram output;
	int filterHalfWidth = int(kernel.size())/2;

	for (int i = 0; i < iterationCount; i++){
		HistogramData::iterator histBeg = m_histogramData.begin();
		HistogramData::iterator histOutputBeg = output.m_histogramData.begin();

		while (histBeg != m_histogramData.end()){
			HistogramData::iterator histLocalBegin = histBeg - filterHalfWidth;
			HistogramData::iterator histLocalEnd = histBeg + filterHalfWidth;
			std::vector<int>::const_iterator kernelBeg = kernel.begin();
			CounterType kernelSum = 0.0; 

			CounterType kernelResponse = 0.0;
			while (kernelBeg != kernel.end()){
				if (histLocalBegin >= m_histogramData.begin() && histLocalBegin < m_histogramData.end()){
					kernelResponse += *histLocalBegin * (*kernelBeg);
					kernelSum += *kernelBeg;
				}

				kernelBeg++;
				histLocalBegin++;
			}

			kernelResponse /= kernelSum;
			*histOutputBeg = kernelResponse;
			++histBeg;
			++histOutputBeg;
		}

		*this = output;
	}
	
	return *this;
}


iimg::CHistogram& CHistogram::Clipp(double threshold, int intervallFrom, int intervallTo)
{
	I_ASSERT(intervallFrom < intervallTo);
	I_ASSERT(intervallFrom >= 0 && intervallTo >= 0);
	I_ASSERT(intervallFrom <  (int)m_histogramData.size() && intervallTo < (int)m_histogramData.size());

	HistogramData::iterator begIter = m_histogramData.begin() + intervallFrom;
	HistogramData::iterator endIter = m_histogramData.begin() + intervallTo+1;

	HistogramItem histItem = GetHistogramMaximum(intervallFrom, intervallTo);
	double clipBy = histItem.second * threshold;

	while (begIter != endIter){
		if (*begIter < clipBy){
			*begIter = 0;
		}
		begIter++;
	}

	return *this;
}


} // namespace iimg

