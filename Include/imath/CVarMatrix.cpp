#include "imath/CVarMatrix.h"


// STL includes
#include <cmath>

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CVarVector.h"


namespace imath
{


CVarMatrix::CVarMatrix()
{
}


CVarMatrix::CVarMatrix(const CVarMatrix& matrix)
:	BaseClass(matrix)
{
}


CVarMatrix::CVarMatrix(istd::CIndex2d size)
{
	SetSizes(size);
}


CVarMatrix::CVarMatrix(const CVarVector& vector, bool isTransposed)
{
	if (isTransposed){
		DeepCopy(vector.GetElements(), istd::CIndex2d(vector.GetElementsCount(), 1));
	}
	else{
		DeepCopy(vector.GetElements(), istd::CIndex2d(1, vector.GetElementsCount()));
	}
}


// init operations

void CVarMatrix::Clear()
{
	istd::CIndex2d size = GetSizes();

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			SetAt(index, 0);
		}
    }
}


void CVarMatrix::InitToIdentity(int size)
{
    SetSizes(istd::CIndex2d(size, size));

	Clear();

	for (int index = 0; index < size; ++index){
        SetAt(istd::CIndex2d(index, index), 1);
	}
}


double CVarMatrix::GetMaxElement() const
{
	istd::CIndex2d size = GetSizes();
	if (size.IsSizeEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	double retVal = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			double value = GetAt(index);

			if (value > retVal){
				retVal = value;
			}
		}
    }

	return retVal;
}


double CVarMatrix::GetMinElement() const
{
	istd::CIndex2d size = GetSizes();
	if (size.IsSizeEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	double retVal = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			double value = GetAt(index);

			if (value < retVal){
				retVal = value;
			}
		}
    }

	return retVal;
}


void CVarMatrix::GetNegated(CVarMatrix& result)
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


void CVarMatrix::GetAdded(const CVarMatrix& matrix, CVarMatrix& result) const
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(size == matrix.GetSizes());	// only matrix with the same size can be substracted

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, GetAt(index) + matrix.GetAt(index));
		}
	}
}


void CVarMatrix::GetSubstracted(const CVarMatrix& matrix, CVarMatrix& result) const
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(size == matrix.GetSizes());	// only matrix with the same size can be substracted

	result.SetSizes(size);

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			result.SetAt(index, GetAt(index) - matrix.GetAt(index));
		}
	}
}


void CVarMatrix::GetMultiplied(const CVarMatrix& matrix, CVarMatrix& result) const
{
	istd::CIndex2d matrixSize = matrix.GetSizes();
	istd::CIndex2d size = GetSizes();
    I_ASSERT(size[0] == matrixSize[1]);	// width of first matrix must be equal of second one height

    double sum = 0;

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


void CVarMatrix::GetScaled(double value, CVarMatrix& result) const
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


void CVarMatrix::GetTransposed(CVarMatrix& result) const
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


double CVarMatrix::GetEuclideanNorm2() const
{
	istd::CIndex2d size = GetSizes();

	double retVal = 0.0;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			double value = GetAt(index);
			retVal += double(value * value);
		}
    }
	return retVal;
}


double CVarMatrix::GetEuclideanNorm() const
{
	return std::sqrt(GetEuclideanNorm2());
}


bool CVarMatrix::GetTriangleDecomposed(
				CVarMatrix& result,
				const CVarMatrix* matrix2Ptr,
				CVarMatrix* result2Ptr,
				int maxColumns,
				double minHhNorm) const
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
		double hhNorm = 0;
		for (istd::CIndex2d normIndex(hhIndex, hhIndex); normIndex[1] < size[1]; ++normIndex[1]){
			double element = result.GetAt(normIndex);

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


bool CVarMatrix::GetSolvedTriangle(const CVarMatrix& vector, CVarMatrix& result, double accuracy) const
{
	istd::CIndex2d size = GetSizes();

	I_ASSERT(vector.GetSize(1) >= size[0]);
	I_ASSERT(size[1] >= size[0]);

	int columnsCount = vector.GetSize(0);
    result.SetSizes(istd::CIndex2d(columnsCount, size[0]));

	istd::CIndex2d resultIndex;
	for (resultIndex[0] = 0; resultIndex[0] < columnsCount; ++resultIndex[0]){
		for (resultIndex[1] = size[0] - 1; resultIndex[1] >= 0; --resultIndex[1]){
			double diagonalElement = GetAt(istd::CIndex2d(resultIndex[1], resultIndex[1]));
			if (::fabs(diagonalElement) < accuracy){
				return false;
			}

			double previousProduct = 0.0;
			for (int i = size[0] - 1; i > resultIndex[1]; i--){
				previousProduct += result.GetAt(istd::CIndex2d(resultIndex[0], i)) * GetAt(istd::CIndex2d(i, resultIndex[1]));
			}

			result.SetAt(resultIndex, (vector.GetAt(resultIndex) - previousProduct) / diagonalElement);
		}
	}

    return true;
}


bool CVarMatrix::GetSolvedLSP(const CVarMatrix& vector, CVarMatrix& result, double accuracy) const
{
	CVarMatrix matrixR;
	CVarMatrix matrixQY;

	if (GetTriangleDecomposed(matrixR, &vector, &matrixQY, -1, accuracy)){
		return matrixR.GetSolvedTriangle(matrixQY, result, accuracy);
	}

	return false;
}


void CVarMatrix::GetColumnVector(int columnIndex, CVarVector& result)
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(columnIndex < size[0]);

	result.SetElementsCount(size[1]);

	istd::CIndex2d index(columnIndex, 0);

	for (int i = 0; i < size[1]; ++index[1]){
		result[index[1]] = GetAt(index);
	}
}


void CVarMatrix::GetRowVector(int rowIndex, CVarVector& result)
{
	istd::CIndex2d size = GetSizes();
	I_ASSERT(rowIndex < size[1]);

	result.SetElementsCount(size[0]);

	istd::CIndex2d index(0, rowIndex);

	for (; index[0] < size[0]; ++index[0]){
		result[index[0]] = GetAt(index);
	}
}


bool CVarMatrix::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag sizeTag("Size", "Size of matrix");
	static iser::CArchiveTag cellsTag("Cells", "Values of matrix cells");

	retVal = retVal && archive.BeginTag(sizeTag);
	if (archive.IsStoring()){
		istd::CIndex2d size = GetSizes();

		retVal = retVal && archive.Process(size[0]);
		retVal = retVal && archive.Process(size[1]);
	}
	else{
		istd::CIndex2d size;

		retVal = retVal && archive.Process(size[0]);
		retVal = retVal && archive.Process(size[1]);

		if (!retVal){
			return false;
		}

		SetSizes(size);
	}
	retVal = retVal && archive.EndTag(sizeTag);

	retVal = retVal && archive.BeginTag(cellsTag);
	for (Iterator iter = Begin(); iter != End(); ++iter){
		retVal = retVal && archive.Process(*iter);
	}
	retVal = retVal && archive.EndTag(cellsTag);

	return retVal;
}


} // namespace imath


