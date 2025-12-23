#ifndef istd_TRanges_included
#define istd_TRanges_included


// STL includes
#include <set>
#include <vector>

// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/TRange.h>


namespace istd
{


/**
	Set of ranges.
	This set is defined by initial begin state (state of minus infinity) and list of state switch points.
	Example given, set [10, 20) is represented as begin state \c false, and two switch points: at 10 and 20.
*/
template <typename ValueType>
class TRanges
{
public:
	typedef QList< TRange<ValueType> > RangeList;
	typedef std::vector< TRange<ValueType> > RangeVector;
	typedef std::set<ValueType> SwitchPoints;

	/**
		Default constructor initializing this set to be empty.
	*/
	TRanges();

	/**
		Convert from simple range.
	*/
	explicit TRanges(const istd::TRange<ValueType>& range);

	/**
		Set this set to be empty.
	*/
	void Reset();

	/**
		Check if this set is empty.
		This set is empty if no point belongs him.
	*/
	bool IsEmpty() const;

	/**
		Get stored switch points.
	*/
	const SwitchPoints& GetSwitchPoints() const;
	/**
		Allows access to stored switch points.
	*/
	SwitchPoints& GetSwitchPointsRef();

	/**
		Insert new switch point.
		It add the new switch point or remove it, if such switch point exist.
		Considering interpretation of switch points, it inverts the sets in range (point, infinity).
	*/
	void InsertSwitchPoint(ValueType point);

	/**
		Get begin state.
		It is true, if values before first switch point belongs to set or not.
	*/
	bool GetBeginState() const;
	/**
		Set begin state.
		Begin state is true, if values before first switch point belongs to set or not.
		Changing of begin state inverts the set.
	*/
	void SetBeginState(bool state);

	/**
		Check if some point belongs to set.
	*/
	bool IsInside(ValueType point) const;

	/**
		Check if some range belongs to set.
	*/
	bool IsInside(const TRange<ValueType>& range) const;

	/**
		Check if some other set belongs to this set.
	*/
	bool IsInside(const TRanges<ValueType>& rangesList) const;

	/**
		Get inverted range.
	*/
	void GetInverted(TRanges<ValueType>& result, const TRange<ValueType>* clipRangePtr) const;

	/**
		Invert this range in place.
	*/
	void Invert(const TRange<ValueType>* clipRangePtr);

	/**
		Get union of two range list.
	*/
	TRanges<ValueType> GetUnion(const TRanges<ValueType>& rangesList) const;

	/**
		Get union of two range lists.
		\overload
	*/
	void GetUnion(const TRanges<ValueType>& rangesList, TRanges<ValueType>& result) const;

	/**
		Calculate union of this range list and the other one.
		Result is stored in this object.
	*/
	void Union(const TRanges<ValueType>& rangesList);

	/**
		Calculate union of this range list and some range.
		Result is stored in this object.
	*/
	void Union(const TRange<ValueType>& range, bool isInverted = false);

	/**
		Get intersection of two range list.
	*/
	TRanges<ValueType> GetIntersection(const TRanges<ValueType>& rangesList) const;

	/**
		Get intersection of two range lists.
		\overload
	*/
	void GetIntersection(const TRanges<ValueType>& rangesList, TRanges<ValueType>& result) const;

	/**
		Calculate intersection of this range list and the other one.
		Result is stored in this object.
	*/
	void Intersection(const TRanges<ValueType>& rangesList);

	/**
		Calculate intersection of this range list and some range.
		Result is stored in this object.
	*/
	void Intersection(const TRange<ValueType>& range, bool isInverted = false);

	/**
		Calculate erosion of this range list.
		Erosion shortens the ends of each range with some value.
		Result is stored in this object.
	*/
	void Erode(ValueType leftValue, ValueType rightValue);

	/**
		Calculate dilatation of this range list.
		Dilatation prolongs the ends of each range with some value for left and right side.
		Result is stored in this object.
	*/
	void Dilate(ValueType leftValue, ValueType rightValue);
	/**
		Remove gaps with some length.
		\param	gapState	state interpreted as a gape.
							If it is false the morphological closing is done,
							otherwhile the morphological opening.
	*/
	void RemoveGaps(ValueType value, bool gapState = false);

	/**
		ShiftRanges all points in this set using specified offset.
	*/
	void ShiftRanges(ValueType offset);

	/**
		Get this set as list of \c istd::TRange objects.
		\param	range	enclosing range.
						Must be provided becouse this set can represent values inclusive infinities.
	*/
	void GetAsList(const TRange<ValueType>& range, RangeList& result) const;

	bool operator==(const TRanges<ValueType>& ranges) const;
	bool operator!=(const TRanges<ValueType>& ranges) const;

	uint GetHashValue(uint seed = 0) const;

private:
	SwitchPoints m_switchPoints;

	bool  m_beginState;
};


template <typename ValueType>
TRanges<ValueType>::TRanges()
:	m_beginState(false)
{
}


template <typename ValueType>
TRanges<ValueType>::TRanges(const istd::TRange<ValueType>& range)
:	m_beginState(false)
{
	if (!range.IsEmpty()){
		m_switchPoints.insert(range.GetMinValue());
		m_switchPoints.insert(range.GetMaxValue());
	}
}


template <typename ValueType>
void TRanges<ValueType>::Reset()
{
	m_switchPoints.clear();
	m_beginState = false;
}


template <typename ValueType>
bool TRanges<ValueType>::IsEmpty() const
{
	return 	m_switchPoints.empty() && (m_beginState == false);
}


template <typename ValueType>
const typename TRanges<ValueType>::SwitchPoints& TRanges<ValueType>::GetSwitchPoints() const
{
	return m_switchPoints;
}


template <typename ValueType>
typename TRanges<ValueType>::SwitchPoints& TRanges<ValueType>::GetSwitchPointsRef()
{
	return m_switchPoints;
}


template <typename ValueType>
void TRanges<ValueType>::InsertSwitchPoint(ValueType point)
{
	std::pair<typename SwitchPoints::iterator, bool> insertionStatus = m_switchPoints.insert(point);
	if (!insertionStatus.second){
		m_switchPoints.erase(insertionStatus.first);
	}
}


template <typename ValueType>
bool TRanges<ValueType>::GetBeginState() const
{
	return m_beginState;
}


template <typename ValueType>
void TRanges<ValueType>::SetBeginState(bool state)
{
	m_beginState = state;
}


template <typename ValueType>
bool TRanges<ValueType>::IsInside(ValueType point) const
{
	bool state = m_beginState;
	for (		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
				iter != m_switchPoints.end();
				++iter){
		if (*iter > point){
			break;
		}

		state = !state;
	}

	return state;
}


template <typename ValueType>
bool TRanges<ValueType>::IsInside(const TRange<ValueType>& range) const
{
	bool state = m_beginState;
	for (		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
				iter != m_switchPoints.end();
				++iter){
		if (*iter > range.GetMinValue()){
			if (state){
				++iter;

				// is the next switch out of checked range?
				return (iter == m_switchPoints.end()) || (*iter >= range.GetMaxValue());
			}
			else{
				return false;
			}
		}

		state = !state;
	}

	return state;
}


template <typename ValueType>
bool TRanges<ValueType>::IsInside(const TRanges<ValueType>& rangesList) const
{
	typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
	typename SwitchPoints::const_iterator listIter = rangesList.m_switchPoints.begin();

	bool state = m_beginState;
	bool listState = rangesList.m_beginState;

	for (;;){
		bool isListCovered = state || !listState;
		if (!isListCovered){
			return false;
		}

		if (iter != m_switchPoints.end()){
			ValueType point = *iter;

			if (listIter != rangesList.m_switchPoints.end()){
				ValueType listPoint = *listIter;

				if (point <= listPoint){
					++iter;
					state = !state;
				}

				if (listPoint <= point){
					++listIter;
					listState = !listState;
				}
			}
			else{
				return true;
			}
		}
		else{
			// if no more segments in this list, check if no more switch is inside
			return listIter == rangesList.m_switchPoints.end();
		}
	}
}


template <typename ValueType>
void TRanges<ValueType>::GetInverted(TRanges<ValueType>& result, const TRange<ValueType>* clipRangePtr) const
{
	if (clipRangePtr != NULL){
		result.Reset();
		result.m_beginState = false;

		int prevPosition = clipRangePtr->GetMinValue();

		bool state = m_beginState;

		for (		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
					iter != m_switchPoints.end();
					++iter){
			ValueType position = *iter;
			if (position >= clipRangePtr->GetMaxValue()){
				break;
			}

			if (position > prevPosition){
				if (!state){
					result.m_switchPoints.insert(prevPosition);
					result.m_switchPoints.insert(position);
				}

				prevPosition = position;
			}

			state = !state;
		}

		if (!state){
			result.m_switchPoints.insert(prevPosition);
			result.m_switchPoints.insert(clipRangePtr->GetMaxValue());
		}
	}
	else{
		result.m_switchPoints = m_switchPoints;
		result.m_beginState = !m_beginState;
	}
}


template <typename ValueType>
void TRanges<ValueType>::Invert(const TRange<ValueType>* clipRangePtr)
{
	if (clipRangePtr != NULL){
		int prevPosition = clipRangePtr->GetMinValue();

		bool state = m_beginState;

		// remove all points before
		for (		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
					(iter != m_switchPoints.end()) && (*iter < prevPosition);
					m_switchPoints.erase(iter++)){
			state = !state;
		}

		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();

		if (!state){
			if ((iter != m_switchPoints.end()) && (*iter > prevPosition)){	// no first element? add it!
				m_switchPoints.insert(prevPosition);
			}
		}

		for (; iter != m_switchPoints.end(); ++iter){
			ValueType position = *iter;
			if (position >= clipRangePtr->GetMaxValue()){	// remove all points after
				m_switchPoints.erase(iter, m_switchPoints.end());

				break;
			}

			state = !state;
		}

		if (!state){
			m_switchPoints.insert(clipRangePtr->GetMaxValue());
		}

		m_beginState = false;
	}
	else{
		m_beginState = !m_beginState;
	}
}


template <typename ValueType>
TRanges<ValueType> TRanges<ValueType>::GetUnion(const TRanges<ValueType>& rangesList) const
{
	TRanges<ValueType> retVal;

	GetUnion(rangesList, retVal);

	return retVal;
}


template <typename ValueType>
void TRanges<ValueType>::GetUnion(const TRanges<ValueType>& rangesList, TRanges<ValueType>& result) const
{
	typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
	typename SwitchPoints::const_iterator listIter = rangesList.m_switchPoints.begin();

	bool state = m_beginState;
	bool listState = rangesList.m_beginState;

	bool resultState = state || listState;

	result.m_beginState = resultState;
	result.m_switchPoints.clear();

	for (;;){
		if (iter != m_switchPoints.end()){
			ValueType point = *iter;

			if (listIter != rangesList.m_switchPoints.end()){
				ValueType listPoint = *listIter;

				if (point <= listPoint){
					++iter;
					state = !state;
				}

				if (listPoint <= point){
					++listIter;
					listState = !listState;
				}

				bool unitedStateAfter = state || listState;
				if (unitedStateAfter != resultState){
					result.m_switchPoints.insert(qMin(point, listPoint));

					resultState = unitedStateAfter;
				}
			}
			else{
				if (!listState){
					result.m_switchPoints.insert(iter, m_switchPoints.end());
				}

				return;
			}
		}
		else{
			if (!state && (listIter != rangesList.m_switchPoints.end())){
				result.m_switchPoints.insert(listIter, rangesList.m_switchPoints.end());
			}

			return;
		}
	}
}


template <typename ValueType>
void TRanges<ValueType>::Union(const TRanges<ValueType>& rangesList)
{
	typename SwitchPoints::iterator iter = m_switchPoints.begin();
	typename SwitchPoints::const_iterator listIter = rangesList.m_switchPoints.begin();

	bool state = m_beginState;
	bool listState = rangesList.m_beginState;

	m_beginState = state || listState;

	for (;;){
		if (iter != m_switchPoints.end()){
			ValueType point = *iter;

			if (listIter != rangesList.m_switchPoints.end()){
				ValueType listPoint = *listIter;

				if (point == listPoint){
					++iter;
					state = !state;
					++listIter;
					listState = !listState;
				}
				else if (point < listPoint){
					if (listState){
						iter = m_switchPoints.erase(iter);
					}
					else{
						++iter;
					}
					state = !state;
				}
				else{
					if (!state){
						iter = m_switchPoints.insert(iter, listPoint);
					}
					++listIter;
					listState = !listState;
				}
			}
			else{
				if (listState){
					m_switchPoints.erase(iter, m_switchPoints.end());
				}

				return;
			}
		}
		else{
			if (!state && (listIter != rangesList.m_switchPoints.end())){
				m_switchPoints.insert(listIter, rangesList.m_switchPoints.end());
			}

			return;
		}
	}
}


template <typename ValueType>
void TRanges<ValueType>::Union(const TRange<ValueType>& range, bool isInverted)
{
	if (range.IsEmpty()){
		return;
	}

	bool firstPointState = m_beginState;
	typename SwitchPoints::const_iterator firstIter = m_switchPoints.begin();
	for (;firstIter != m_switchPoints.end(); ++firstIter){
		if (*firstIter >= range.GetMinValue()){
			break;
		}

		firstPointState = !firstPointState;
	}

	bool secondPointState = firstPointState;
	typename SwitchPoints::const_iterator secondIter = firstIter;
	for (;secondIter != m_switchPoints.end(); ++secondIter){
		if (*secondIter >= range.GetMaxValue()){
			break;
		}

		secondPointState = !secondPointState;
	}

	if (firstIter != secondIter){
		// if there are some points in range...
		if (isInverted){
			m_switchPoints.erase(m_switchPoints.begin(), firstIter);	// remove all points before
			m_switchPoints.erase(m_switchPoints.begin(), secondIter);	// remove all points after
		}
		else{
			m_switchPoints.erase(firstIter, secondIter);	// remove all points before
		}
	}
	else{
		if (isInverted){
			m_switchPoints.clear();
		}
	}

	m_beginState = m_beginState || isInverted;

	if (firstPointState == isInverted){
		m_switchPoints.insert(range.GetMinValue());
	}

	if (secondPointState == isInverted){
		m_switchPoints.insert(range.GetMaxValue());
	}
}


template <typename ValueType>
TRanges<ValueType> TRanges<ValueType>::GetIntersection(const TRanges<ValueType>& rangesList) const
{
	TRanges<ValueType> retVal;

	GetIntersection(rangesList, retVal);

	return retVal;
}


template <typename ValueType>
void TRanges<ValueType>::GetIntersection(const TRanges<ValueType>& rangesList, TRanges<ValueType>& result) const
{
	typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
	typename SwitchPoints::const_iterator listIter = rangesList.m_switchPoints.begin();

	bool state = m_beginState;
	bool listState = rangesList.m_beginState;

	bool resultState = state && listState;

	result.m_beginState = resultState;
	result.m_switchPoints.clear();

	for (;;){
		if (iter != m_switchPoints.end()){
			ValueType point = *iter;

			if (listIter != rangesList.m_switchPoints.end()){
				ValueType listPoint = *listIter;

				if (point <= listPoint){
					++iter;
					state = !state;
				}

				if (listPoint <= point){
					++listIter;
					listState = !listState;
				}

				bool unitedStateAfter = state && listState;
				if (unitedStateAfter != resultState){
					result.m_switchPoints.insert(qMin(point, listPoint));

					resultState = unitedStateAfter;
				}
			}
			else{
				if (listState){
					result.m_switchPoints.insert(iter, m_switchPoints.end());
				}

				return;
			}
		}
		else{
			if (state && (listIter != rangesList.m_switchPoints.end())){
				result.m_switchPoints.insert(listIter, rangesList.m_switchPoints.end());
			}

			return;
		}
	}
}


template <typename ValueType>
void TRanges<ValueType>::Intersection(const TRanges<ValueType>& rangesList)
{
	typename SwitchPoints::iterator iter = m_switchPoints.begin();
	typename SwitchPoints::const_iterator listIter = rangesList.m_switchPoints.begin();

	bool state = m_beginState;
	bool listState = rangesList.m_beginState;

	m_beginState = state && listState;

	for (;;){
		if (iter != m_switchPoints.end()){
			ValueType point = *iter;

			if (listIter != rangesList.m_switchPoints.end()){
				ValueType listPoint = *listIter;

				if (point == listPoint){
					++iter;
					state = !state;
					++listIter;
					listState = !listState;
				}
				else if (point < listPoint){
					if (!listState){
						iter = m_switchPoints.erase(iter);
					}
					else{
						++iter;
					}
					state = !state;
				}
				else{
					if (state){
						iter = m_switchPoints.insert(iter, listPoint);
					}
					++listIter;
					listState = !listState;
				}
			}
			else{
				if (!listState){
					m_switchPoints.erase(iter, m_switchPoints.end());
				}

				return;
			}
		}
		else{
			if (state && (listIter != rangesList.m_switchPoints.end())){
				m_switchPoints.insert(listIter, rangesList.m_switchPoints.end());
			}

			return;
		}
	}
}


template <typename ValueType>
void TRanges<ValueType>::Intersection(const TRange<ValueType>& range, bool isInverted)
{
	m_beginState = !m_beginState;

	Union(range, !isInverted);

	m_beginState = !m_beginState;
}


template <typename ValueType>
void TRanges<ValueType>::Erode(ValueType leftValue, ValueType rightValue)
{
	TRanges<ValueType>::Dilate(-leftValue, -rightValue);
}


template <typename ValueType>
void TRanges<ValueType>::Dilate(ValueType leftValue, ValueType rightValue)
{
	if (leftValue * rightValue < 0){	// do 2 steps for complex dilatation
		Dilate(leftValue, 0);
		Dilate(0, rightValue);

		return;
	}

	ValueType absoluteSum = std::abs(leftValue + rightValue);
	if (absoluteSum <= 0){
		return;	// nothing to do
	}

	bool isErosion = (leftValue + rightValue < 0);
	ValueType leftValueAbs = std::abs(leftValue);
	ValueType rightValueAbs = std::abs(rightValue);

	typename SwitchPoints::iterator iter = m_switchPoints.begin();
	while (iter != m_switchPoints.end()){
		ValueType point = *iter;

		const bool state = std::distance(m_switchPoints.begin(), iter) % 2 > 0 ? m_beginState : !m_beginState;
		if (!state == isErosion){
			// move point back
			if (iter != m_switchPoints.begin()){
				auto prevIter = std::prev(iter);

				if (*prevIter > point - absoluteSum){
					// previous range should be removed - is smaller than the erosion kernel
					m_switchPoints.erase(prevIter);
					m_switchPoints.erase(iter++);

					continue;
				}
			}

			// range can be moved using kernel size
			m_switchPoints.erase(iter++);
			iter = m_switchPoints.insert(iter, point - leftValueAbs);
		}
		else{
			// move point forward
			auto nextIter = std::next(iter);

			if (nextIter != m_switchPoints.end()){
				if (*nextIter < point + absoluteSum){
					// following range should be removed - is smaller than the erosion kernel
					m_switchPoints.erase(nextIter);
					m_switchPoints.erase(iter++);

					continue;
				}
			}

			// range can be moved using kernel size
			m_switchPoints.erase(iter++);
			iter = m_switchPoints.insert(iter, point + rightValueAbs);
		}

		++iter;
	}
}


template <typename ValueType>
void TRanges<ValueType>::RemoveGaps(ValueType value, bool gapState)
{
	if (!gapState){
		Dilate(value, value);
		Erode(value, value);
	}
	else{
		Erode(value, value);
		Dilate(value, value);
	}
}


template <typename ValueType>
void TRanges<ValueType>::ShiftRanges(ValueType offset)
{
	SwitchPoints newSwitchPoints;
	for (		typename SwitchPoints::iterator iter = m_switchPoints.begin();
				iter != m_switchPoints.end();
				++iter){
		newSwitchPoints.insert(*iter + offset);
	}

	m_switchPoints.swap(newSwitchPoints);
}


template <typename ValueType>
void TRanges<ValueType>::GetAsList(const TRange<ValueType>& range, RangeList& result) const
{
	int prevPosition = range.GetMinValue();

	bool state = m_beginState;

	for (		typename SwitchPoints::const_iterator iter = m_switchPoints.begin();
				iter != m_switchPoints.end();
				++iter){
		ValueType position = *iter;
		if (position >= range.GetMaxValue()){
			break;
		}

		if (position > prevPosition){
			if (state){
				result.push_back(TRange<ValueType>(prevPosition, position));
			}

			prevPosition = position;
		}

		state = !state;
	}

	if (state){
		result.push_back(TRange<ValueType>(prevPosition, range.GetMaxValue()));
	}
}


template <typename ValueType>
bool TRanges<ValueType>::operator==(const TRanges<ValueType>& ranges) const
{
	return (m_beginState == ranges.m_beginState) && (m_switchPoints == ranges.m_switchPoints);
}


template <typename ValueType>
bool TRanges<ValueType>::operator!=(const TRanges<ValueType>& ranges) const
{
	return (m_beginState != ranges.m_beginState) || (m_switchPoints != ranges.m_switchPoints);
}


template <typename ValueType>
uint TRanges<ValueType>::GetHashValue(uint seed) const
{
	uint retVal = seed;

	if (m_beginState){
		retVal++;
	}

	for (		typename TRanges<ValueType>::SwitchPoints::const_iterator iter = m_switchPoints.begin();
				iter != m_switchPoints.end();
				++iter){
		const ValueType& pos = *iter;

		retVal = retVal ^ uint(pos);
	}

	return retVal;
}


// related global functions

template <typename ValueType>
inline uint qHash(const istd::TRanges<ValueType>& key, uint seed = 0)
{
	return key.GetHashValue(seed);
}


// typedefs

typedef TRanges<double> CRanges;
typedef TRanges<int> CIntRanges;


} // namespace istd


#endif //!istd_TRanges_included


