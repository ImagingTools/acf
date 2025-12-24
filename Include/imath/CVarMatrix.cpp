#include <imath/CVarMatrix.h>


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <imath/CVarVector.h>


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


double CVarMatrix::GetElementAt(int x, int y) const
{
	return BaseClass::GetAt(istd::CIndex2d(x, y));
}


double& CVarMatrix::GetElementRef(int x, int y)
{
	return BaseClass::GetAtRef(istd::CIndex2d(x, y));
}


double CVarMatrix::GetMaxElement() const
{
	istd::CIndex2d size = GetSizes();
	if (size.IsSizeEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	double maxValue = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			double value = GetAt(index);

			if (value > maxValue){
				maxValue = value;
			}
		}
	}

	return maxValue;
}


double CVarMatrix::GetMinElement() const
{
	istd::CIndex2d size = GetSizes();
	if (size.IsSizeEmpty()){
		return 0;
	}

	istd::CIndex2d index(0, 0);
	double minValue = GetAt(index);

	for (index[1] = 0; index[1] < size[1]; ++index[1]){
		for (index[0] = 0; index[0] < size[0]; ++index[0]){
			double value = GetAt(index);

			if (value < minValue){
				minValue = value;
			}
		}
	}

	return minValue;
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
	Q_ASSERT(size == matrix.GetSizes());	// only matrix with the same size can be substracted

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
	Q_ASSERT(size == matrix.GetSizes());	// only matrix with the same size can be substracted

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
	Q_ASSERT(size[0] == matrixSize[1]);	// width of first matrix must be equal of second one height

	result.SetSizes(istd::CIndex2d(matrixSize[0], size[1]));

	istd::CIndex2d index;
	istd::CIndex2d matrixIndex;
	istd::CIndex2d resultIndex;

	for (resultIndex[1] = 0; resultIndex[1] < size[1]; ++resultIndex[1]){
		index[1] = resultIndex[1];

		for (resultIndex[0] = 0; resultIndex[0] < matrixSize[0]; ++resultIndex[0]){
			matrixIndex[0] = resultIndex[0];

			double sum = 0;
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


double CVarMatrix::GetTrace() const
{
	istd::CIndex2d size = GetSizes();

	int commonSize = qMin(size.GetX(), size.GetY());

	double retVal = 0;
	for (int i = 0; i < commonSize; ++i){
		double value = GetAt(istd::CIndex2d(i, i));

		retVal += value;
	}

	return retVal;
}


double CVarMatrix::GetFrobeniusNorm2() const
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


double CVarMatrix::GetFrobeniusNorm() const
{
	return qSqrt(GetFrobeniusNorm2());
}


bool CVarMatrix::GetTriangleDecomposed(
				CVarMatrix& result,
				CVarMatrix* matrix2Ptr,
				int maxColumns,
				double minHhNorm) const
{
	istd::CIndex2d size = GetSizes();

	Q_ASSERT((matrix2Ptr == NULL) || (matrix2Ptr->GetSize(1) == size[1]));

	if (&result != this){
		result = *this;
	}

	int columnsCount = qMin(size[1] - 1, size[0]);
	if ((maxColumns >= 0) && (maxColumns < columnsCount)){
		columnsCount = maxColumns;
	}

	for (int hhIndex = 0; hhIndex < columnsCount; ++hhIndex){
		double hhNorm2 = 0;
		for (istd::CIndex2d normIndex(hhIndex, hhIndex); normIndex[1] < size[1]; ++normIndex[1]){
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

		// correct optional matrix
		if (matrix2Ptr != NULL){
			int matrix2ColumnsCount = matrix2Ptr->GetSize(0);

			istd::CIndex2d matrix2Index;
			for (matrix2Index[0] = 0; matrix2Index[0] < matrix2ColumnsCount; ++matrix2Index[0]){
				matrix2Index[1] = hhIndex;
				double dotProduct = matrix2Ptr->GetAt(matrix2Index) * hhVector0;
				for (++matrix2Index[1]; matrix2Index[1] < size[1]; ++matrix2Index[1]){
					dotProduct += matrix2Ptr->GetAt(matrix2Index) * result[istd::CIndex2d(hhIndex, matrix2Index[1])];
				}

				double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
				matrix2Index[1] = hhIndex;
				matrix2Ptr->GetAtRef(matrix2Index) -= hhVector0 * reflexionFactor;
				for (matrix2Index[1]++; matrix2Index[1] < size[1]; ++matrix2Index[1]){
					matrix2Ptr->GetAtRef(matrix2Index) -= result.GetAt(istd::CIndex2d(hhIndex, matrix2Index[1])) * reflexionFactor;
				}
			}
		}

		// correct current column
		result.SetAt(istd::CIndex2d(hhIndex, hhIndex), -hhLength);
		for (int i = hhIndex + 1; i < size[1]; ++i){
			result.SetAt(istd::CIndex2d(hhIndex, i), 0);
		}
	}
	return true;
}


bool CVarMatrix::TransformR(int firstPartWidth)
{
	istd::CIndex2d size = GetSizes();

	Q_ASSERT(firstPartWidth < size.GetX());

	// QR transformation using Householder's reflections.
	for (int column = 0; column < firstPartWidth; column++){
		// calculate Householder's vector.
		double normA2 = 0;
		int i;
		for (i = column; i < size.GetY(); i++){
			double element = GetAt(istd::CIndex2d(column, i));
			normA2 += element * element;
		}

		if (qAbs(normA2) < I_EPSILON){
			return false;
		}

		double element0 = GetAt(istd::CIndex2d(column, column));

		double hhSign = (element0 >= 0)? 1: -1;
		double lenghtA = sqrt(normA2);

		double hhVector_0 = element0 + hhSign * lenghtA;	// v = a + e * |a|, where e = {1, 0, 0, ..., 0}'
		double normV2 = normA2 + hhVector_0 * hhVector_0 - element0 * element0;	// norm2 := |v|^2;

		// transform matrix R
		istd::CIndex2d changedIndex;
		for (int x = column + 1; x < size.GetX(); x++){
			changedIndex.IncreaseAt(0);

			// calculate dot product
			changedIndex.SetY(column);
			int y = column;

			double dotProduct = GetAt(changedIndex) * hhVector_0;

			for (y++; y < size.GetY(); y++){
				changedIndex.IncreaseAt(1);

				dotProduct += GetAt(changedIndex) * GetAt(istd::CIndex2d(column, changedIndex.GetY()));
			}

			double transConst = 2 * dotProduct / normV2;
			// transform column: c = c - 2 * v * <c, v> / <v, v>, where <., .> is dot product operator
			changedIndex.SetY(column);
			GetAtRef(changedIndex) -= hhVector_0 * transConst;

			y = column;

			for (y++; y < size.GetY(); y++){
				changedIndex.IncreaseAt(1);

				GetAtRef(changedIndex) -= GetAt(istd::CIndex2d(column, changedIndex.GetY())) * transConst;
			}
		}

		SetAt(istd::CIndex2d(column, column), -hhSign * lenghtA);
		for (i = column + 1; i < size.GetY(); i++){
			SetAt(istd::CIndex2d(column, i), 0.0);
		}
	}

	return true;
}


bool CVarMatrix::GetSolvedTriangle(const CVarMatrix& vector, CVarMatrix& result, double accuracy) const
{
	istd::CIndex2d size = GetSizes();

	Q_ASSERT(vector.GetSize(1) >= size[0]);
	Q_ASSERT(size[1] >= size[0]);

	int columnsCount = vector.GetSize(0);
	result.SetSizes(istd::CIndex2d(columnsCount, size[0]));

	istd::CIndex2d resultIndex;
	for (resultIndex[0] = 0; resultIndex[0] < columnsCount; ++resultIndex[0]){
		for (resultIndex[1] = size[0] - 1; resultIndex[1] >= 0; --resultIndex[1]){
			double diagonalElement = GetAt(istd::CIndex2d(resultIndex[1], resultIndex[1]));
			if (qAbs(diagonalElement) < accuracy){
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
	CVarMatrix matrixQY = vector;

	if (GetTriangleDecomposed(matrixR, &matrixQY, -1, accuracy)){
		return matrixR.GetSolvedTriangle(matrixQY, result, accuracy);
	}

	return false;
}


bool CVarMatrix::GetDecompositionQDQ(CVarMatrix& matrixQ, CVarVector& diagonalD, double tolerance, int maxIterations) const
{
	istd::CIndex2d size = GetSizes();

	if (size.GetX() != size.GetY()){
		return false;
	}

	CVarMatrix matrixR = *this;
	matrixQ.SetSizes(size);
	matrixQ.Clear();

	for (int i = 0; i < maxIterations; ++i){
		CVarMatrix tempMatrixR;
		CVarMatrix tempMatrixQ;
		if (!matrixR.GetTriangleDecomposed(tempMatrixR, &tempMatrixQ)){
			return false;
		}
		matrixR = tempMatrixR.GetMultiplied(tempMatrixQ);

		matrixQ = tempMatrixQ.GetMultiplied(matrixQ);

		double residue = 0;
		istd::CIndex2d index;
		for (index[0] = 0; index[0] < size.GetX(); ++index[0]){
			for (index[1] = 0; index[1] < size.GetX(); ++index[1]){
				if (index[0] != index[1]){
					double element = matrixR.GetAt(index);

					residue += element * element;
				}
			}
		}

		if (residue <= tolerance){
			for (int j = 0; j < size.GetX(); ++j){
				diagonalD[j] = matrixR.GetAt(istd::CIndex2d(j, j));
			}

			return true;
		}
	}

	return false;
}


void CVarMatrix::GetColumnVector(int columnIndex, CVarVector& result)
{
	istd::CIndex2d size = GetSizes();
	Q_ASSERT(columnIndex < size[0]);

	result.SetElementsCount(size[1]);

	istd::CIndex2d index(columnIndex, 0);

	for (; index[1] < size[1]; ++index[1]){
		result[index[1]] = GetAt(index);
	}
}


void CVarMatrix::GetRowVector(int rowIndex, CVarVector& result)
{
	istd::CIndex2d size = GetSizes();
	Q_ASSERT(rowIndex < size[1]);

	result.SetElementsCount(size[0]);

	istd::CIndex2d index(0, rowIndex);

	for (; index[0] < size[0]; ++index[0]){
		result[index[0]] = GetAt(index);
	}
}


bool CVarMatrix::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag sizeTag("Size", "Size of matrix", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag cellsTag("Cells", "Values of matrix cells", iser::CArchiveTag::TT_GROUP);

	bool retVal = true;

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


// static methods

/**
	LSP solving using Householder reflexions with column preselection.
	In each step we have calculated column with maximal norm.
	This column will be transformed and its index will be stored in permutation table.
	This permutation table stores index in matrix A for each calculation step.
*/
void CVarMatrix::SolveRobustLSP(CVarMatrix matrixA, CVarMatrix& matrixY, CVarMatrix& matrixX, double accuracy)
{
	Q_ASSERT(accuracy > 0);

	istd::CIndex2d size = matrixA.GetSizes();
	Q_ASSERT(size[1] == matrixY.GetSize(1));

	int columnsCount = size[0];
	int matrixYColumnsCount = matrixY.GetSize(0);

	int maxStepsCount = qMin(size[1], columnsCount);

	QVector<double> columnNorms2(columnsCount);
	QVector<int> realColumnIndices(columnsCount);

	int maxNormColumnIndex = -1;
	double maxNorm2 = 0;

	istd::CIndex2d matrixIndex;
	for (matrixIndex[0] = 0; matrixIndex[0] < columnsCount; ++matrixIndex[0]){
		realColumnIndices[matrixIndex[0]] = matrixIndex[0];

		double norm2 = 0;
		for (matrixIndex[1] = 0; matrixIndex[1] < size[1]; ++matrixIndex[1]){
			double element = matrixA.GetAt(matrixIndex);

			norm2 += element * element;
		}

		if (norm2 >= maxNorm2){
			maxNorm2 = norm2;
			maxNormColumnIndex = matrixIndex[0];
		}

		columnNorms2[matrixIndex[0]] = norm2;
	}

	int stepIndex = 0;
	for (; stepIndex < maxStepsCount; ++stepIndex){
		if (maxNorm2 < accuracy){
			break;
		}
		Q_ASSERT(maxNormColumnIndex >= stepIndex);

		int realColumnIndex = realColumnIndices[maxNormColumnIndex];
		if (maxNormColumnIndex != stepIndex){
			int prevIndex = realColumnIndices[stepIndex];	// switch columns in permutation table
			realColumnIndices[stepIndex] = realColumnIndex;
			realColumnIndices[maxNormColumnIndex] = prevIndex;
		}

		double element0 = matrixA[istd::CIndex2d(realColumnIndex, stepIndex)];

		double hhLength = (element0 >= 0)? qSqrt(maxNorm2): -qSqrt(maxNorm2);	// destination diagonal value of current processed column, sign is choosen optimal for maximal householder vector length

		double hhVector0 = element0 + hhLength;	// element 0 of householder vector, rest of ist will be taken directly from matrix elements
		double hhVectorNorm2 = maxNorm2 + hhVector0 * hhVector0 - element0 * element0;	// sqare of norm of householder vector, first element replaced from original column norm sqare

		maxNormColumnIndex = -1;
		maxNorm2 = 0;
		// correct next columns
		for (int i = stepIndex + 1; i < columnsCount; ++i){
			matrixIndex[0] = realColumnIndices[i];
			matrixIndex[1] = stepIndex;
			double dotProduct = matrixA[matrixIndex] * hhVector0;
			for (matrixIndex[1]++; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				dotProduct += matrixA.GetAt(matrixIndex) * matrixA.GetAt(istd::CIndex2d(realColumnIndex, matrixIndex[1]));
			}

			double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
			matrixIndex[1] = stepIndex;

			double& firstElement = matrixA[matrixIndex];

			firstElement -= hhVector0 * reflexionFactor;
			for (matrixIndex[1]++; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				matrixA.GetAtRef(matrixIndex) -= matrixA[istd::CIndex2d(realColumnIndex, matrixIndex[1])] * reflexionFactor;
			}

			// correct norm table, remove the current row
			double norm2 = columnNorms2[matrixIndex[0]] - firstElement * firstElement;
			if (norm2 >= maxNorm2){
				maxNorm2 = norm2;
				maxNormColumnIndex = i;
			}
			columnNorms2[matrixIndex[0]] = norm2;
		}

		// correct matrix QY
		for (matrixIndex[0] = 0; matrixIndex[0] < matrixYColumnsCount; ++matrixIndex[0]){
			matrixIndex[1] = stepIndex;
			double dotProduct = matrixY.GetAt(matrixIndex) * hhVector0;
			for (++matrixIndex[1]; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				dotProduct += matrixY.GetAt(matrixIndex) * matrixA[istd::CIndex2d(realColumnIndex, matrixIndex[1])];
			}

			double reflexionFactor = 2 * dotProduct / hhVectorNorm2;
			matrixIndex[1] = stepIndex;
			matrixY.GetAtRef(matrixIndex) -= hhVector0 * reflexionFactor;
			for (matrixIndex[1]++; matrixIndex[1] < size[1]; ++matrixIndex[1]){
				matrixY.GetAtRef(matrixIndex) -= matrixA.GetAt(istd::CIndex2d(realColumnIndex, matrixIndex[1])) * reflexionFactor;
			}
		}

		// correct current column
		matrixA.SetAt(istd::CIndex2d(realColumnIndex, stepIndex), -hhLength);
		for (int i = stepIndex + 1; i < size[1]; ++i){
			matrixA.SetAt(istd::CIndex2d(realColumnIndex, i), 0);
		}
	}

	// calculate output matrix X solving equation RX = Y
	matrixX.SetSizes(istd::CIndex2d(matrixYColumnsCount, size[0]));

	double restNorm2 = 0;
	if (stepIndex > 0){
		matrixIndex[1] = stepIndex - 1;
		for (int i = stepIndex - 1; i < columnsCount; ++i){
			matrixIndex[0] = realColumnIndices[i];
			double element = matrixA.GetAt(matrixIndex);

			restNorm2 += element * element;
		}
	}

	istd::CIndex2d resultIndex;
	for (resultIndex[0] = 0; resultIndex[0] < matrixYColumnsCount; ++resultIndex[0]){
		if (qAbs(restNorm2) >= accuracy){
			Q_ASSERT(stepIndex > 0);	// restNorm is positive only if it was calculated -> there are non-zero elemens

			double valueY = matrixY.GetAt(istd::CIndex2d(resultIndex[0], stepIndex - 1));

			matrixIndex[1] = stepIndex - 1;
			for (int i = stepIndex - 1; i < columnsCount; ++i){
				matrixIndex[0] = realColumnIndices[i];
				resultIndex[1] = matrixIndex[0];

				matrixX.SetAt(resultIndex, valueY * matrixA.GetAt(matrixIndex) / restNorm2);
			}
		}
		else{
			for (int i = qMax(stepIndex - 1, 0); i < columnsCount; ++i){
				resultIndex[1] = realColumnIndices[i];

				matrixX.SetAt(resultIndex, 0);
			}
		}

		for (int i = stepIndex - 2; i >= 0; --i){
			resultIndex[1] = realColumnIndices[i];

			double diagonalElement = matrixA.GetAt(istd::CIndex2d(resultIndex[1], i));
			Q_ASSERT(qAbs(diagonalElement) >= accuracy);	// was sorted previously and this condition was filled

			double previousProduct = 0.0;
			for (int columnIndex = size[0] - 1; columnIndex > i; columnIndex--){
				int realColumnIndex = realColumnIndices[columnIndex];
				previousProduct += matrixX.GetAt(istd::CIndex2d(resultIndex[0], realColumnIndex)) * matrixA.GetAt(istd::CIndex2d(realColumnIndex, i));
			}

			double valueY = matrixY.GetAt(istd::CIndex2d(resultIndex[0], i));
			matrixX.SetAt(resultIndex, (valueY - previousProduct) / diagonalElement);
		}
	}
}


} // namespace imath


