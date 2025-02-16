#include <icmm/CSpectrum.h>


// ACF includes
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


namespace icmm
{


// public methods

CSpectrum::CSpectrum()
{
}


CSpectrum::CSpectrum(const CSpectrum& other)
	:CSampledFunction(other),
	m_step(0)
{
	SetLogicalRange(other.GetLogicalRange(0));
}


CSpectrum::CSpectrum(int startWavelength, int endWavelength, int step, const std::vector<double>& spectrumSamples)
	:CSampledFunction(int(spectrumSamples.size()))
{
	int count = int(spectrumSamples.size());
	for (int i = 0; i < count; i++){
		SetSampleValue(i, spectrumSamples[i]);
	}

	SetLogicalRange(istd::CRange(startWavelength, endWavelength));
	m_step = step;

	SetLogicalRange(istd::CRange(startWavelength, endWavelength));
}


CSpectrum::CSpectrum(const istd::CIntRange& wavelengthRange, int step, const std::vector<double>& spectrumSamples)
	: CSampledFunction(int(spectrumSamples.size()))
{
	int count = int(spectrumSamples.size());
	for (int i = 0; i < count; i++){
		SetSampleValue(i, spectrumSamples[i]);
	}

	m_step = step;

	SetLogicalRange(istd::CRange(wavelengthRange.GetMinValue(), wavelengthRange.GetMaxValue()));
}


// reimplemented (icmm::ISpectrumInfoProvider)

const ISpectrumInfo* CSpectrum::GetSpectrumInfo() const
{
	return this;
}


// reimplemented (icmm::ISpectrumInfo)

istd::CIntRange CSpectrum::GetSpectralRange() const
{
	istd::CRange logicalRange = GetLogicalRange(0);

	return istd::CIntRange(logicalRange.GetMinValue(), logicalRange.GetMaxValue());
}


int CSpectrum::GetStep() const
{
	return m_step;
}


// reimplemented (iser::ISerializable)

bool CSpectrum::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(!archive.IsStoring() ? this : nullptr);

	QList<double> samples;
	if (archive.IsStoring()){
		Index index;

		int sampleCount = GetTotalSamplesCount();
		for (int i = 0; i < sampleCount; i++){
			index.SetAt(0, i);
			samples.append(GetSampleAt(index));
		}
	}

	bool retVal = iser::CPrimitiveTypesSerializer::SerializeContainer(archive, samples, "Samples", "Sample");

	if (!archive.IsStoring()){
		int sampleCount = samples.size();

		if (!Initialize(sampleCount, 0)){
			return false;
		}

		for (int i = 0; i < sampleCount; i++){
			SetSampleValue(i, samples[i]);
		}
	}

	static iser::CArchiveTag logicalRangeTag(
		"SpectralRangeNM", "Spectral of the spectral values given in nm", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(logicalRangeTag);
	{
		istd::CRange logicalRange = GetLogicalRange(0);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeRange(archive, logicalRange);
		if (!archive.IsStoring()){
			SetLogicalRange(logicalRange);
		}
	}
	retVal = retVal && archive.EndTag(logicalRangeTag);

	static iser::CArchiveTag spectrumInfoTag("Step", "Step in nm between spectral values", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(spectrumInfoTag);
	retVal = retVal && archive.Process(m_step);
	retVal = retVal && archive.EndTag(spectrumInfoTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSpectrum::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY | SO_RESET;
}


bool CSpectrum::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSpectrum* objectPtr = dynamic_cast<const CSpectrum*>(&object);

	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		bool retVal = BaseClass::CopyFrom(*objectPtr);
		m_step = objectPtr->m_step;

		return retVal;
	}

	return false;
}


bool CSpectrum::IsEqual(const IChangeable& object) const
{
	const CSpectrum* objectPtr = dynamic_cast<const CSpectrum*>(&object);

	if (objectPtr != nullptr){
		bool retVal = BaseClass::IsEqual(*objectPtr);
		retVal = retVal && (m_step == objectPtr->m_step);

		return retVal;
	}

	return false;
}


istd::IChangeable* CSpectrum::CloneMe(CompatibilityMode /*mode*/) const
{
	istd::TDelPtr<CSpectrum> clonePtr(new CSpectrum());

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return nullptr;
}


bool CSpectrum::ResetData(CompatibilityMode /*mode*/)
{
	istd::CChangeNotifier notifier(this);

	BaseClass::ResetData();

	m_step = 0;

	return true;
}


} // namespace icmm


