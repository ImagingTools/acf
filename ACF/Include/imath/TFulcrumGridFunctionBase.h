#ifndef imath_TFulcrumGridFunctionBase_included
#define imath_TFulcrumGridFunctionBase_included


#include "istd/TChangesReductorWrap.h"

#include "iser/ISerializable.h"

#include "imath/TIMathFunction.h"
#include "imath/CVector2d.h"


namespace imath
{


/**
	Base class for interpolated functions based on fulcrums in multi-dimesional grid.
	This grid may be imagined as set of parallel layers for each dimension.
	Number of this layers and their position can be individually controlled.
*/
template <class Element, int Dimensions>
class TFulcrumGridFunctionBase:
			public istd::TChangesReductorWrap<iser::ISerializable>,
			public TIMathFunction<imath::TVector<Dimensions>, Element>
{
public:
	typedef istd::TChangesReductorWrap<iser::ISerializable> BaseClass;

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
	istd::TIndex<Dimensions> GetGridSize() const;

	/**
		Get number of fulcrum points used in this function.
	*/
	int GetLayersCount(int dimension) const;

	/**
		Set number of fulcrum layers at specified dimension.
		Please note, that this fulcrum velues will be not valid after this operation.
	*/
	void SetLayersCount(int dimension, int count);

	/**
		Inserts single fulcrum layer.
		\return	index of added layer.
	*/
	int InsertLayer(int dimension, double position);

	/**
		Get position of specified layer of fulcrums.
	*/
	double void GetLayerPosition(int dimension, int layerIndex) const;

	/**
		Set position of specified layer of fulcrums.
		Please note, that layer are sorted by its position during update.
		If you want to block update mechanism, use istd::TUpdateNotifier.
	*/
	void SetLayerPosition(int dimension, int layerIndex, double position);

	/**
		Get value at specified index.
	*/
	const Element& GetValueAtIndex(const istd::TIndex<Dimensions>& index) const;

	/**
		Set single fulcrum point.
	*/
	void SetValueAtIndex(const istd::TIndex<Dimensions>& index, const Element& value);

	/**
		Remove fulcrums at spacified layer.
	*/
	void RemoveLayer(int dimension, int layerIndex);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods

	int GetDimensionsCount();

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

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	typedef istd::TArray<Element, Dimensions> Fulcrums;
	Fulcrums m_fulcrums;

	typedef ::std::vector<double> LayerPositions;

	LayerPositions m_layersPositions[Dimensions];
};



// inline methods

template <class Element, int Dimensions>
inline istd::TIndex<Dimensions> TFulcrumGridFunctionBase<Element, Dimensions>::GetGridSize() const
{
	return m_fulcrum.GetSizes();
}


// static inline methods

template <class Element, int Dimensions>
inline int TFulcrumGridFunctionBase<Element, Dimensions>::GetDimensionsCount()
{
	return Dimensions;
}


// public methods

template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::Reset()
{
	m_fulcrums.Reset();

	for (int i = 0; i < Dimensions; ++i){
		m_layersPositions[i].clear();
	}
}


template <class Element, int Dimensions>
int TFulcrumGridFunctionBase<Element, Dimensions>::GetLayersCount(int dimension) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	return m_layersPositions[dimension].size();
}


template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::SetLayersCount(int dimension, int count)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	m_layersPositions[dimension].resize(count);
	m_fulcrums.SetSize(dimension, count);
}


template <class Element, int Dimensions>
int TFulcrumGridFunctionBase<Element, Dimensions>::InsertLayer(int dimension, double position)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	Fulcrums oldFulcrums = m_fulcrums;

	m_fulcrums.SetSize(dimension, m_fulcrums.GetSize(dimension) + 1);

	int layerIndex = FindLayerIndex(dimension, position);

	LayerPositions& positions = m_layersPositions[dimension];

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
	I_ASSERT(oldLayersCount == oldFulcrums.GetSize(dimension));

	positions.insert(positions.begin() + layerIndex + 1, position);

	TVector<Dimensions> sizes = m_fulcrums.GetSizes();
	TVector<Dimensions> destIndex(0);
	if (destIndex.IsInside(sizes)){
		do{
			TVector<Dimensions> sourceIndex = destIndex;
			if ((destIndex[dimension] >= layerIndex) && (sourceIndex[dimension] > 0)){
				sourceIndex[dimension]--;

				if ((destIndex[dimension] == layerIndex) && (layerIndex < oldLayersCount)){
					// calculate interpolated value
					m_fulcrums[destIndex] = prevFactor * oldFulcrums[sourceIndex] + nextFactor * oldFulcrums[destIndex];

					continue;
				}
			}

			m_fulcrums[destIndex] = oldFulcrums[sourceIndex];
		} while (destIndex.Increase(sizes));
	}
}


template <class Element, int Dimensions>
double void TFulcrumGridFunctionBase<Element, Dimensions>::GetLayerPosition(int dimension, int layerIndex) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	LayerPositions& positions = m_layersPositions[dimension];

	I_ASSERT(layerIndex >= 0);
	I_ASSERT(layerIndex < positions.size());

	return positions[layerIndex];
}


template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::SetLayerPosition(int dimension, int layerIndex, double position)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	LayerPositions& positions = m_layersPositions[dimension];

	I_ASSERT(layerIndex >= 0);
	I_ASSERT(layerIndex < positions.size());

	istd::CChangeNotifier notifier(CF_SORT_LAYERS);

	positions[layerIndex] = position;
}


template <class Element, int Dimensions>
const TFulcrumGridFunctionBase<Element, Dimensions>::Element& GetValueAtIndex(const istd::TIndex<Dimensions>& index) const
{
	return m_fulcrum.GetAt(index);
}


template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::SetValueAtIndex(const istd::TIndex<Dimensions>& index, const Element& value)
{
	istd::CChangeNotifier notifier();

	m_fulcrum.SetAt(index, value);
}


template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::RemoveLayer(int dimension, int layerIndex)
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);
	I_ASSERT(m_fulcrums.GetSize(dimension) > 0);

	Fulcrums oldFulcrums = m_fulcrums;

	m_fulcrums.SetSize(dimension, m_fulcrums.GetSize(dimension) + 1);

	LayerPositions& positions = m_layersPositions[dimension];

	positions.erase(positions.begin() + layerIndex);

	TVector<Dimensions> sizes = m_fulcrums.GetSizes();
	TVector<Dimensions> destIndex(0);
	if (destIndex.IsInside(sizes)){
		do{
			TVector<Dimensions> sourceIndex = destIndex;
			if (destIndex[dimension] >= layerIndex){
				sourceIndex[dimension]++;
			}

			m_fulcrums[destIndex] = oldFulcrums[sourceIndex];
		} while (destIndex.Increase(sizes));
	}
}


// reimplemented (iser::ISerializable)

template <class Element, int Dimensions>
bool TFulcrumGridFunctionBase<Element, Dimensions>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	iser::CArchiveTag axesTag("Axes", "List of axes of fulcrum grid");
	iser::CArchiveTag positionsTag("Positions", "List of layer positions of fulcrum grid");
	iser::CArchiveTag positionTag("Position", "Single layer position");
	iser::CArchiveTag fulcrumsTag("Fulcrums", "List of fulcrums");
	iser::CArchiveTag fulcrumTag("Fulcrum", "Single fulcrum");

	bool isStoring = archive.IsStoring();

	istd::TIndex<Dimensions> sizes = m_fulcrum.GetSizes();

	retVal = retVal && archive.BeginTag(axesTag);

	for (int dimensionIndex = 0; dimensionIndex < dimensions; ++dimensionIndex){
		LayerPositions& positions = m_layersPositions[dimensionIndex];

		int positionsCount& = sizes[dimensionIndex];
		I_ASSERT(positionsCount == int(positions.size()));

		retVal = retVal && archive.BeginMultiTag(positionsTag, positionTag, positionsCount);

		if (!isStoring){
			if (!retVal){
				Reset();

				return false;
			}

			positions.resize(positionsCount);
		}

		for (int positionIndex = 0; positionIndex < positionsCount; ++positionIndex){
			retVal = retVal && archive.BeginMultiTag(positionTag);
			retVal = retVal && archive.Process(positions[positionIndex]);
			retVal = retVal && archive.BeginTag(positionTag);
		}

		retVal = retVal && archive.BeginTag(positionsTag);
	}

	retVal = retVal && archive.EndTag(axesTag);

	retVal = retVal && archive.BeginTag(fulcrumsTag);


	if (!isStoring){
		if (!retVal){
			Reset();

			return false;
		}

		m_fulcrums.SetSizes(sizes);
	}

	istd::TIndex<Dimensions> index(0);
	if (index.IsInside(sizes)){
		do{
			Element& point = m_fulcrums[i];

			retVal = retVal && archive.BeginTag(fulcrumTag);
			retVal = retVal && point.Serialize(archive);
			retVal = retVal && archive.EndTag(fulcrumTag);
		} while (index.Increment())
	}

	retVal = retVal && archive.EndTag(fulcrumsTag);

	return retVal;
}


// protected methods

template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::SortFulcrums()
{
	for (int i = 0; i < Dimensions; ++i){
		LayerPositions& positions = m_layersPositions[i];

		::std::sort(positions.begin(), positions.end());
	}
}


template <class Element, int Dimensions>
int TFulcrumGridFunctionBase<Element, Dimensions>::FindIndex(int dimension, double value) const
{
	I_ASSERT(dimension >= 0);
	I_ASSERT(dimension < Dimensions);

	LayerPositions& positions = m_layersPositions[dimension];

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


// reimplemented (istd::IChangeable)

template <class Element, int Dimensions>
void TFulcrumGridFunctionBase<Element, Dimensions>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	if ((changeFlags & CF_SORT_LAYERS) != 0){
		SortFulcrums();
	}
}


} // namespace imath


#endif // !imath_TFulcrumGridFunctionBase_included


