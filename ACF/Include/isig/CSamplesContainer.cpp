#include "isig/CSamplesContainer.h"


#include "istd/TChangeNotifier.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isig
{


// reimplemented (isig::ISamplesContainer)

void CSamplesContainer::ResetContainer()
{
	m_samples.clear();
}


int CSamplesContainer::GetSamplesCount() const
{
	return int(m_samples.size());
}


bool CSamplesContainer::SetSamplesCount(int count)
{
	m_samples.resize(count, 0.0);

	return true;
}


double CSamplesContainer::GetSample(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_samples.size()));

	return m_samples[index];
}


void CSamplesContainer::SetSample(int index, double value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_samples.size()));

	m_samples[index] = value;
}


// reimplemented (iser::ISerializable)

bool CSamplesContainer::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag samplesListTag("SampleList", "List of sample values");
	static iser::CArchiveTag sampleTag("SampleList", "List of sample values");

	bool retVal = true;

	int samplesCount = int(m_samples.size());

	retVal = retVal && archive.BeginMultiTag(samplesListTag, sampleTag, samplesCount);

	if (!retVal){
		return false;
	}

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		m_samples.resize(samplesCount);
	}

	for (int i = 0; i < samplesCount; ++i){
		retVal = retVal && archive.BeginTag(sampleTag);
		retVal = retVal && archive.Process(m_samples[i]);
		retVal = retVal && archive.EndTag(sampleTag);
	}

	retVal = retVal && archive.EndTag(samplesListTag);

	return retVal;
}


} // namespace isig


