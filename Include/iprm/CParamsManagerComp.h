/********************************************************************************
**
**	Copyright (C) 2007-2011 Witold Gantzke & Kirill Lepskiy
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
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the ACF.
**
********************************************************************************/


#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TDelPtr.h"
#include "imod/TModelWrap.h"
#include "imod/CMultiModelBridgeBase.h"
#include "icomp/CComponentBase.h"
#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsList.h"
#include "iprm/INameParam.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public IOptionsList,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "unnamed_%1");
		I_ASSIGN(m_elementIndexParamId, "ElementIndexParamId", "ID of index of returned parameter set in manager list", false, "Index");
		I_ASSIGN(m_elementNameParamId, "ElementNameParamId", "ID of index of returned parameter set in manager list", false, "Name");
		I_ASSIGN(m_paramsSetTypeIdAttrPtr, "ParamsSetTypeId", "ID of factorisied parameter set", true, "Default");
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet");
	I_END_COMPONENT;

	CParamsManagerComp();

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual int GetIndexOperationFlags(int index = -1) const;
	virtual int GetParamsSetsCount() const;
	virtual const IOptionsList* GetParamsTypeConstraints() const;
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual QString GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const QString& name);

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	QString GetNewSetName() const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);
	I_ATTR(QByteArray, m_paramsSetTypeIdAttrPtr);
	I_ATTR(QByteArray, m_elementIndexParamId);
	I_ATTR(QByteArray, m_elementNameParamId);
	I_FACT(IParamsSet, m_paramSetsFactPtr);

	class ParamSet:
				public CMultiModelBridgeBase,
				virtual public IParamsSet,
				virtual public ISelectionParam,
				virtual public INameParam
	{
	public:
		ParamSet();

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

		// reimplemented (iprm::ISelectionParam)
		virtual const IOptionsList* GetSelectionConstraints() const;
		virtual int GetSelectedOptionIndex() const;
		virtual bool SetSelectedOptionIndex(int index);
		virtual ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iser::INameParam)
		virtual const QString& GetName() const;
		virtual void SetName(const QString& name);
		virtual bool IsNameFixed() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		istd::TDelPtr<IParamsSet> paramSetPtr;
		QString name;
		const CParamsManagerComp* parentPtr;
	};
	
	int FindParamSetIndex(const QString& name) const;

	typedef istd::TDelPtr<ParamSet> ParamSetPtr;

	typedef QList<ParamSetPtr> ParamSets;

	ParamSets m_paramSets;

	int m_selectedIndex;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


