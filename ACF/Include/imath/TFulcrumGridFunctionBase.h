#ifndef imath_TFulcrumGridFunctionBase_included
#define imath_TFulcrumGridFunctionBase_included


#include <algorithm>
#include <vector>

#include "istd/TChangeNotifier.h"
#include "istd/TCachedUpdateManagerWrap.h"

#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "imath/TIMathFunction.h"


namespace imath
{


/**
	Base class for interpolated functions based on fulcrums in multi-dimesional grid.
	This grid may be imagined as set of parallel layers for each dimension.
	Number of this layers and their position can be individually controlled.
	Plese note, that method SetDimensionsCount must be called manually to initialize number of input dimensions also for fixed-size arguments.
	\param	Argument	function argument type.
	\param	Result		function result type.
	\param	Fulcrums	array type used to stored fulcrums. It muss define following public typedefs
						\li \c ElementType	type of element stored in this array,
						\li \c IndexType	type of index used to address elements,
						\li \c SizesType	type using to represent array size. This type muss define public typedef \c IndexType.
*/
	template <class Argument, class Result, class Fulcrums>
class TFulcrumGridFunctionBase:
			public istd::TCachedUpdateManagerWrap<iser::ISerializable>,
			public TIMathFunction<Argument, Result>
{
public:
	typedef istd::TCachedUpdateManagerWrap<iser::ISerializable> BaseClass;

	typedef typename Fulcrums::ElementType FulcrumType;
	typedef typename Fulcrums::IndexType FulcrumIndex;
	typedef typename Fulcrums::SizesType FulcrumSizes;

	enum ChangeFlags
	{
		CF_SORT_LAYERS = 0x4000
	};

	/**
		Removes all fulcrum points.
	*/
	void Reset();

	/**
		Return complete grid size.
	*/
	FulcrumSizes GetGridSize() const;

	/**
		Get number of dimensions.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions.
		Please note, that depending on used array type for grid representation not all values must be supported.
		\return	true, if number of dimension was set or false otherwise.
	*/
	virtual bool SetDimensionsCount(int count);

	/**
		Get number of fulcrum points used in this function.
	*/
	int GetLayersCount(int dimension) const;

	/**
		Set number of fulcrum layers at specified dimension.
		Please note, that this fulcrum velues will be not valid after this operation.
	*/
	virtual void SetLayersCount(int dimension, int count);

	/**
		Get position of specified layer of fulcrums.
	*/
	double GetLayerPosition(int dimension, int layerIndex) const;

	/**
		Set position of specified layer of fulcrums.
		Please note, that layer are sorted by its position during update.
		If you want to block update mechanism, use istd::TUpdateNotifier.
	*/
	virtual void SetLayerPosition(int dimension, int layerIndex, double position);

	/**
		Get position of node at specified grid index.
	*/
	void GetFulcrumPosition(const FulcrumIndex& index, ArgumentType& result) const;

	/**
		Get value at specified index.
	*/
	const FulcrumType& GetFulcrumAtIndex(const FulcrumIndex& index) const;

	/**
		Set single fulcrum point.
	*/
	virtual void SetFulcrumAtIndex(const FulcrumIndex& index, const FulcrumType& value);

	/**
		Inserts single fulcrum layer.
		\return	index of added layer.
	*/
	int InsertLayer(int dimension, double position);

	/**
		Remove fulcrums at spacified layer.
	*/
	virtual void RemoveLayer(int dimension, int layerIndex);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Sort fulcrums in this collection.
	*/
	virtual void SortFulcrums();

	/**
		Find layer index of specified value for specified dimension.
		\param	dimension	specifies dimension should be used.
		\param	value		argument value.
		\return				biggest layer index, which position is smaller or equal than specified value
							or -1 if this value is smaller than first layer position.
	*/
	int FindIndex(int dimension, double value) const;
	/**
		Find indices of cuboid containing specified argument value.
	*/
	FulcrumIndex FindIndices(const ArgumentType& argument) const;

	// reimplemented (istd::TCachedUpdateManagerWrap)
	virtual bool CalculateCache(int changeFlags);

private:
	Fulcrums m_fulcrums;

	typedef std::vector<double> LayerPositions;
	typedef std::vector<LayerPositions> Layers;

	Layers m_layers;
};



// inline methods

template <class Argument, class Result, class Fulcrums>
inline typename TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::FulcrumSizes
			TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetGridSize() const
{
	return m_fulcrums.GetSizes();
}


template <class Argument, class Result, class Fulcrums>
inline int TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetDimensionsCount() const
{
	return m_fulcrums.GetDimensionsCount();
}


template <class Argument, class Result, class Fulcrums>
inline int TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetLayersCount(int dimension) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < int(m_layers.size()));

	return m_layers[dimension].size();
}


template <class Argument, class Result, class Fulcrums>
inline double TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetLayerPosition(int dimension, int layerIndex) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < int(m_layers.size()));

	const LayerPositions& positions = m_layers[dimension];

	I_ASSERT(layerIndex >= 0);
	I_ASSERT(layerIndex < int(positions.size()));

	return positions[layerIndex];
}


// public methods

template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::Reset()
{
	m_fulcrums.Reset();

	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		iter->clear();
	}
}


template <class Argument, class Result, class Fulcrums>
bool TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::SetDimensionsCount(int count)
{
	if (m_fulcrums.SetDimensionsCount(count)){
		m_layers.resize(count);

		return true;
	}

	return false;
}


template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::SetLayersCount(int dimension, int count)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < int(m_layers.size()));

	m_layers[dimension].resize(count);
	m_fulcrums.SetSize(dimension, count);
}


template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::SetLayerPosition(int dimension, int layerIndex, double position)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < int(m_layers.size()));

	LayerPositions& positions = m_layers[dimension];

	I_ASSERT(layerIndex >= 0);
	I_ASSERT(layerIndex < int(positions.size()));

	istd::CChangeNotifier notifier(this, CF_SORT_LAYERS);

	positions[layerIndex] = position;
}


template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetFulcrumPosition(const FulcrumIndex& index, ArgumentType& result) const
{
	int layersCount = int(m_layers.size());
	I_ASSERT(layersCount <= index.GetDimensionsCount());

	result.SetElementsCount(layersCount);

	for (int i = 0; i < layersCount; ++i){
		const LayerPositions& positions = m_layers[i];

		I_ASSERT(index[i] >= 0);
		I_ASSERT(index[i] < int(positions.size()));

		result[i] = positions[index[i]];
	}
}


template <class Argument, class Result, class Fulcrums>
typename const TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::FulcrumType&
			TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::GetFulcrumAtIndex(const FulcrumIndex& index) const
{
	return m_fulcrums.GetAt(index);
}


template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::SetFulcrumAtIndex(const FulcrumIndex& index, const FulcrumType& value)
{
	istd::CChangeNotifier notifier(this);

	m_fulcrums.SetAt(index, value);
}


template <class Argument, class Result, class Fulcrums>
int TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::InsertLayer(int dimension, double position)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < m_fulcrums.GetDimensionsCount());

	Fulcrums newFulcrums = m_fulcrums;

	newFulcrums.SetSize(dimension, m_fulcrums.GetSize(dimension) + 1);

	int layerIndex = FindLayerIndex(dimension, position);

	LayerPositions& positions = m_layers[dimension];

	double prevPosition = position;
	double nextPosition = position;
	if (!positions.empty()){
		if (layerIndex >= 0){
			prevPosition = positions[layerIndex];
		}
		if (layerIndex < GetSize(dimension) - 1){
			nextPosition = positions[layerIndex + 1];
		}
	}
	I_ASSERT(position >= prevPosition);
	I_ASSERT(position <= nextPosition);

	double prevFactor = 0.5;
	if (nextPosition - prevPosition > I_BIG_EPSILON){
		prevFactor = (position - prevPosition) / (nextPosition - prevPosition);

		I_ASSERT(prevFactor >= 0);
		I_ASSERT(prevFactor - I_EPSILON <= 1.0);
	}
	double nextFactor = 1 - prevFactor;

	int oldLayersCount = int(positions.size());
	I_ASSERT(oldLayersCount == m_fulcrums.GetSize(dimension));

	for (		Fulcrums::Iterator destIter = newFulcrums.Begin();
				destIter != newFulcrums.End();
				++destIter){
		ArgumentType sourceIndex = destIter;
		if ((destIter[dimension] >= layerIndex) && (sourceIndex[dimension] > 0)){
			sourceIndex[dimension]--;

			if ((destIter[dimension] == layerIndex) && (layerIndex < oldLayersCount)){
				ArgumentType prevFulcrumPosition;
				GetFulcrumPosition(sourceIndex, prevFulcrumPosition);

				ArgumentType nextFulcrumPosition;
				GetFulcrumPosition(destIter, nextFulcrumPosition);

				ArgumentType position = prevFactor * prevFulcrumPosition + nextFactor * nextFulcrumPosition;

				newFulcrums[destIter] = GetValueAt(position);

				continue;
			}
		}

		*destIter = m_fulcrums[sourceIndex];
	}

	positions.insert(positions.begin() + layerIndex + 1, position);
	m_fulcrums = newFulcrums;
}


template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::RemoveLayer(int dimension, int layerIndex)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < GetDimensionsCount());
	I_ASSERT(m_fulcrums.GetSize(dimension) > 0);

	Fulcrums newFulcrums = m_fulcrums;

	newFulcrums.SetSize(dimension, m_fulcrums.GetSize(dimension) + 1);

	LayerPositions& positions = m_layers[dimension];

	for (		Fulcrums::Iterator destIter = newFulcrums.Begin();
				destIter != newFulcrums.End();
				++destIter){
		FulcrumIndex sourceIndex = destIter;
		if (destIter[dimension] >= layerIndex){
			sourceIndex.IncreaseAt(dimension);
		}

		*destIter = m_fulcrums[sourceIndex];
	}

	positions.erase(positions.begin() + layerIndex);
	m_fulcrums = newFulcrums;
}


// reimplemented (iser::ISerializable)

template <class Argument, class Result, class Fulcrums>
bool TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	iser::CArchiveTag gridTag("Grid", "Description of fulcrums grid");
	iser::CArchiveTag positionsTag("Positions", "List of positions at single axis of fulcrum grid");
	iser::CArchiveTag positionTag("Position", "Single layer position");
	iser::CArchiveTag fulcrumsTag("Fulcrums", "List of fulcrums");
	iser::CArchiveTag fulcrumTag("Fulcrum", "Single fulcrum");

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	int dimensionsCount = m_fulcrums.GetDimensionsCount();
	I_ASSERT(dimensionsCount == int(m_layers.size()));

	retVal = retVal && archive.BeginMultiTag(gridTag, positionsTag, dimensionsCount);

	if (!isStoring && (dimensionsCount != m_fulcrums.GetDimensionsCount())){
		if (!SetDimensionsCount(dimensionsCount)){
			return false;
		}
	}

	FulcrumSizes sizes = m_fulcrums.GetSizes();
	I_ASSERT(sizes.GetDimensionsCount() >= dimensionsCount);

	for (int dimensionIndex = 0; dimensionIndex < dimensionsCount; ++dimensionIndex){
		LayerPositions& positions = m_layers[dimensionIndex];

		int& positionsCount = sizes[dimensionIndex];
		I_ASSERT(positionsCount == int(positions.size()));

		retVal = retVal && archive.BeginMultiTag(positionsTag, positionTag, positionsCount);

		if (!isStoring){
			if (!retVal){
				Reset();

				return false;
			}

			SetLayersCount(dimensionIndex, positionsCount);
		}

		for (int positionIndex = 0; positionIndex < positionsCount; ++positionIndex){
			retVal = retVal && archive.BeginTag(positionTag);
			retVal = retVal && archive.Process(positions[positionIndex]);
			retVal = retVal && archive.EndTag(positionTag);
		}

		retVal = retVal && archive.EndTag(positionsTag);
	}

	retVal = retVal && archive.EndTag(gridTag);

	retVal = retVal && archive.BeginTag(fulcrumsTag);

	for (		Fulcrums::Iterator iter = m_fulcrums.Begin();
				iter != m_fulcrums.End();
				++iter){
		FulcrumType& point = *iter;

		retVal = retVal && archive.BeginTag(fulcrumTag);
		retVal = retVal && point.Serialize(archive);
		retVal = retVal && archive.EndTag(fulcrumTag);
	}

	retVal = retVal && archive.EndTag(fulcrumsTag);

	return retVal;
}


// protected methods

template <class Argument, class Result, class Fulcrums>
void TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::SortFulcrums()
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		LayerPositions& positions = *iter;

		std::sort(positions.begin(), positions.end());
	}
}


template <class Argument, class Result, class Fulcrums>
int TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::FindIndex(int dimension, double value) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < GetDimensionsCount());

	EnsureCacheValid();

	const LayerPositions& positions = m_layers[dimension];

	int left = 0;
	int right = int(positions.size());

	while (left != right){
		int center = (left + right) / 2;

		if (value >= positions[center]){
			left = center + 1;
		}
		else{
			right = center;
		}
	}

	return left - 1;
}


template <class Argument, class Result, class Fulcrums>
typename TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::FulcrumIndex
			TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::FindIndices(const ArgumentType& argument) const
{
	int dimensionsCount = GetDimensionsCount();

	FulcrumIndex retVal(dimensionsCount);

	for (int i = 0; i < dimensionsCount; ++i){
		retVal[i] = FindIndex(i, argument[i]);
	}

	return retVal;
}


// reimplemented (istd::TCachedUpdateManagerWrap)

template <class Argument, class Result, class Fulcrums>
bool TFulcrumGridFunctionBase<Argument, Result, Fulcrums>::CalculateCache(int changeFlags)
{
	bool retVal = BaseClass::CalculateCache(changeFlags);

	if ((changeFlags & CF_SORT_LAYERS) != 0){
		SortFulcrums();
	}

	return retVal;
}


} // namespace imath


#endif // !imath_TFulcrumGridFunctionBase_included


