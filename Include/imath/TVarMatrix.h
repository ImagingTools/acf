#ifndef imath_TVarMatrix_included
#define imath_TVarMatrix_included


// STL includes
#include <cmath>

#include "istd/TArray.h"
#include "istd/CIndex2d.h"

#include "imath/TVarVector.h"


namespace imath
{


/**
	Definition of mathematical matrix.
*/
template <class Element>
class TVarMatrix: public istd::TArray<Element, 2>
{
public:
	/**
		Create empty matrix.
	*/
	TVarMatrix<Element>();

	/**
		Copy constructor.
	*/
	TVarMatrix<Element>(const TVarMatrix<Element>& matrix);

	/**
		Create matrix with specified size.
	*/
	TVarMatrix<Element>(istd::CIndex2d size);

	/**
		Create matrix from vector.
	*/
	TVarMatrix<Element>(const TVarVector<Element>& vector, bool isTransposed = false);

	/**
		Set all matrix cells to zero.
	*/
	virtual void Clear();

	/**
		Create identity matrix.
	*/
	virtual void InitToIdentity(int size);

	/*
		Get maximal element.
	*/
	Element GetMaxElement() const;

	/*
		Get minimal element.
	*/
	Element GetMinElement() const;

	/**
		Get result matrix with negated all elements.
	*/
	void GetNegated(TVarMatrix<Element>& result);

	/**
		Get sum of two matrices.
	*/
	void GetAdded(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const;

	/**
		Get result of substraction of two matrices.
	*/
	void GetSubstracted(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	void GetMultiplied(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const;

	/**
		Get result of multiplication of this matrix with scalar value.
	*/
	void GetScaled(Element value, TVarMatrix<Element>& result) const;

	/**
		Get transposed matrix.
	*/
	void GetTransposed(TVarMatrix<Element>& result) const;

	/**
		Get transposed matrix.
	*/
	TVarMatrix<Element> GetTransposed() const;

	/**
		Transpose matrix.
	*/
	void Transpose();

	/*
		Get square of euclidean norm.
	*/
	double GetEuclideanNorm2() const;

	/*
		Get euclidean norm.
	*/
	double GetEuclideanNorm() const;

	/**
		Transform matrix to upper triangle form using method of Householder reflexions.
		\param	result		triangle matrix.
		\param	matrix2Ptr	optional source matrix will be transformed using the same reflexions.
							To realize QR decomposition you can use identity matrix.
		\param	result2Ptr	optional result object storing transformation of matrix2Ptr.
		\param	maxColumns	optional maximal number of transformed columns. If it is negative value whole matrix will be transformed.
		\param	minHhNorm	minimal Hausholder reflexion vector length. If any reflexion vector is shorter this method fails.
		\return				true if success.
	*/
	bool GetTriangleDecomposed(
				TVarMatrix<Element>& result,
				const TVarMatrix<Element>* matrix2Ptr = NULL,
				TVarMatrix<Element>* result2Ptr = NULL,
				int maxColumns = -1,
				Element minHhNorm = I_BIG_EPSILON) const;

	/**
		Transform this matrix in place.
	*/
	bool TransformR(int firstPartWidth);

	/**
		Solving of linear system with triangle matrix.
		Rx = y, result = x.
		\return	true if linear equation system was solved.
	*/
	bool GetSolvedTriangle(const TVarMatrix<Element>& vector, TVarMatrix<Element>& result, Element accuracy = I_BIG_EPSILON) const;

	/**
		Solve 'Least Square Problem'.
		Solve linear Least Square Problem for equation Ax = y, where A is a N * M matrix, N >= M.
	 */
	bool GetSolvedLSP(const TVarMatrix<Element>& vector, TVarMatrix<Element>& result, Element minHhNorm = I_BIG_EPSILON) const;

	/**
		Get single column as vector.
	*/
	void GetColumnVector(int columnIndex, TVarVector<Element>& result);
	/**
		Get single row as vector.
	*/
	void GetRowVector(int rowIndex, TVarVector<Element>& result);

	bool Serialize(iser::IArchive& archive);

	// operators
	TVarMatrix<Element> operator+(const TVarMatrix<Element>& b) const;
	TVarMatrix<Element> operator-(const TVarMatrix<Element>& b) const;
	TVarMatrix<Element> operator-();
	TVarMatrix<Element> operator*(const TVarMatrix<Element>& b) const;
	TVarMatrix<Element> operator*(Element value) const;

	bool operator==(const TVarMatrix<Element>& matrix) const;
	bool operator!=(const TVarMatrix<Element>& matrix) const;
};


// inline methods

template <class Element>
inline void TVarMatrix<Element>::Transpose()
{
    TVarMatrix<Element> result;

	GetTransposed(result);

	*this=result;
}


template <class Element>
inline TVarMatrix<Element> TVarMatrix<Element>::operator+(const TVarMatrix<Element>& matrix) const
{
    TVarMatrix<Element> result;

	GetAdded(matrix, result);

	return result;
}


template <class Element>
inline TVarMatrix<Element> TVarMatrix<Element>::operator-(const TVarMatrix<Element>& matrix) const
{
    TVarMatrix<Element> result;

    GetSubstracted(matrix, result);

	return result;
}


template <class Element>
inline TVarMatrix<Element> TVarMatrix<Element>::operator-()
{
	TVarMatrix<Element> result;

	GetNegated(result);

	return result;
}


template <class Element>
inline TVarMatrix<Element> TVarMatrix<Element>::operator*(const TVarMatrix<Element>& matrix) const
{
    TVarMatrix<Element> result;

    GetMultiplied(matrix, result);

	return result;
}


template <class Element>
inline TVarMatrix<Element> TVarMatrix<Element>::operator*(Element value) const
{
    TVarMatrix<Element> result;

    GetScaled(value, result);

	return result;
}


template <class Element>
inline TVarMatrix<Element> operator*(Element value, const imath::TVarMatrix<Element>& matrix)
{
    return matrix * value;
}


// public methods

template <class Element>
TVarMatrix<Element>::TVarMatrix()
{
}


template <class Element>
TVarMatrix<Element>::TVarMatrix(const TVarMatrix<Element>& matrix)
:	BaseClass(matrix)
{
}


template <class Element>
TVarMatrix<Element>::TVarMatrix(istd::CIndex2d size)
{
	SetSizes(size);
}


template <class Element>
TVarMatrix<Element>::TVarMatrix(const TVarVector<Element>& vector, bool isTransposed)
{
	if (isTransposed){
		DeepCopy(vector.GetElements(), istd::CIndex2d(vector.GetElementsCount(), 1));
	}
	else{
		DeepCopy(vector.GetElements(), istd::CIndex2d(1, vector.GetElementsCount()));
	}
}


// init operations

template <class Element>
void TVarMatrix<Element>::Clear()
{
	istd::CIndex2d size = GetSizes();

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			SetAt(index, 0);
		}
    }
}


template <class Element>
void TVarMatrix<Element>::InitToIdentity(int size)
{
    SetSizes(istd::CIndex2d(size, size));

	Clear();

	for (int index = 0; index < size; ++index){
        SetAt(istd::CIndex2d(index, index), 1);
	}
}


template <class Element>
Element TVarMatrix<Element>::GetMaxElement() const
{
	if (IsEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	Element retVal = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			const Element& value = GetAt(index);

			if (value > retVal){
				retVal = value;
			}
		}
    }

	return retVal;
}


template <class Element>
Element TVarMatrix<Element>::GetMinElement() const
{
	if (IsEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	Element retVal = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			const Element& value = GetAt(index);

			if (value < retVal){
				retVal = value;
			}
		}
    }

	return retVal;
}


template <class Element>
void TVarMatrix<Element>::GetNegated(TVarMatrix<Element>& result)
{
	istd::CIndex2d size = GetSizes();

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, -GetAt(index));
		}
	}
}


template <class Element>
void TVarMatrix<Element>::GetAdded(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(size == matrix.GetSizes());

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, GetAt(index) + matrix.GetAt(index));
		}
	}
}


template <class Element>
void TVarMatrix<Element>::GetSubstracted(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(size == matrix.GetSizes());

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, GetAt(index) - matrix.GetAt(index));
		}
	}
}


template <class Element>
void TVarMatrix<Element>::GetMultiplied(const TVarMatrix<Element>& matrix, TVarMatrix<Element>& result) const
{
	istd::CIndex2d matrixSize = matrix.GetSizes();
	istd::CIndex2d size = GetSizes();
    I_ASSERT(size[0] == matrixSize[1]);

    Element sum;

    result.SetSizes(istd::CIndex2d(matrixSize[0], size[1]));

	istd::CIndex2d index;
	istd::CIndex2d matrixIndex;
	istd::CIndex2d resultIndex;

    for (resultIndex[1] = 0; resultIndex[1] < size[1]; ++resultIndex[1]){
		index[1] = resultIndex[1];

		for (resultIndex[0] = 0; resultIndex[0] < matrixSize[0]; ++resultIndex[0]){
			matrixIndex[0] = resultIndex[0];

			for (index[0] = 0; index[0] < size[0]; ++index[0]){
				matrixIndex[1] = index[0];

				sum += GetAt(index) * matrix.GetAt(matrixIndex);
            }

			result.SetAt(resultIndex, sum);
        }
    }
}


template <class Element>
void TVarMatrix<Element>::GetScaled(Element value, TVarMatrix<Element>& result) const
{
	istd::CIndex2d size = GetSizes();

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, GetAt(index) * value);
		}
    }
}


template <class Element>
void TVarMatrix<Element>::GetTransposed(TVarMatrix<Element>& result) const
{
	istd::CIndex2d size = GetSizes();

	result.SetSizes(istd::CIndex2d(size[1], size[0]));

	istd::CIndex2d index;
	istd::CIndex2d transposedIndex;
	for (index[0] = 0; index[0] < size[0]; ++index[0]){
		transposedIndex[1] = index[0];
		for (index[1] = 0; index[1] < size[1]; ++index[1]){
			transposedIndex[0] = index[1];

			result.SetAt(transposedIndex, GetAt(index));
        }
    }
}


template <class Element>
double TVarMatrix<Element>::GetEuclideanNorm2() const
{
	double retVal = 0.0;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			const Element& value = GetAt(index);
			result += double(value * value);
		}
    }
	return retVal;
}


template <class Element>
double TVarMatrix<Element>::GetEuclideanNorm() const
{
	return std::sqrt(GetEuclideanNorm2());
}


template <class Element>
bool TVarMatrix<Element>::GetTriangleDecomposed(
				TVarMatrix<Element>& result,
				const TVarMatrix<Element>* matrix2Ptr,
				TVarMatrix<Element>* result2Ptr,
				int maxColumns,
				Element minHhNorm) const
{
	I_ASSERT((matrix2Ptr == NULL) == (result2Ptr == NULL));

	istd::CIndex2d size = GetSizes();

	int matrix2ColumnsCount = 0;
	if (matrix2Ptr != NULL){
		I_ASSERT(matrix2Ptr->GetSize(1) == size[1]);

		if (matrix2Ptr != result2Ptr){
			*result2Ptr = *matrix2Ptr;
		}

		matrix2ColumnsCount = matrix2Ptr->GetSize(0);
	}

	if (&result != this){
		result = *this;
	}

	int columnsCount = istd::Min(size[1] - 1, size[0]);
	if ((maxColumns >= 0) && (maxColumns < columnsCount)){
		columnsCount = maxColumns;
	}

	for (int hhIndex = 0; hhIndex < columnsCount; ++hhIndex){
		Element hhNorm = 0;
		for (istd::CIndex2d normIndex(hhIndex, hhIndex); normIndex[1] < size[1]; ++normIndex[1]){
			Element element = result.GetAt(normIndex);

			hhNorm += element * element;
		}

		if (hhNorm < minHhNorm){
			return false;
		}

		double element0 = result[istd::CIndex2d(hhIndex, hhIndex)];

		double hhLength = (element0 >= 0)? std::sqrt(hhNorm): -sqrt(hhNorm);

		double hhVector0 = element0 + hhLength;
		double hhVectorNorm2 = hhNorm + hhVector0 * hhVector0 - element0 * element0;

		if (result2Ptr != NULL){
			istd::CIndex2d matrix2Index;
			for (matrix2Index[0] = 0; matrix2Index[0] < matrix2ColumnsCount; ++matrix2Index[0]){
				matrix2Index[1] = hhIndex;
				double dotProduct = result2Ptr->GetAt(matrix2Index) * hhVector0;
				for (++matrix2Index[1]; matrix2Index[1] < size[1]; ++matrix2Index[1]){
					dotProduct += result2Ptr->GetAt(matrix2Index) * result[istd::CIndex2d(hhIndex, matrix2Index[1])];
				}

				double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
				matrix2Index[1] = hhIndex;
				result2Ptr->GetAtRef(matrix2Index) -= hhVector0 * reflexionFactor;
				for (matrix2Index[1]++; matrix2Index[1] < size[1]; ++matrix2Index[1]){
					result2Ptr->GetAtRef(matrix2Index) -= result.GetAt(istd::CIndex2d(hhIndex, matrix2Index[1])) * reflexionFactor;
				}
			}
		}

		istd::CIndex2d matrixIndex;
		for (matrixIndex[0] = hhIndex + 1; matrixIndex[0] < size[0]; ++matrixIndex[0]){
			matrixIndex[1] = hhIndex;
			double dotProduct = result[matrixIndex] * hhVector0;
			for (matrixIndex[1]++; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				dotProduct += result.GetAt(matrixIndex) * result.GetAt(istd::CIndex2d(hhIndex, matrixIndex[1]));
			}

			double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
			matrixIndex[1] = hhIndex;
			result[matrixIndex] -= hhVector0 * reflexionFactor;
			for (matrixIndex[1]++; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				result.GetAtRef(matrixIndex) -= result[istd::CIndex2d(hhIndex, matrixIndex[1])] * reflexionFactor;
			}
		}

		result.SetAt(istd::CIndex2d(hhIndex, hhIndex), -hhLength);
		for (int i = hhIndex + 1; i < size[1]; ++i){
			result.SetAt(istd::CIndex2d(hhIndex, i), 0);
		}
	}
	return true;
}


template <class Element>
bool TVarMatrix<Element>::GetSolvedTriangle(const TVarMatrix<Element>& vector, TVarMatrix<Element>& result, Element accuracy) const
{
	istd::CIndex2d size = GetSizes();

	I_ASSERT(vector.GetSize(1) >= size[0]);
	I_ASSERT(size[1] >= size[0]);

	int columnsCount = vector.GetSize(0);
    result.SetSizes(istd::CIndex2d(columnsCount, size[0]));

	istd::CIndex2d resultIndex;
	for (resultIndex[0] = 0; resultIndex[0] < columnsCount; ++resultIndex[0]){
		for (resultIndex[1] = size[0] - 1; resultIndex[1] >= 0; --resultIndex[1]){
			Element diagonalElement = GetAt(istd::CIndex2d(resultIndex[1], resultIndex[1]));
			if (::fabs(diagonalElement) < accuracy){
				return false;
			}

			Element previousProduct = 0.0;
			for (int i = size[0] - 1; i > resultIndex[1]; i--){
				previousProduct += result.GetAt(istd::CIndex2d(resultIndex[0], i)) * GetAt(istd::CIndex2d(i, resultIndex[1]));
			}

			result.SetAt(resultIndex, (vector.GetAt(resultIndex) - previousProduct) / diagonalElement);
		}
	}

    return true;
}


template <class Element>
bool TVarMatrix<Element>::GetSolvedLSP(const TVarMatrix<Element>& vector, TVarMatrix<Element>& result, Element accuracy) const
{
	TVarMatrix<Element> matrixR;
	TVarMatrix<Element> matrixQY;

	if (GetTriangleDecomposed(matrixR, &vector, &matrixQY, -1, accuracy)){
		return matrixR.GetSolvedTriangle(matrixQY, result, accuracy);
	}

	return false;
}


template <class Element>
void TVarMatrix<Element>::GetColumnVector(int columnIndex, TVarVector<Element>& result)
{
	I_ASSERT(columnIndex < m_sizes[0]);

	result.SetElementsCount(m_sizes[1]);

	istd::CIndex2d index(columnIndex, 0);

	for (; index[1] < m_sizes[1]; ++index[1]){
		result[index[1]] = GetAt(index);
	}
}


template <class Element>
void TVarMatrix<Element>::GetRowVector(int rowIndex, TVarVector<Element>& result)
{
	I_ASSERT(rowIndex < m_sizes[1]);

	result.SetElementsCount(m_sizes[0]);

	istd::CIndex2d index(0, rowIndex);

	for (; index[0] < m_sizes[0]; ++index[0]){
		result[index[0]] = GetAt(index);
	}
}


template <class Element>
bool TVarMatrix<Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag sizeTag("Size", "Size of matrix");
	static iser::CArchiveTag cellsTag("Cells", "Values of matrix cells");

	retVal = retVal && archive.BeginTag(sizeTag);
	if (archive.IsStoring()){
		istd::CIndex2d size = GetSizes();

		retVal = retVal && archive.Archive(size[0]);
		retVal = retVal && archive.Archive(size[1]);
	}
	else{
		istd::CIndex2d size;

		retVal = retVal && archive.Archive(size[0]);
		retVal = retVal && archive.Archive(size[1]);

		if (!retVal){
			return false;
		}

		SetSizes(size);
	}
	retVal = retVal && archive.EndTag(sizeTag);

	retVal = retVal && archive.BeginTag(cellsTag);
	for (Iterator iter = Begin(); iter != End(); ++iter){
		retVal = retVal && archive.Archive(*iter);
	}
	retVal = retVal && archive.EndTag(cellsTag);

	return retVal;
}


typedef TVarMatrix<double> CVarMatrix;


} // namespace imath


#endif // !imath_TVarMatrix_included


