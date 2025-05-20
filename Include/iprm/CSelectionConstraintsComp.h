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


#ifndef iprm_CSelectionConstraintsComp_included
#define iprm_CSelectionConstraintsComp_included


// ACF includes
#include <iser/ISerializable.h>
#include <icomp/CComponentBase.h>
#include <iprm/IOptionsList.h>
#include <iprm/IEnableableParam.h>


namespace iprm
{


/**
	Implementation of selection constraints.
*/
class CSelectionConstraintsComp:
			public icomp::CComponentBase,
			virtual public iprm::IOptionsList
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSelectionConstraintsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_ASSIGN_MULTI_0(m_optionNamesAttrPtr, "OptionNames", "Names of selectable options", true);
		I_ASSIGN_MULTI_0(m_optionDescriptionsAttrPtr, "OptionDescriptions", "Descriptions of selectable options", true);
		I_ASSIGN_MULTI_0(m_optionIdsAttrPtr, "OptionIds", "List of option IDs", true);
		I_ASSIGN_MULTI_0(m_optionsEnablersCompPtr, "OptionEnablers", "List of enablers for the options", false);
	I_END_COMPONENT;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

private:
	I_MULTITEXTATTR(m_optionNamesAttrPtr);
	I_MULTITEXTATTR(m_optionDescriptionsAttrPtr);
	I_MULTIATTR(QByteArray, m_optionIdsAttrPtr);
	I_MULTIREF(iprm::IEnableableParam, m_optionsEnablersCompPtr);
};


} // namespace iprm


#endif // !iprm_CSelectionConstraintsComp_included


