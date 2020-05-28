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
	virtual QString GetText() const;
	virtual void SetText(const QString& text);
	virtual bool IsReadOnly() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool IsEqual(const IChangeable& object) const;
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	QString m_text;
};


} // namespace iprm


#endif // !iprm_CTextParam_included


