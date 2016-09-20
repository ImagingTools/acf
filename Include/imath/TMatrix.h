#ifndef imath_TMatrix_included
#define imath_TMatrix_included


#include <istd/TArray.h>
#include <istd/CIndex2d.h>

#include <iser/CArchiveTag.h>

#include <imath/TVector.h>


namespace iser
{
	class IArchive;
}


namespace imath
{


/**
	Definition of mathematical matrix with fixed dimensions.
*/
template <int Width, int Height, typename Element = double>
class TMatrix
{
public:
	typedef istd::CIndex2d IndexType;
	typedef istd::CIndex2d SizesType;
	typedef Element ElementType;
	typedef imath::TVector<Height, Element> ColumnVector;
	typedef imath::TVector<Width, Element> RowVector;

	enum MatrixInitMode
	{
		/**
			All elements initialized to 0.
		*/
		MIM_ZERO,
		/**
			All elements initialized to 1.
		*/
		MIM_ONES,
		/**
			Identity matrix.
		*/
		MIM_IDENTITY,
		/**
			Upper triangle matrix.
		*/
		MIM_UPPER_TRIANGLE,
		/**
			Downer triangle matrix.
		*/
		MIM_DOWNER_TRIANGLE
	};

	/**
		Create matrix without initialization.
	*/
	TMatrix();

	/**
		Create matrix with initialization to some specified one.
	*/
	TMatrix(MatrixInitMode mode);

	/**
		Copy constructor.
	*/
	TMatrix(const TMatrix& matrix);

	/**
		Set all matrix cells to zero.
		It makes the same as Clear() and is used for compatibility with other matrix implementations.
	*/
	void Reset();

	/**
		Set all matrix cells to zero.
		It makes the same as Reset() and is used for compatibility with other matrix implementations.
	*/
	void Clear();

	/**
		Check, if number dimensions is fixed.
		It is provided for template implementations. It returns always true.
	*/
	bool IsDimensionsCountFixed() const;

	/**
		Get number of dimensions of this array.
		It is provided for template implementations. It returns always 2.
	*/
	int GetDimensionsCount() const;

	/**
		Set number of dimensions of this array.
		This is only dummy method, to provide methods compatibility with template implementations.
		\param	count	number of dimensions will be set.
		\return			true, if number of set dimensions equals 2, or false if isn't.
	*/
	bool SetDimensionsCount(int count);

	/**
		Get list of all sizes.
	*/
	const SizesType& GetSizes() const;

	/**
		Set list of all sizes.
		This is only dummy method, to provide methods compatibility with template implementations.
		\return			true, if set dimensions equals the template parameters, or false if isn't.
	*/
	bool SetSizes(const SizesType& sizes);

	/**
		Get size of array for specified dimension.
	*/
	int GetSize(int dimension) const;

	/**
		Set size of array for specified dimension.
		This is only dummy method, to provide methods compatibility with template implementations.
		\return			true, if set dimension equals the template parameter, or false if isn't.
	*/
	bool SetSize(int dimension, int size);

	/**
		Get element stored at specified index.
	*/
	const ElementType& GetAt(const IndexType& index) const;

	/**
		Get element stored at specified index.
	*/
	const ElementType& GetAt(int x, int y) const;

	/**
		Get reference to element stored at specified index.
	*/
	ElementType& GetAtRef(const IndexType& index);

	/**
		Get reference to element stored at specified index.
	*/
	ElementType& GetAtRef(int x, int y);

	/**
		Set element at specified index.
	*/
	void SetAt(const IndexType& index, const ElementType& value);

	/**
		Set element at specified index.
	*/
	void SetAt(int x, int y, const ElementType& value);

	/**
		Create identity matrix.
	*/
	void InitToIdentity();

	/*
		Get maximal element.
	*/
	double GetMaxElement() const;

	/*
		Get minimal element.
	*/
	double GetMinElement() const;

	/**
		Get result matrix with negated all elements.
	*/
	void GetNegated(TMatrix<Width, Height, Element>& result);

	/**
		Get sum of two matrices.
	*/
	void GetAdded(const TMatrix<Width, Height, Element>& matrix, TMatrix<Width, Height, Element>& result) const;

	/**
		Get result of substraction of two matrices.
	*/
	void GetSubstracted(const TMatrix<Width, Height, Element>& matrix, TMatrix<Width, Height, Element>& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	template <int SecondWidth>
	void GetMultiplied(const TMatrix<SecondWidth, Width, Element>& matrix, TMatrix<SecondWidth, Height, Element>& result) const
	{
		for (int resultY = 0; resultY < Height; ++resultY){
			for (int resultX = 0; resultX < SecondWidth; ++resultX){
				double sum = 0;
				for (int i = 0; i < Width; ++i){
					sum += m_elements[i][resultY] * matrix.m_elements[resultX][i];
				}

				result.m_elements[resultX][resultY] = sum;
			}
		}
	}

	/**
		Get result of multiplication of two matrices.
	*/
	template <int SecondWidth>
	TMatrix<SecondWidth, Height, Element> GetMultiplied(const TMatrix<SecondWidth, Width, Element>& matrix) const
	{
		TMatrix<SecondWidth, Height, Element> retVal;

		GetMultiplied(matrix, retVal);

		return retVal;
	}

	/**
		Get result of multiplication of this matrix and some vector.
	*/
	void GetMultiplied(const TVector<Width, Element>& vector, TVector<Height, Element>& result) const;

	/**
		Get result of multiplication of this matrix and some vector.
	*/
	TVector<Height, Element> GetMultiplied(const TVector<Width, Element>& vector) const;

	/**
		Get result of multiplication of this matrix with scalar value.
	*/
	void GetScaled(double value, TMatrix<Width, Height, Element>& result) const;

	/**
		Get transposed matrix.
	*/
	void GetTransposed(TMatrix<Height, Width, Element>& result) const;

	/**
		Get transposed matrix.
	*/
	TMatrix<Height, Width, Element> GetTransposed() const;

	/**
		Transpose matrix.
	*/
	void Transpose();

	/*
		Get square of euclidean norm.
	*/
	double GetFrobeniusNorm2() const;

	/*
		Get euclidean norm.
	*/
	double GetFrobeniusNorm() const;

	/**
		Transform matrix to upper triangle form using method of Householder reflexions.
		\param	result		triangle matrix.
		\param	matrix2Ptr	optional matrix will be transformed using the same reflexions.
							To realize QR decomposition you can use identity matrix.
		\param	maxColumns	optional maximal number of transformed columns. If it is negative value whole matrix will be transformed.
		\param	minHhNorm	minimal Hausholder reflexion vector length. If any reflexion vector is shorter this method fails.
		\return				true if success.
	*/
	bool GetTriangleDecomposed(
				TMatrix<Width, Height, Element>& result,
				TMatrix<Height, Height, Element>* matrixQPtr = NULL,
				int maxColumns = -1,
				double minHhNorm = I_BIG_EPSILON) const;

	/**
		Get single column as vector.
	*/
	void GetColumnVector(int columnIndex, TVector<Height, Element>& result);

	/**
		Set a single column vector to matrix.
	*/
	void SetColumnVector(int columnIndex, const TVector<Height, Element>& columnVector);

	/**
		Get single row as vector.
	*/
	void GetRowVector(int rowIndex, TVector<Width, Element>& result);

	bool Serialize(iser::IArchive& archive);

	// operators
	TMatrix<Width, Height, Element> operator+(const TMatrix<Width, Height, Element>& matrix) const;
	TMatrix<Width, Height, Element> operator-(const TMatrix<Width, Height, Element>& matrix) const;
	TMatrix<Width, Height, Element> operator-();

	template <int SecondWidth>
	TMatrix<SecondWidth, Height, Element> operator*(const TMatrix<SecondWidth, Width, Element>& matrix) const
	{
		TMatrix<SecondWidth, Height, Element> retVal;

		GetMultiplied(matrix, retVal);

		return retVal;
	}

	TMatrix<Width, Height, Element> operator*(double value) const;

	TMatrix<Width, Height, Element>& operator+=(const TMatrix<Width, Height, Element>& matrix);
	TMatrix<Width, Height, Element>& operator-=(const TMatrix<Width, Height, Element>& matrix);
	TMatrix<Width, Height, Element>& operator*=(double value);

	bool operator==(const TMatrix<Width, Height, Element>& matrix) const;
	bool operator!=(const TMatrix<Width, Height, Element>& matrix) const;

	const ElementType& operator[](const IndexType& index) const;
	ElementType& operator[](const IndexType& index);

private:
	typedef Element Elements[Width][Height];

	Elements m_elements;
};


// inline methods

template <int Width, int Height, typename Element>
inline TMatrix<Height, Width, Element> TMatrix<Width, Height, Element>::GetTransposed() const
{
	TMatrix<Height, Width, Element> retVal;

	GetTransposed(retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
inline void TMatrix<Width, Height, Element>::Transpose()
{
	TMatrix<Height, Width, Element> retVal;

	GetTransposed(retVal);

	*this = retVal;
}


template <int Width, int Height, typename Element>
TVector<Height, Element> TMatrix<Width, Height, Element>::GetMultiplied(const TVector<Width, Element>& vector) const
{
	TVector<Height, Element> retVal;

	GetMultiplied(vector, retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
inline TMatrix<Width, Height, Element> TMatrix<Width, Height, Element>::operator+(const TMatrix<Width, Height, Element>& matrix) const
{
	TMatrix<Width, Height, Element> retVal;

	GetAdded(matrix, retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
inline TMatrix<Width, Height, Element> TMatrix<Width, Height, Element>::operator-(const TMatrix<Width, Height, Element>& matrix) const
{
	TMatrix<Width, Height, Element> retVal;

	GetSubstracted(matrix, retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
inline TMatrix<Width, Height, Element> TMatrix<Width, Height, Element>::operator-()
{
	TMatrix<Width, Height, Element> retVal;

	GetNegated(retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
inline TMatrix<Width, Height, Element> TMatrix<Width, Height, Element>::operator*(double value) const
{
	TMatrix retVal;

	GetScaled(value, retVal);

	return retVal;
}


template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>& TMatrix<Width, Height, Element>::operator+=(const TMatrix<Width, Height, Element>& matrix)
{
	GetAdded(matrix, *this);

	return *this;
}


template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>& TMatrix<Width, Height, Element>::operator-=(const TMatrix<Width, Height, Element>& matrix)
{
	GetSubstracted(matrix, *this);

	return *this;
}


template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>& TMatrix<Width, Height, Element>::operator*=(double value)
{
	GetScaled(value, *this);

	return *this;
}


template <int Width, int Height, typename Element>
inline TMatrix<Width, Height, Element> operator*(double value, const imath::TMatrix<Width, Height, Element>& matrix)
{
	return matrix * value;
}


// public methods

template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>::TMatrix()
{
}


template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>::TMatrix(MatrixInitMode mode)
{
	switch (mode){
	case MIM_ZERO:
		for (int x = 0; x < Width; ++x){
			for (int y = 0; y < Height; ++y){
				m_elements[x][y] = 0;
			}
		}
		break;

	case MIM_ONES:
		for (int x = 0; x < Width; ++x){
			for (int y = 0; y < Height; ++y){
				m_elements[x][y] = 1;
			}
		}
		break;

	case MIM_IDENTITY:
		for (int x = 0; x < Width; ++x){
			for (int y = 0; y < Height; ++y){
				if (x == y){
					m_elements[x][y] = 1;
				}
				else{
					m_elements[x][y] = 0;
				}
			}
		}
		break;

	case MIM_UPPER_TRIANGLE:
		for (int x = 0; x < Width; ++x){
			for (int y = 0; y < Height; ++y){
				if (x <= y){
					m_elements[x][y] = 1;
				}
				else{
					m_elements[x][y] = 0;
				}
			}
		}
		break;

	case MIM_DOWNER_TRIANGLE:
		for (int x = 0; x < Width; ++x){
			for (int y = 0; y < Height; ++y){
				if (x >= y){
					m_elements[x][y] = 1;
				}
				else{
					m_elements[x][y] = 0;
				}
			}
		}
		break;
	}
}


template <int Width, int Height, typename Element>
TMatrix<Width, Height, Element>::TMatrix(const TMatrix& matrix)
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			m_elements[x][y] = matrix.m_elements[x][y];
		}
	}
}


// init operations

template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::Reset()
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			m_elements[x][y] = 0;
		}
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::Clear()
{
	Reset();
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::IsDimensionsCountFixed() const
{
	return true;
}


template <int Width, int Height, typename Element>
int TMatrix<Width, Height, Element>::GetDimensionsCount() const
{
	return 2;
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::SetDimensionsCount(int count)
{
	return (count == 2);
}


template <int Width, int Height, typename Element>
const typename TMatrix<Width, Height, Element>::SizesType& TMatrix<Width, Height, Element>::GetSizes() const
{
	static SizesType retVal(Width, Height);

	return retVal;
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::SetSizes(const SizesType& sizes)
{
	return (sizes[0] == Width) && (sizes[1] == Height);
}


template <int Width, int Height, typename Element>
int TMatrix<Width, Height, Element>::GetSize(int dimension) const
{
	switch (dimension){
	case 0:
		return Width;

	case 1:
		return Height;

	default:
		return 0;
	}
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::SetSize(int dimension, int size)
{
	switch (dimension){
	case 0:
		return (size == Width);

	case 1:
		return (size == Height);

	default:
		return false;
	}
}


template <int Width, int Height, typename Element>
const typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::GetAt(const IndexType& index) const
{
	return m_elements[index[0]][index[1]];
}


template <int Width, int Height, typename Element>
const typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::GetAt(int x, int y) const
{
	return m_elements[x][y];
}


template <int Width, int Height, typename Element>
typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::GetAtRef(const IndexType& index)
{
	return m_elements[index[0]][index[1]];
}


template <int Width, int Height, typename Element>
typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::GetAtRef(int x, int y)
{
	return m_elements[x][y];
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::SetAt(const IndexType& index, const ElementType& value)
{
	m_elements[index[0]][index[1]] = value;
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::SetAt(int x, int y, const ElementType& value)
{
	m_elements[x][y] = value;
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::InitToIdentity()
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			if (x != y){
				m_elements[x][y] = 0;
			}
			else{
				m_elements[x][y] = 1;
			}
		}
	}
}


template <int Width, int Height, typename Element>
double TMatrix<Width, Height, Element>::GetMaxElement() const
{
	double retVal = m_elements[0][0];

	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			double value = m_elements[x][y];

			if (value > retVal){
				retVal = value;
			}
		}
	}

	return retVal;
}


template <int Width, int Height, typename Element>
double TMatrix<Width, Height, Element>::GetMinElement() const
{
	double retVal = m_elements[0][0];

	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			double value = m_elements[x][y];

			if (value < retVal){
				retVal = value;
			}
		}
	}

	return retVal;
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetNegated(TMatrix<Width, Height, Element>& result)
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			result.m_elements[x][y] = -m_elements[x][y];
		}
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetAdded(const TMatrix<Width, Height, Element>& matrix, TMatrix<Width, Height, Element>& result) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			result.m_elements[x][y] = m_elements[x][y] + matrix.m_elements[x][y];
		}
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetSubstracted(const TMatrix<Width, Height, Element>& matrix, TMatrix<Width, Height, Element>& result) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			result.m_elements[x][y] = m_elements[x][y] - matrix.m_elements[x][y];
		}
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetMultiplied(const TVector<Width, Element>& vector, TVector<Height, Element>& result) const
{
	for (int resultY = 0; resultY < Height; ++resultY){
		double sum = 0;
		for (int i = 0; i < Width; ++i){
			sum += m_elements[i][resultY] * vector[i];
		}

		result[resultY] = sum;
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetScaled(double value, TMatrix<Width, Height, Element>& result) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			result.m_elements[x][y] = m_elements[x][y] * value;
		}
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetTransposed(TMatrix<Height, Width, Element>& result) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			result.m_elements[y][x] = m_elements[x][y];
		}
	}
}


template <int Width, int Height, typename Element>
double TMatrix<Width, Height, Element>::GetFrobeniusNorm2() const
{
	double retVal = 0.0;

	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			double value = m_elements[x][y];
			retVal += double(value * value);
		}
	}
	return retVal;
}


template <int Width, int Height, typename Element>
double TMatrix<Width, Height, Element>::GetFrobeniusNorm() const
{
	return qSqrt(GetFrobeniusNorm2());
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetColumnVector(int columnIndex, TVector<Height, Element>& result)
{
	Q_ASSERT(columnIndex < Width);
	Q_ASSERT(columnIndex >= 0);

	for (int y = 0; y < Height; ++y){
		result[y] = m_elements[columnIndex][y];
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::SetColumnVector(int columnIndex, const TVector<Height, Element>& columnVector)
{
	Q_ASSERT(columnIndex < Width);
	Q_ASSERT(columnIndex >= 0);

	for (int y = 0; y < Height; ++y){
		m_elements[columnIndex][y] = columnVector[y];
	}
}


template <int Width, int Height, typename Element>
void TMatrix<Width, Height, Element>::GetRowVector(int rowIndex, TVector<Width, Element>& result)
{
	Q_ASSERT(rowIndex < Height);

	for (int x = 0; x < Width; ++x){
		result[rowIndex] = m_elements[x][rowIndex];
	}
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::GetTriangleDecomposed(
			TMatrix<Width, Height, Element>& result,
			TMatrix<Height, Height, Element>* matrixQPtr,
			int maxColumns,
			double minHhNorm) const
{
	if (&result != this){
		result = *this;
	}

	int columnsCount = qMin(Height - 1, Width);
	if ((maxColumns >= 0) && (maxColumns < columnsCount)){
		columnsCount = maxColumns;
	}

	for (int hhIndex = 0; hhIndex < columnsCount; ++hhIndex){
		double hhNorm2 = 0;
		for (istd::CIndex2d normIndex(hhIndex, hhIndex); normIndex[1] < Height; ++normIndex[1]){
			double element = result.GetAt(normIndex);

			hhNorm2 += element * element;
		}

		if (hhNorm2 < minHhNorm){
			return false;
		}

		double element0 = result[istd::CIndex2d(hhIndex, hhIndex)];

		double hhLength = (element0 >= 0)? qSqrt(hhNorm2): -qSqrt(hhNorm2);	// destination diagonal value of current processed column, sign is choosen optimal for maximal householder vector length

		double hhVector0 = element0 + hhLength;	// element 0 of householder vector, rest of ist will be taken directly from matrix elements
		double hhVectorNorm2 = hhNorm2 + hhVector0 * hhVector0 - element0 * element0;	// sqare of norm of householder vector, first element replaced from original column norm sqare

		// correct next columns
		istd::CIndex2d matrixIndex;
		for (matrixIndex[0] = hhIndex + 1; matrixIndex[0] < Width; ++matrixIndex[0]){
			matrixIndex[1] = hhIndex;
			double dotProduct = result[matrixIndex] * hhVector0;
			for (matrixIndex[1]++; matrixIndex[1] < Height; ++matrixIndex[1]){
				dotProduct += result.GetAt(matrixIndex) * result.GetAt(istd::CIndex2d(hhIndex, matrixIndex[1]));
			}

			double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
			matrixIndex[1] = hhIndex;
			result[matrixIndex] -= hhVector0 * reflexionFactor;
			for (matrixIndex[1]++; matrixIndex[1] < Height; ++matrixIndex[1]){
				result.GetAtRef(matrixIndex) -= result[istd::CIndex2d(hhIndex, matrixIndex[1])] * reflexionFactor;
			}
		}

		// correct optional matrix
		if (matrixQPtr != NULL){
			istd::CIndex2d matrix2Index;
			for (matrix2Index[0] = 0; matrix2Index[0] < Height; ++matrix2Index[0]){
				matrix2Index[1] = hhIndex;
				double dotProduct = matrixQPtr->GetAt(matrix2Index) * hhVector0;
				for (++matrix2Index[1]; matrix2Index[1] < Height; ++matrix2Index[1]){
					dotProduct += matrixQPtr->GetAt(matrix2Index) * result[istd::CIndex2d(hhIndex, matrix2Index[1])];
				}

				double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
				matrix2Index[1] = hhIndex;
				matrixQPtr->GetAtRef(matrix2Index) -= hhVector0 * reflexionFactor;
				for (matrix2Index[1]++; matrix2Index[1] < Height; ++matrix2Index[1]){
					matrixQPtr->GetAtRef(matrix2Index) -= result.GetAt(istd::CIndex2d(hhIndex, matrix2Index[1])) * reflexionFactor;
				}
			}
		}

		// correct current column
		result.SetAt(istd::CIndex2d(hhIndex, hhIndex), -hhLength);
		for (int i = hhIndex + 1; i < Height; ++i){
			result.SetAt(istd::CIndex2d(hhIndex, i), 0);
		}
	}

	return true;
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag sizeTag("Size", "Size of matrix", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag cellsTag("Cells", "Values of matrix cells", iser::CArchiveTag::TT_GROUP);

	retVal = retVal && archive.BeginTag(sizeTag);
	if (archive.IsStoring()){
		int width = Width;
		int height = Height;

		retVal = retVal && archive.Process(width);
		retVal = retVal && archive.Process(height);
	}
	else{
		int width = 0;
		int height = 0;

		retVal = retVal && archive.Process(width);
		retVal = retVal && archive.Process(height);

		if (!retVal || (width != Width) || (height != Height)){
			return false;
		}
	}

	retVal = retVal && archive.EndTag(sizeTag);

	retVal = retVal && archive.BeginTag(cellsTag);
	for (int y = 0; y < Height; ++y){
		for (int x = 0; x < Width; ++x){
			retVal = retVal && archive.Process(m_elements[x][y]);
		}
	}
	retVal = retVal && archive.EndTag(cellsTag);

	return retVal;
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::operator==(const TMatrix<Width, Height, Element>& matrix) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			if (m_elements[x][y] != matrix.m_elements[x][y]){
				return false;
			}
		}
	}

	return true;
}


template <int Width, int Height, typename Element>
bool TMatrix<Width, Height, Element>::operator!=(const TMatrix<Width, Height, Element>& matrix) const
{
	for (int x = 0; x < Width; ++x){
		for (int y = 0; y < Height; ++y){
			if (m_elements[x][y] != matrix.m_elements[x][y]){
				return true;
			}
		}
	}

	return false;
}


template <int Width, int Height, typename Element>
const typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::operator[](const IndexType& index) const
{
	return m_elements[index[0]][index[1]];
}


template <int Width, int Height, typename Element>
typename TMatrix<Width, Height, Element>::ElementType& TMatrix<Width, Height, Element>::operator[](const IndexType& index)
{
	return m_elements[index[0]][index[1]];
}


} // namespace imath


#endif // !imath_TMatrix_included


