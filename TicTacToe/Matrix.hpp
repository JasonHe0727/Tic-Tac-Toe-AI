#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <initializer_list>
#include <iostream>
template <typename T>
class Matrix;
template <typename T>
class Matrix
{
public:
    Matrix();
    ~Matrix();
    Matrix(int nRows, int nCols);
    Matrix(int nRows, int nCols, std::initializer_list<T> args);
    Matrix(const Matrix<T>& other);
    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix(Matrix<T>&& other);
    Matrix<T>& operator=(Matrix<T>&& other);

    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator-(const Matrix<T>& other) const;
    Matrix<T> operator*(const Matrix<T>& other) const;

    Matrix<T> DotMultiply(const Matrix<T>& other) const;
    Matrix<T> DotDivide(const Matrix<T>& other) const;

    T& operator()(int index) const;
    T& operator()(int row, int col) const;
    template <typename>
    friend std::ostream& operator<<(std::ostream& stream, Matrix<T> matrix);

    template <typename>
    friend Matrix<T> operator+(const T& value, const Matrix<T> matrix);
    template <typename>
    friend Matrix<T> operator+(const Matrix<T> matrix, const T& value);

    template <typename>
    friend Matrix<T> operator-(const T& value, const Matrix<T> matrix);
    template <typename>
    friend Matrix<T> operator-(const Matrix<T> matrix, const T& value);

    template <typename>
    friend Matrix<T> operator*(const T& value, const Matrix<T> matrix);
    template <typename>
    friend Matrix<T> operator*(const Matrix<T> matrix, const T& value);

    template <typename>
    friend Matrix<T> operator/(const T& value, const Matrix<T> matrix);
    template <typename>
    friend Matrix<T> operator/(const Matrix<T> matrix, const T& value);

    int Rows() const;
    int Cols() const;
    T& At(int row, int col);
    Matrix<T> Transpose();

    template <typename TFunction>
    Matrix<T> Apply(const TFunction& f) const;

    template <typename TFunction>
    Matrix<T> Apply(const Matrix<T>& other, const TFunction& f) const;

    T SumOfRow(int row);
    T SumOfColumn(int col);

    static Matrix<T> RowVector(int nRows);

    static Matrix<T> ColumnVector(int nCols);

private:
    int nRows;
    int nCols;
    T* items;

    bool IsEmptyMatrix() const;
};

class MatrixException
{
public:
    int RowCount;
    int ColumnCount;
    MatrixException(int RowCount, int ColumnCount)
        : RowCount{RowCount}
        , ColumnCount{ColumnCount}
    {
    }
};

class MatrixInitializationException : public MatrixException
{
public:
    MatrixInitializationException(int RowCount, int ColumnCount)
        : MatrixException(RowCount, ColumnCount)
    {
    }
};

class MatrixDimensionException : public MatrixException
{
public:
    MatrixDimensionException(int RowCount, int ColumnCount)
        : MatrixException(RowCount, ColumnCount)
    {
    }
};

template <typename T>
Matrix<T>::Matrix()
    : nRows{0}
    , nCols{0}
    , items{nullptr}
{
}

template <typename T>
Matrix<T>::~Matrix()
{
    if (items)
    {
        delete[] items;
    }
}

template <typename T>
Matrix<T>::Matrix(int nRows, int nCols)
    : nRows{nRows}
    , nCols{nCols}
    , items{new T[nRows * nCols]}
{
}

template <typename T>
Matrix<T>::Matrix(int nRows, int nCols, std::initializer_list<T> args)
    : nRows{nRows}
    , nCols{nCols}
    , items{new T[nRows * nCols]}
{
    int length = nRows * nCols;
    if (length == static_cast<int>(args.size()))
    {
        int i = 0;
        for (T item : args)
        {
            items[i] = item;
            i++;
        }
    }
    else
    {
        throw MatrixInitializationException(nRows, nCols);
    }
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
    nRows = other.nRows;
    nCols = other.nCols;
    if (other.items == nullptr)
    {
        items = nullptr;
        return *this;
    }
    else
    {
        int length = nRows * nCols;
        items = new T[length];
        for (int i = 0; i < length; i++)
        {
            items[i] = other.items[i];
        }
        return *this;
    }
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other)
    : nRows{other.nRows}
    , nCols{other.nCols}
    , items{other.items}
{
    other.items = nullptr;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other)
{
    nRows = other.nRows;
    nCols = other.nCols;
    items = other.items;
    other.items = nullptr;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
{
    if (nRows == other.nRows && nCols == other.nCols)
    {
        Matrix result(nRows, nCols);
        int length = nRows * nCols;
        for (int i = 0; i < length; i++)
        {
            result.items[i] = items[i] + other.items[i];
        }
        return result;
    }
    else
    {
        throw MatrixDimensionException(nRows, nCols);
    }
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const
{
    if (nRows == other.nRows && nCols == other.nCols)
    {
        Matrix result(nRows, nCols);
        int length = nRows * nCols;
        for (int i = 0; i < length; i++)
        {
            result.items[i] = items[i] - other.items[i];
        }
        return result;
    }
    else
    {
        throw MatrixDimensionException(nRows, nCols);
    }
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
    if (nCols == other.nRows)
    {
        Matrix result(nRows, other.nCols);
        for (int i = 0; i < nRows; i++)
        {
            for (int j = 0; j < other.nCols; j++)
            {
                T sum = 0;
                for (int k = 0; k < nCols; k++)
                {
                    sum = sum + items[i * nCols + k] *
                                    other.items[k * other.nCols + j];
                }
                result.items[i * result.nCols + j] = sum;
            }
        }
        return result;
    }
    else
    {
        throw MatrixDimensionException(nRows, nCols);
    }
}

template <typename T>
Matrix<T> Matrix<T>::DotMultiply(const Matrix<T>& other) const
{
    if (nRows == other.nRows && nCols == other.nCols)
    {
        Matrix<double> result(nRows, nCols);
        int length = nRows * nCols;
        for (int i = 0; i < length; i++)
        {
            result.items[i] = items[i] * other.items[i];
        }
        return result;
    }
    else
    {
        throw MatrixDimensionException(nRows, nCols);
    }
}

template <typename T>
Matrix<T> Matrix<T>::DotDivide(const Matrix<T>& other) const
{
    if (nRows == other.nRows && nCols == other.nCols)
    {
        Matrix result(nRows, nCols);
        int length = nRows * nCols;
        for (int i = 0; i < length; i++)
        {
            result.items[i] = items[i] / other.items[i];
        }
        return result;
    }
    else
    {
        throw MatrixDimensionException(nRows, nCols);
    }
}

template <typename T>
T& Matrix<T>::operator()(int index) const
{
    return items[index];
}

template <typename T>
T& Matrix<T>::operator()(int row, int col) const
{
    return items[row * nCols + col];
}

template <typename T>
int Matrix<T>::Rows() const
{
    return nRows;
}
template <typename T>
int Matrix<T>::Cols() const
{
    return nCols;
}

template <typename T>
T& Matrix<T>::At(int row, int col)
{
    return items[row * nCols + col];
}

template <typename T>
Matrix<T> Matrix<T>::Transpose()
{
    Matrix<T> result(nCols, nRows);
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

template <typename T>
T Matrix<T>::SumOfRow(int row)
{
    T sum = 0;
    for (int j = 0; j < nCols; j++)
    {
        sum = sum + (*this)(row, j);
    }
    return sum;
}

template <typename T>
T Matrix<T>::SumOfColumn(int col)
{
    T sum = 0;
    for (int i = 0; i < nRows; i++)
    {
        sum = sum + (*this)(i, col);
    }
    return sum;
}

template <typename T>
Matrix<T> Matrix<T>::RowVector(int nRows)
{
    return Matrix<T>(nRows, 1);
}

template <typename T>
Matrix<T> Matrix<T>::ColumnVector(int nCols)
{
    return Matrix<T>(1, nCols);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, Matrix<T> matrix)
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            stream << matrix.At(i, j) << '\t';
        }
        stream << std::endl;
    }
    return stream;
}

template <typename T>
bool Matrix<T>::IsEmptyMatrix() const
{
    return nRows == 0 && nCols == 0 && items == nullptr;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : nRows{other.nRows}
    , nCols{other.nCols}
{
    if (other.items == nullptr)
    {
        items = nullptr;
    }
    else
    {
        int length = nRows * nCols;
        items = new T[length];
        for (int i = 0; i < length; i++)
        {
            items[i] = other.items[i];
        }
    }
}

template <typename T>
Matrix<T> operator+(const T& value, const Matrix<T>& matrix)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = value + matrix(i);
    }
    return result;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& matrix, const T& value)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = matrix(i) + value;
    }
    return result;
}

template <typename T>
Matrix<T> operator-(const T& value, const Matrix<T>& matrix)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = value - matrix(i);
    }
    return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& matrix, const T& value)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = matrix(i) - value;
    }
    return result;
}

template <typename T>
Matrix<T> operator*(const T& value, const Matrix<T>& matrix)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = value * matrix(i);
    }
    return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& value)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = matrix(i) * value;
    }
    return result;
}

template <typename T>
Matrix<T> operator/(const T& value, const Matrix<T>& matrix)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = value / matrix(i);
    }
    return result;
}

template <typename T>
Matrix<T> operator/(const Matrix<T>& matrix, const T& value)
{
    Matrix<T> result(matrix.Rows(), matrix.Cols());
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        result(i) = matrix(i) / value;
    }
    return result;
}

template <typename T>
template <typename TFunction>
Matrix<T> Matrix<T>::Apply(const TFunction& f) const
{
    Matrix<T> result(nRows, nCols);
    int length = nRows * nCols;
    for (int i = 0; i < length; i++)
    {
        result(i) = f((*this)(i));
    }
    return result;
}

template <typename T>
template <typename TFunction>
Matrix<T> Matrix<T>::Apply(const Matrix<T>& other, const TFunction& f) const
{
    Matrix<T> result(nRows, nCols);
    int length = nRows * nCols;
    for (int i = 0; i < length; i++)
    {
        result(i) = f((*this)(i), other(i));
    }
    return result;
}
#endif // MATRIX_HPP
