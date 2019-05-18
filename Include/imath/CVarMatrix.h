#ifndef imath_CVarMatrix_included
#define imath_CVarMatrix_included


// ACF includes
#include <istd/istd.h>
#include <istd/TArray.h>
#include <istd/CIndex2d.h>


namespace iser
{
	class IArchive;
}


namespace imath
{


class CVarVector;


/**
	Definition of mathematical matrix.
*/
class CVarMatrix: public istd::TArray<double, 2>
{
public:
	typedef istd::TArray<double, 2> BaseClass;

	/**
		Create empty matrix.
	*/
	CVarMatrix();

	/**
		Copy constructor.
	*/
	CVarMatrix(const CVarMatrix& matrix);

	/**
		Create matrix with specified size.
	*/
	explicit CVarMatrix(istd::CIndex2d size);

	/**
		Create matrix from vector.
	*/
	explicit CVarMatrix(const CVarVector& vector, bool isTransposed = false);

	/**
		Set all matrix cells to zero.
	*/
	void Clear();

	/**
		Create identity matrix.
	*/
	void InitToIdentity(int size);

	/*
		Get a matrix element at the given position.
	*/
	double GetElementAt(int x, int y) const;

	/*
		Get the reference of a matrix element at the given position.
	*/
	double& GetElementRef(int x, int y);

	/**
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
	void GetNegated(CVarMatrix& result);

	/**
		Get sum of two matrices.
	*/
	void GetAdded(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of substraction of two matrices.
	*/
	void GetSubstracted(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	void GetMultiplied(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	CVarMatrix GetMultiplied(const CVarMatrix& matrix) const;

	/**
		Get result of multiplication of this matrix with scalar value.
	*/
	void GetScaled(double value, CVarMatrix& result) const;

	/**
		Get transposed matrix.
	*/
	void GetTransposed(CVarMatrix& result) const;

	/**
		Get transposed matrix.
	*/
	CVarMatrix GetTransposed() const;

	/**
		Transpose matrix.
	*/
	void Transpose();

	/**
		Get trace of this matrix.
	*/
	double GetTrace() const;

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
				CVarMatrix& result,
				CVarMatrix* matrix2Ptr = NULL,
				int maxColumns = -1,
				double minHhNorm = I_BIG_EPSILON) const;

	/**
		Transform this matrix in place.
	*/
	bool TransformR(int firstPartWidth);

	/**
		Solving of linear system with triangle matrix.
		Rx = y, result = x.
		\return	true if linear equation system was solved.
	*/
	bool GetSolvedTriangle(const CVarMatrix& vector, CVarMatrix& result, double accuracy = I_BIG_EPSILON) const;

	/**
		Solve 'Least Square Problem'.
		Solve linear Least Square Problem for equation AX = Y, where A is a N * M matrix, N >= M, X is n * k matrix and Y is m * k matrix.
	 */
	bool GetSolvedLSP(const CVarMatrix& vector, CVarMatrix& result, double minHhNorm = I_BIG_EPSILON) const;

	/**
		Calculate decomposition in form of QDQ where \c Q is orthogonal matrix and \c D is diagonal one.
		It works for square matrix only.
	*/
	bool GetDecompositionQDQ(CVarMatrix& matrixQ, CVarVector& diagonalD, double tolerance = I_BIG_EPSILON, int maxIterations = 100) const;

	/**
		Get single column as vector.
	*/
	void GetColumnVector(int columnIndex, CVarVector& result);
	/**
		Get single row as vector.
	*/
	void GetRowVector(int rowIndex, CVarVector& result);

	bool Serialize(iser::IArchive& archive);

	// operators
	CVarMatrix operator+(const CVarMatrix& b) const;
	CVarMatrix operator-(const CVarMatrix& b) const;
	CVarMatrix operator-();
	CVarMatrix operator*(const CVarMatrix& b) const;
	CVarMatrix operator*(double value) const;

	bool operator==(const CVarMatrix& matrix) const;
	bool operator!=(const CVarMatrix& matrix) const;

	// static methods
	/**
		Solve 'Least Square Problem' using robust algorithm.
		Solve linear Least Square Problem for equation AX = Y, where A is a \em {n * m} matrix, X is \em {m * k} matrix and Y is \em {n * k} matrix.
		This implementation solve LSP in place, it transforms internal matrix A into R = HA and matrix Y into Y' = HY.
		Then it solves equation in form RX = Y', where R is 'quasi' triangle matrix.
		\param	matrixA	input matrix A in equation AX = Y.
						It will be destroyed by this operation (triangle matrix R = HA will be calculated in place).
		\param	matrixY	input matrix Y in equation AX = Y.
						It will be destroyed by this operation (matrix Y' = HY will be calculated in place).
						Please note, that height of this matrix must be the same as height of matrix A.
		\param	matrixX result matrix X in equation AX = Y. It will be initialized be this function.
						The output size of this matrix will be set to \em {m * k} where \em m is width of matrix A and \em k is height of matrix Y.
	 */
	static void SolveRobustLSP(CVarMatrix matrixA, CVarMatrix& matrixY, CVarMatrix& matrixX, double minHhNorm = I_BIG_EPSILON);
};


// inline methods

inline CVarMatrix CVarMatrix::GetMultiplied(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetMultiplied(matrix, result);

	return result;
}


inline void CVarMatrix::Transpose()
{
	CVarMatrix result;

	GetTransposed(result);

	*this=result;
}


inline CVarMatrix CVarMatrix::operator+(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetAdded(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator-(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetSubstracted(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator-()
{
	CVarMatrix result;

	GetNegated(result);

	return result;
}


inline CVarMatrix CVarMatrix::operator*(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetMultiplied(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator*(double value) const
{
	CVarMatrix result;

	GetScaled(value, result);

	return result;
}


inline CVarMatrix operator*(double value, const imath::CVarMatrix& matrix)
{
	return matrix * value;
}


} // namespace imath


#endif // !imath_CVarMatrix_included


