#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <icmm/ISpectrumInfo.h>


namespace icmm
{


/**
	Data model implementation of ISpectrumInfo interface.
*/
class CSpectrumInfo: virtual public ISpectrumInfo, virtual public iser::ISerializable
{
public:
	CSpectrumInfo();

	bool SetRange(const istd::CIntRange& range);
	bool SetStep(int step);

	bool operator==(const CSpectrumInfo& other) const;

	// reimplemented (icmm::ISpectrumInfo)
	virtual istd::CIntRange GetSpectralRange() const override;
	virtual int GetStep() const override;
	virtual int GetSamplesCount() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	istd::CIntRange m_range;
	int m_step;
};


} // namespace icmm


