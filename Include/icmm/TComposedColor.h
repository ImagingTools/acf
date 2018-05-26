#ifndef icmm_TComposedColor_included
#define icmm_TComposedColor_included


// STL includes
#include <cstring>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>
#include <imath/IDoubleManip.h>
#include <imath/TVector.h>
#include <imath/CVarVector.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Generic color implementation.
*/
template <int Size>
class TComposedColor: public imath::TVector<Size>, virtual public iser::ISerializable
{
public:
	typedef imath::TVector<Size> BaseClass;
	typedef typename BaseClass::Elements Elements;

	TComposedColor();
	TComposedColor(const BaseClass& value);
	explicit TComposedColor(const imath::CVarVector& vector);

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const TComposedColor<Size>& color, double tolerance = I_BIG_EPSILON) const;

	/**
		Check if this color value is normalized.
		Normalized values have all components in the range [0, 1].
	*/
	bool IsNormalized() const;

	/**
		Make this color to be normalized.
		Normalized values have all components in the range [0, 1].
	*/
	void Normalize();

	/**
		Get normalized color.
		Normalized values have all components in the range [0, 1].
	*/
	void GetNormalized(TComposedColor<Size>& result) const;

	TComposedColor<Size> operator+(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator-(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator*(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator/(const TComposedColor<Size>& color) const;

	TComposedColor<Size> operator*(double value) const;
	TComposedColor<Size> operator/(double value) const;

	const TComposedColor<Size>& operator=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator=(double colorValue);

	const TComposedColor<Size>& operator+=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator-=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator*=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator/=(const TComposedColor<Size>& color);

	const TComposedColor<Size>& operator*=(double value);
	const TComposedColor<Size>& operator/=(double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


// inline methods

template <int Size>
inline TComposedColor<Size>::TComposedColor()
	:BaseClass()
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const BaseClass& value)
:	BaseClass(value)
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const imath::CVarVector& vector)
{
	int commonSize = qMin(vector.GetElementsCount(), Size);
	for (int copyIndex = 0; copyIndex < commonSize; ++copyIndex){
		BaseClass::SetElement(copyIndex, vector[copyIndex]);
	}

	for (int resetIndex = commonSize; resetIndex < Size; ++resetIndex){
		BaseClass::SetElement(resetIndex, 0.0);
	}
}


template <int Size>
inline bool TComposedColor<Size>::IsSimilar(const TComposedColor<Size>& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


template <int Size>
inline bool TComposedColor<Size>::IsNormalized() const
{
	for (int i = 0; i < Size; ++i){
		double component = BaseClass::GetElement(i);

		if ((component < 0) || (component > 1)){
			return false;
		}
	}

	return true;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator+(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) + color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator-(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) - color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) * color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) / color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) * value;
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) / value;
	}

	return retVal;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(const TComposedColor<Size>& color)
{
	for (int i = 0; i < Size; ++i){
		BaseClass::SetElement(i, color[i]);
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(double colorValue)
{
	std::memset(&BaseClass::GetElementsRef()[0], 0.0, sizeof(double) * Size);

	BaseClass::SetElement(0, colorValue);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator+=(const TComposedColor<Size>& color)
{
	BaseClass::operator+=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator-=(const TComposedColor<Size>& color)
{
	BaseClass::operator-=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(const TComposedColor<Size>& color)
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int index = 0; index < Size; ++index){
		elements[index] *= color[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(const TComposedColor<Size>& color)
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int index = 0; index < Size; ++index){
		elements[index] /= color[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


// public normal methods

template <int Size>
void TComposedColor<Size>::GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result)
{
	for (int i = 0; i < Size; ++i){
		result[i] = manipulator.GetRounded(BaseClass::GetElement(i));
	}
}


template <int Size>
bool TComposedColor<Size>::IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const
{
	for (int i = 0; i < Size; ++i){
		if (!manipulator.IsEqual(BaseClass::GetElement(i), color[i])){
			return false;
		}
	}

	return true;
}


template <int Size>
void TComposedColor<Size>::Normalize()
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int i = 0; i < Size; ++i){
		if (elements[i] < 0.0){
			elements[i] = 0.0;
		}

		if (elements[i] > 1.0){
			elements[i] = 1.0;
		}
	}
}


template <int Size>
void TComposedColor<Size>::GetNormalized(TComposedColor<Size>& result) const
{
	result = *this;

	result.Normalize();
}


// reimplemented (iser::ISerializable)

template <int Size>
bool TComposedColor<Size>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag colorComponentsTag("ColorComponents", "List of color components", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag componentTag("Component", "Single component", iser::CArchiveTag::TT_LEAF, &colorComponentsTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	int elementsCount = BaseClass::GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(colorComponentsTag, componentTag, elementsCount);

	if ((!retVal) || (!archive.IsStoring() && (elementsCount != Size))){
		return false;
	}

	for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(componentTag);
		retVal = retVal && archive.Process(BaseClass::GetElementRef(i));
		retVal = retVal && archive.EndTag(componentTag);
	}

	retVal = retVal && archive.EndTag(colorComponentsTag);

	return retVal;
}


// related global functions

template <int Size>
uint qHash(const TComposedColor<Size>& color, uint seed = 0)
{
	quint64 retVal = seed;

	union{
		double value;
		quint64 raw;
	} element;
	element.raw = 0;

	int elementsCount = color.GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		element.value = color[i];

		retVal = (retVal >> 1) ^ (element.raw + 1);
	}

	return uint(retVal);
}


} // namespace icmm


#endif // !icmm_TComposedColor_included


