#ifndef iqt_CHierarchicalCommand_included
#define iqt_CHierarchicalCommand_included


#include <QAction>
#include <QString>

#include "istd/TOptPointerVector.h"

#include "ibase/TNamedBase.h"
#include "ibase/THierarchicalBase.h"
#include "ibase/TEnableableWrap.h"

#include "idoc/ICommand.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Implementation of hierarchical command based on \c QAction from Qt.
*/
class CHierarchicalCommand:
			public QAction,
			public ibase::TEnableableWrap< ibase::THierarchicalBase< ibase::TNamedBase<idoc::IHierarchicalCommand> > >
{
public:
	typedef QAction BaseClass;
	typedef ibase::TEnableableWrap< ibase::THierarchicalBase< ibase::TNamedBase<idoc::IHierarchicalCommand> > > BaseClass2;

	explicit CHierarchicalCommand(const istd::CString& name = "", int priority = 100, int staticFlags = CF_GLOBAL_MENU, int groupId = GI_NORMAL);

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
	*/
	void InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag = false);

	/**
		Joint the second root as links.
		\param	rootPtr	pointer to root of commands tree. It cannot be NULL.
	*/
	void JoinLinkFrom(const idoc::IHierarchicalCommand* rootPtr);

	/**
		Set all visual elements of this command.
		This method is designed to use in OnRetranslate() imaplementation and that's why it uses Qt \c QString.
		\param	name		general name, used in menus.
		\param	shortName	short version of name used in toolbars.
		\param	description	description used as tool tip.
		\param	icon		icon shown in menus and toolbars.
	*/
	void SetVisuals(const QString& name, const QString& shortName, const QString& description, const QIcon& icon = QIcon());

	// reimplemented (idoc::ICommand)
	virtual int GetPriority() const;
	virtual int GetGroupId() const;
	virtual int GetStaticFlags() const;
	virtual bool Execute(istd::IPolymorphic* contextPtr);

	// reimplemented (istd::TIHierarchical<idoc::ICommand>)
	virtual int GetChildsCount() const;
	virtual idoc::ICommand* GetChild(int index) const;

	// reimplemented (istd::INamed)
	virtual void SetName(const istd::CString& name);

	// reimplemented (istd::IEnableable)
	virtual void SetEnabled(bool isEnabled = true);

protected:
	/**
		Find the same command in child list.
		\param	command	command will be used as search template.
		\return	index of found child or negative value, if no child is found.
	*/
	int FindTheSameCommand(const idoc::IHierarchicalCommand& command) const;
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


} // namespace iqt


#endif // !iqt_CHierarchicalCommand_included


