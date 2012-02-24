#ifndef iqtgui_CHierarchicalCommand_included
#define iqtgui_CHierarchicalCommand_included


// Qt includes
#include <QAction>
#include <QString>


// ACF includes
#include "istd/TOptPointerVector.h"

#include "iprm/CNameParam.h"
#include "iprm/CEnableableParam.h"

#include "ibase/ICommand.h"
#include "ibase/THierarchicalBase.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


/**
	Implementation of hierarchical command based on \c QAction from Qt.
*/
class CHierarchicalCommand:
			public QAction,
			public iprm::CEnableableParam,
			public iprm::CNameParam,
			public ibase::THierarchicalBase<ibase::IHierarchicalCommand>
{
	Q_OBJECT

public:
	typedef QAction BaseClass;
	typedef iprm::CEnableableParam BaseClass2;
	typedef iprm::CNameParam BaseClass3;

	explicit CHierarchicalCommand(const QString& name = "", int priority = 100, int staticFlags = CF_GLOBAL_MENU, int groupId = GI_NORMAL);

	void SetPriority(int priority);
	void SetStaticFlags(int flags);
	void SetGroupId(int groupId);

	/**
		Reset list of childs.
	*/
	void ResetChilds();

	/**
		Insert command to child list.
		Please note, this pointer is not owned by this container and will not be removed.
		\param	commandPtr	pointer to child command instance. It cannot be NULL.
		\param	releaseFlag	if true, command instance will be automatically removed.
		\param	index		index position of command to be inserted. Negative value indicate end of collection.
							Please note, that position can be other in merged tree.
	*/
	void InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag = false, int index = -1);

	/**
		Remove child at specified index.
	*/
	void RemoveChild(int index);

	/**
		Joint the second root as links.
		\param	rootPtr	pointer to root of commands tree. It cannot be NULL.
	*/
	void JoinLinkFrom(const ibase::IHierarchicalCommand* rootPtr);

	/**
		Set all visual elements of this command.
		This method is designed to use in OnRetranslate() implementation and that's why it uses Qt \c QString.
		\param	name		general name, used in menus.
		\param	shortName	short version of name used in toolbars.
		\param	description	description used as tool tip.
		\param	icon		icon shown in menus and toolbars.
	*/
	void SetVisuals(const QString& name, const QString& shortName, const QString& description, const QIcon& icon = QIcon());

	// reimplemented (ibase::ICommand)
	virtual int GetPriority() const;
	virtual int GetGroupId() const;
	virtual int GetStaticFlags() const;
	virtual bool Execute(istd::IPolymorphic* contextPtr);

	// reimplemented (istd::TIHierarchical<ibase::ICommand>)
	virtual int GetChildsCount() const;
	virtual ibase::ICommand* GetChild(int index) const;

	// reimplemented (iprm::INameParam)
	virtual void SetName(const QString& name);

	// reimplemented (iprm::IEnableableParam)
	virtual void SetEnabled(bool isEnabled = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected Q_SLOTS:
	void OnTriggered();

protected:
	/**
		Find the same command in child list.
		\param	command	command will be used as search template.
		\return	index of found child or negative value, if no child is found.
	*/
	int FindTheSameCommand(const ibase::IHierarchicalCommand& command) const;
	/**
		Find index where element with specified priority should be inserted.
	*/
	int FindInsertingIndex(int priority) const;

private:
	int m_priority;
	int m_staticFlags;
	int m_groupId;

	typedef istd::TOptPointerVector<CHierarchicalCommand> Childs;
	Childs m_childs;
};


} // namespace iqtgui


#endif // !iqtgui_CHierarchicalCommand_included


