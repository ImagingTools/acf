#ifndef iqt_CHierarchicalCommand_included
#define iqt_CHierarchicalCommand_included


#include <QAction>

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

	explicit CHierarchicalCommand(const istd::CString& name = "", int priority = 100, int staticFlags = CF_ALL_ENABLED);

	void SetPriority(int priority);
	void SetStaticFlags(int flags);

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

	template <class MenuType>
	void CreateMenu(MenuType& result) const;

	// reimplemented (idoc::ICommand)
	virtual int GetPriority() const;
	virtual int GetStaticFlags() const;
	virtual bool Execute(istd::IPolymorphic* contextPtr);

	// reimplemented (istd::TIHierarchical<idoc::ICommand>)
	virtual int GetChildsCount() const;
	virtual idoc::ICommand* GetChild(int index) const;

	// reimplemented (istd::INamed)
	virtual void SetName(const istd::CString& name);

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

	typedef istd::TOptPointerVector<CHierarchicalCommand> Childs;
	Childs m_childs;
};


// public template methods

template <class MenuType>
void CHierarchicalCommand::CreateMenu(typename MenuType& result) const
{
	int childsCount = m_childs.GetCount();

	for (int i = 0; i < childsCount; ++i){
		const idoc::ICommand* childPtr = m_childs.GetAt(i);
		I_ASSERT(childPtr != NULL);

		const CHierarchicalCommand* hierarchicalPtr = dynamic_cast<const CHierarchicalCommand*>(childPtr);

		if (hierarchicalPtr != NULL){
			if (hierarchicalPtr->GetChildsCount() > 0){
				QMenu* newMenuPtr = new QMenu(&result);
				newMenuPtr->setTitle(iqt::GetQString(childPtr->GetName()));

				hierarchicalPtr->CreateMenu<QMenu>(*newMenuPtr);

				result.addMenu(newMenuPtr);
			}
			else{
				result.addAction(const_cast<CHierarchicalCommand*>(hierarchicalPtr));
			}
		}
	}
}


} // namespace iqt


#endif // !iqt_CHierarchicalCommand_included


