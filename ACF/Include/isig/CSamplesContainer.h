#ifndef isig_CSamplesContainer_included
#define isig_CSamplesContainer_included


#include "isig/ISamplesContainer.h"


namespace isig
{


class CSamplesContainer: virtual public ISamplesContainer
{
public:
	// reimplemented (isig::ISamplesContainer)
	virtual void ResetContainer();
	virtual int GetSamplesCount() const;
	virtual bool SetSamplesCount(int count);
	virtual double GetSample(int index) const;
	virtual void SetSample(int index, double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef std::vector<double> Samples;
	Samples m_samples;
};


} // namespace isig


#endif // !isig_CSamplesContainer_included


