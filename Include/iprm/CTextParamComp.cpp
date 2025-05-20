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


#include <iprm/CTextParamComp.h>


namespace iprm
{


// public methods

// reimplemented (iprm::INameParam)

void CTextParamComp::SetText(const QString& name)
{
	if (!*m_isReadonlyAttrPtr){
		BaseClass2::SetText(name);
	}
}


bool CTextParamComp::IsReadOnly() const
{
	return *m_isReadonlyAttrPtr;
}


// reimplemented (iser::ISerializable)

bool CTextParamComp::Serialize(iser::IArchive& archive)
{
	if (!*m_serializeParamsAttrPtr) {
		return true;
	}

	return BaseClass2::Serialize(archive);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTextParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultTextAttrPtr.IsValid()){
		BaseClass2::SetText(*m_defaultTextAttrPtr);
	}
}


} // namespace iprm


