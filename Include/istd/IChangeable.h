#ifndef istd_IChangeable_included
#define istd_IChangeable_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QSet>

// ACF includes
#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Common interface for data model objects, which can be changed.
	Data model changes are executed in two steps: begin and end.
	The caller can also put some flags and additional data to the update functions, to control the update logic.

	\ingroup DataModel
*/
class IChangeable: virtual public IPolymorphic  
{
public:
	/**
		Set of change flags (its IDs).
	*/
	class ChangeSet
	{
	public:
		ChangeSet();
		explicit ChangeSet(const QString& description);
		explicit ChangeSet(int id1, const QString& description = "");
		ChangeSet(int id1, int id2, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, int id9, const QString& description = "");
		ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, int id9, int id10, const QString& description = "");

		/**
			Remove all IDs.
		*/
		void Reset();

		/**
			Check if there is any change in the set.
		*/
		bool IsEmpty() const;
		/**
			Check if there is specific change flag in the set.
		*/
		bool Contains(int changeId) const;
		/**
			Check if there is specific change flag in the set explicit set by user.
			There are some control flags, used to indicate that all changes were done. The functionality of this flags will be omitted by this check.
			\param	changeId	ID of requested change.
			\param	singleOnly	if it is true, than this function return tru only if the set contains only requested change and nothing more.
		*/
		bool ContainsExplicit(int changeId, bool singleOnly = false) const;
		/**
			Check if any of IDs is changed.
		*/
		bool ContainsAny(const ChangeSet& changeSet) const;

		/**
			Remove the IDs from the second set.
		*/
		void MaskOut(const ChangeSet& changeSet);

		/**
			Get textual description of this change set.
		*/
		const QString& GetDescription() const;

		/**
			Get set of all stored IDs.
			Please note, that there are some special IDs changing interpretation of this set. It will be exported in raw form.
		*/
		QSet<int> GetIds() const;

		/**
			Get the union of two change sets.
		*/
		ChangeSet operator+(const ChangeSet& changeSet) const;

		/**
			Add some change flag.
		*/
		ChangeSet& operator+=(int changeId);

		/**
			Add some change set.
		*/
		ChangeSet& operator+=(const ChangeSet& changeSet);

	private:
		QSet<int> m_ids;

		QString m_description;
	};

	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			Internal ACF flag.
			Indicate that internal update is done.
		*/
		CF_ACF_INTERNAL = 0,

		/**
			All object data are changed.
		*/
		CF_ALL_DATA,

		/**
			Indicate anonymous change.
		*/
		CF_ANY,

		/**
			Change flag indicate that model is during destruction.
		*/
		CF_DESTROYING,

		/**
			Change flag indicate that this change was delegated (is indirect) for example from aggregated object.
		*/
		CF_DELEGATED,

		/**
			Used to avoid adding of changes to undo list.
		*/
		CF_NO_UNDO
	};

	/**
		Flags for supported operations.
		This flags can be combined.
	*/
	enum SupportedOperations
	{
		/**
			No operations are supported.
		*/
		SO_NONE = 0,

		/**
			Observing of this object via observer-observable pattern.
		*/
		SO_OBSERVE = 1 << 0,
		
		/**
			Copying from other object.
		*/
		SO_COPY = 1 << 1,
		
		/**
			Creating of copy of this object.
		*/
		SO_CLONE = 1 << 2,

		/**
			Comparsion with another object.
		*/
		SO_COMPARE = 1 << 3,

		/**
			Resetting of object state.
		*/
		SO_RESET = 1 << 4
	};

	/**
		Control how relationship betweeen objects are interpreted.
		It is also used to define how objects are copied.
	*/
	enum CompatibilityMode
	{
		/**
			If external references are different this object are not compatible and cannot be compared or copied.
			Copy without references to external objects only if objects are compatible.
		*/
		CM_STRICT,
		/**
			External references are simple ignored.
			Copy without references to external objects.
		*/
		CM_WITHOUT_REFS,
		/**
			External references are part of object integrity.
			For comparation external references must be equal, for copy operations they will be copied.
		*/
		CM_WITH_REFS,
		/**
			Data of object are interpreted in context of external references.
			For comparation the meaning of object (e.g. after transformation using externals) will be compared.
			Copying will converting the data to be compatible according to existing references to external objects.
		*/
		CM_CONVERT
	};

	/**
		Get set of flags for supported operations.
		\sa SupportedOperations
	*/
	virtual int GetSupportedOperations() const;

	/**
		Copy this object from another one.
		Default implementation in istd::IChangeable does nothing.
	*/
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

	/**
		Compare this object with another object.
		\param object	Object to be compared
		\return \c true if the objects are identical and \c false otherwise.
	*/
	virtual bool IsEqual(const IChangeable& object) const;

	/**
		Make a copy of this object.
		\return	new instance or NULL, if this operation is not supported.
	*/
	virtual IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	/**
		Reset data to its default state.
		Default implementation in istd::IChangeable does nothing.
		\return \c true if the operation was successful, and \c false otherwise.
	*/
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

	/**
		Starts the change transaction.
		\note It should be never called or override by user. Please use \c OnBeginChanges instead.
		\sa \c EndChanges
	*/
	virtual void BeginChanges(const ChangeSet& changeSet);

	/**
		Ends the change transaction.
		\note It should be never override called or by user. Please use \c OnEndChanges instead.
		\param	changeSet	set of IDs used to identify what kind of change was done.
							for the empty set you can assume, that no change was done.
		\sa \c BeginChanges
	*/
	virtual void EndChanges(const ChangeSet& changeSet);

	/**
		Starts group of changes.
		It doesn't start signaling of changes, it allows simply to group set of changes into single transaction block.
		\note It should be never called or override by user.
		\sa EndChangeGroup()
	*/
	virtual void BeginChangeGroup(const ChangeSet& changeSet);

	/**
		Ends group of changes.
		It doesn't start signaling of changes, it allows simply to group set of changes into single transaction block.
		\note It should be never called or override by user.
		\sa BeginChangeGroup
	*/
	virtual void EndChangeGroup(const ChangeSet& changeSet);

	// static methods
	/**
		Get empty set of changes.
	*/
	static const ChangeSet& GetNoChanges();

	/**
		Get anonymous change set.
	*/
	static const ChangeSet& GetAnyChange();

	/**
		Get anonymous change set.
	*/
	static const ChangeSet& GetAllChanges();

	/**
		Get delegated change set.
	*/
	static const ChangeSet& GetDelegatedChanges();

protected:
	/**
		Callback function for begin change event.
		Please note, that in some cases no following OnEndChanges can be called.
	*/
	virtual void OnBeginChanges();

	/**
		Callback function for end change event.
	*/
	virtual void OnEndChanges(const ChangeSet& changeSet);

private:
	// static attributes
	static const ChangeSet s_emptyChanges;
	static const ChangeSet s_anyChanges;
	static const ChangeSet s_allChanges;
	static const ChangeSet s_delegatedChanges;
};


// public inline methods

inline int IChangeable::GetSupportedOperations() const
{
	return SO_NONE;
}


inline bool IChangeable::CopyFrom(const IChangeable& /*object*/, CompatibilityMode /*mode*/)
{
	return false;
}


inline bool IChangeable::IsEqual(const IChangeable& /*object*/) const
{
	return false;
}


inline IChangeable* IChangeable::CloneMe(CompatibilityMode /*mode*/) const
{
	return NULL;
}


inline bool IChangeable::ResetData(CompatibilityMode /*mode*/)
{
	return false;
}


inline void IChangeable::BeginChanges(const ChangeSet& /*changeSet*/)
{
	OnBeginChanges();
}


inline void IChangeable::EndChanges(const ChangeSet& changeSet)
{
	OnEndChanges(changeSet);
}


inline void IChangeable::BeginChangeGroup(const ChangeSet& /*changeSet*/)
{
}


inline void IChangeable::EndChangeGroup(const ChangeSet& /*changeSet*/)
{
}


// public static inline methods
/**
	Get empty set of changes.
*/
inline const IChangeable::ChangeSet& IChangeable::GetNoChanges()
{
	return s_emptyChanges;
}


inline const IChangeable::ChangeSet& IChangeable::GetAnyChange()
{
	return s_anyChanges;
}


inline const IChangeable::ChangeSet& IChangeable::GetAllChanges()
{
	return s_allChanges;
}


inline const IChangeable::ChangeSet& IChangeable::GetDelegatedChanges()
{
	return s_delegatedChanges;
}


// protected inline methods

inline void IChangeable::OnBeginChanges()
{
}


inline void IChangeable::OnEndChanges(const ChangeSet& /*changeSet*/)
{
}


} // namespace istd


#endif // !istd_IChangeable_included

