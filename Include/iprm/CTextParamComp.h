#ifndef iprm_CTextParamComp_included
#define iprm_CTextParamComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <iprm/CTextParam.h>


namespace iprm
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CTextParamComp:
			public icomp::CComponentBase,
			public iprm::CTextParam
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iprm::CTextParam BaseClass2;

	I_BEGIN_COMPONENT(CTextParamComp);
		I_REGISTER_INTERFACE(iprm::ITextParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "");
		I_ASSIGN(m_isReadonlyAttrPtr, "IsReadOnly", "When enabled, the text is fixed and cannot be changed during run tume", true, false);
	I_END_COMPONENT;

	// reimplemented (iprm::INameParam)
	virtual void SetText(const QString& name) override;
	virtual bool IsReadOnly() const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_TEXTATTR(m_defaultTextAttrPtr);
	I_ATTR(bool, m_isReadonlyAttrPtr);
};


} // namespace iprm


#endif // !iprm_CTextParamComp_included


