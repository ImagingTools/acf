#ifndef iview_IRuler_included
#define iview_IRuler_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/IChangeable.h"

#include "i2d/CLine2d.h"


namespace iview
{


/**
	Interface for ruler implementation.
	Ruler allow to set a values at different positions on different levels.
	Level 0 is the most important.
*/
class IRuler: virtual public istd::IChangeable
{
public:
	typedef QPair<double, double> Mark;

	/**
		Set levels count to 0 and remove all marks.
	*/
	virtual void Reset() = 0;

	/**
		Remove all marks, but doesn't change reserved space.
	*/
	virtual void Clear() = 0;
	
	/**
		Get count of levels.
		Higher levels have lower priority.
	*/
	virtual int GetLevelsCount() const = 0;

	/**
		Set levels count.
		All marks in the levels will be removed.
	*/
	virtual void SetLevelsCount(int count) = 0;
	
	/**
		Get pixels range, that adequate mark value.
	*/
	virtual i2d::CLine2d GetScreenLine() const = 0;
	
	/**
		Reserve marks space to optimize insert operation.
	*/
	virtual void ReserveLevelSpace(int levelIndex, int marksCount) = 0;
	
	/**
		Get count of marks on specified level.
	*/
	virtual int GetMarksCount(int levelIndex) const = 0;
	
	/**
		Get a mark.
		First element of the mark is a relative position on ruler in range [0, 1].
		Second element is the value.
	*/
	virtual Mark GetMark(int markIndex, int levelIndex) const = 0;
	
	/**
		Insert new mark.
		\sa	GetMark(int, int).
	*/
	virtual void InsertMark(int levelIndex, const Mark& mark) = 0;
};


} // namespace iview


#endif // !iview_IRuler_included

