#ifndef iprm_CTextParam_included
#define iprm_CTextParam_included


// ACF includes
#include <iprm/ITextParam.h>


namespace iprm
{


/**
	Implementation of the text value over iprm::ITextParam interface.
*/
class CTextParam: virtual public ITextParam
{
public:
	// reimplemented (iprm::ITextParam)
	virtual QString GetText() const override;
	virtual void SetText(const QString& text) override;
	virtual bool IsReadOnly() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

protected:
	QString m_text;
};


} // namespace iprm


#endif // !iprm_CTextParam_included


