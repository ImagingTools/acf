/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


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
		I_ASSIGN(m_serializeParamsAttrPtr, "SerializeParams", "If set the parameters will be serialized/saved (true by default)", true, true);
	I_END_COMPONENT;

	// reimplemented (iprm::INameParam)
	virtual void SetText(const QString& name) override;
	virtual bool IsReadOnly() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_TEXTATTR(m_defaultTextAttrPtr);
	I_ATTR(bool, m_isReadonlyAttrPtr);
	I_ATTR(bool, m_serializeParamsAttrPtr);
};


} // namespace iprm


#endif // !iprm_CTextParamComp_included


