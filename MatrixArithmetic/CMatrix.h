#pragma once

#include "CAppException.h"
#include <assert.h>

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T> 
class CMatrix
{
public:
    // ---------------------------------------------------------------------------
    // Initializes the CMatrix object that is of uRow * uCol in size.
    // Optionally the matrix is initialized with the content of pData that if
    // provided should contain uRow * uCol elements.
    // ---------------------------------------------------------------------------

    CMatrix(unsigned int uRow, unsigned int uCol, T * pData = NULL);

    // ---------------------------------------------------------------------------
    // We need a copy constructor to do deep object copies

    CMatrix(const CMatrix<T> & src);
    ~CMatrix();

    // ---------------------------------------------------------------------------
    // Retrieve the matrix's width and height

    inline unsigned int NumRows() { return(m_uRows); }
    inline unsigned int NumColumns() { return(m_uColumns); }

    // ---------------------------------------------------------------------------
    // The matrix data is stored as a contiguous memory that can be indexed. This
    // method converts the index into a row,col coordinates of that position.

    void GetRowColFromIndex(unsigned int uElementIndex, unsigned int * puRow, unsigned int * puCol) const;

    // ---------------------------------------------------------------------------
    // Retrieves an element at the specific row, col coordinates
    T GetAt(unsigned int uRow, unsigned int uCol) const;

    // ---------------------------------------------------------------------------
    // Sets an element at the specific row, col coordinates
    void SetAt(unsigned int uRow, unsigned int uCol, T Element);

    // ---------------------------------------------------------------------------
    // If uNumElements is zero and pData is null, this method returns how many
    // elements the provided buffer should be capable of storing. This is the same
    // calculation as the matrix's row*col. If pData is large enough then all
    // elements from the matrix are copied into it.

    void GetAllData(unsigned int * uNumElements, T * pData) const;

    // ---------------------------------------------------------------------------
    // If uNumElements is zero and pData is null, this method returns how many
    // elements the provided buffer should be capable of storing.
    // Returns all the data for the specified row index.

    void GetRowData(unsigned int uRowIndex, unsigned int * puNumElements, T * pBuffer) const;
    
    // ---------------------------------------------------------------------------
    // If uNumElements is zero and pData is null, this method returns how many
    // elements the provided buffer should be capable of storing.
    // Returns all the data for the specified column index.

    void GetColumnData(unsigned int uColumnIndex, unsigned int * puNumElements, T * pBuffer) const;

    // ---------------------------------------------------------------------------
    // To "transpose" a matrix, swap the rows and columns.

    CMatrix<T> Transpose();

    // ---------------------------------------------------------------------------
    // Scalar multiplier. Multiplies each cell with the provided value. Note that
    // this can also be used to create a negative matrix by multiplying with -1

    CMatrix<T> operator*(const int nVal);

    // ---------------------------------------------------------------------------
    // Matrix to matrix multiplication. The row count of matrix one must be the
    // same as the column count of matrix two.
    CMatrix<T> operator*(const CMatrix<T> & Matrix);

    // ---------------------------------------------------------------------------
    // Matrix to matrix addition. The two matrices must be the same size, i.e. the
    // rows must match in size, and the columns must match in size

    CMatrix<T> operator+(const CMatrix<T> & Matrix);

    // ---------------------------------------------------------------------------
    // Matrix to matrix subtraction. The two matrices must be the same size, i.e. the
    // rows must match in size, and the columns must match in size

    CMatrix<T> operator-(const CMatrix<T> & Matrix);

    // ---------------------------------------------------------------------------
    // Assignment operator is needed for deep copies
    CMatrix<T> & operator=(const CMatrix<T> & Matrix);

private:
    unsigned int m_uRows;
    unsigned int m_uColumns;
    T * m_pMatrix;
};

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T>::CMatrix(unsigned int uRow, unsigned int uCol, T * pData)
{
    unsigned int uNumElements = uRow * uCol;

    m_uRows = uRow;
    m_uColumns = uCol;

    // I treat the 2D matrix as one large block of memory
    m_pMatrix = new T[uNumElements];

    if (pData && uNumElements)
    {
        rsize_t Size = uNumElements * sizeof(T);

        if (memcpy_s(m_pMatrix, Size, pData, uNumElements * sizeof(T)))
        {
            throw CAppException("Source data is incorrectly sized");
        }
    }
    else
    {
        memset(m_pMatrix, 0, uNumElements * sizeof(T));
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T>::CMatrix(const CMatrix<T> & src)
{
    m_uRows = src.m_uRows;
    m_uColumns = src.m_uColumns;
    m_pMatrix = new T[m_uRows * m_uColumns];

    rsize_t Size = m_uRows * m_uColumns * sizeof(T);

    if (memcpy_s(m_pMatrix, Size, src.m_pMatrix, Size))
    {
        throw CAppException("Source data is incorrectly sized");
    }
}


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T>::~CMatrix()
{
    delete[] m_pMatrix;
}

// ---------------------------------------------------------------------------
// The matrix data is represented as a consecutive stream of elements. Each
// element has an index within this stream. Using this method you can obtain
// the elements row and column coordinates.
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::GetRowColFromIndex(unsigned int uElementIndex, unsigned int * puRow, unsigned int * puCol) const
{
    if (uElementIndex >= (m_uRows * m_uColumns))
    {
        throw CAppException("Index out of range");
    }

    *puRow = uElementIndex / m_uColumns;
    *puCol = uElementIndex - m_uColumns * *puRow;
}

// ---------------------------------------------------------------------------
// Retrive an element at the specified coordinates.
// ---------------------------------------------------------------------------

template <class T>
T CMatrix<T>::GetAt(unsigned int uRow, unsigned int uCol) const
{
    assert(uRow <= m_uRows);
    assert(uCol <= m_uColumns);

    return(m_pMatrix[uRow * m_uColumns + uCol]);
}

// ---------------------------------------------------------------------------
// Set an element at the specified coordinates.
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::SetAt(unsigned int uRow, unsigned int uCol, T Element)
{
    assert(uRow <= m_uRows);
    assert(uCol <= m_uColumns);
    m_pMatrix[uRow * m_uColumns + uCol] = Element;
}

// ---------------------------------------------------------------------------
// Retrieve all data from the matrix and place it in the provided buffer of
// type 'T'. The uNumElements is the number of type 'T' elements that the
// provided buffer can hold. Note that this is NOT the same as the number of
// bytes, unless of course each element is one byte character.
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::GetAllData(unsigned int * puNumElements, T * pBuffer) const
{
    unsigned int uSizeNeeded = m_uRows * m_uColumns * sizeof(T);

    if (*puNumElements == 0 && pBuffer == NULL)
    {
        *puNumElements = m_uRows * m_uColumns;
    }
    else if (*puNumElements < uSizeNeeded)
    {
        throw CAppException("Buffer size too small.");
    }
    else
    {
        memcpy_s(pBuffer, *puNumElements * sizeof(T), m_pMatrix, uSizeNeeded)
    }
}

// ---------------------------------------------------------------------------
// Retrieve all data for an entire column.
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::GetColumnData(unsigned int uColumnIndex, unsigned int * puNumElements, T * pBuffer) const
{
    if (uColumnIndex >= m_uColumns)
    {
        throw CAppException("Column index out of range");
    }

    if (*puNumElements == 0 && pBuffer == NULL)
    {
        *puNumElements = m_uRows;
    }
    else if (m_uRows > *puNumElements)
    {
        throw CAppException("Buffer size too small.");
    }
    else
    {
        for (unsigned int uRowIndex = 0; uRowIndex < m_uRows; uRowIndex++)
        {
            T val = GetAt(uRowIndex, uColumnIndex);
            pBuffer[uRowIndex] = val;
        }

        *puNumElements = m_uRows;
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::GetRowData(unsigned int uRowIndex, unsigned int * puNumElements, T * pBuffer) const
{
    if (uRowIndex >= m_uRows)
    {
        throw CAppException("Row index out of range");
    }

    if (*puNumElements == 0 && pBuffer == NULL)
    {
        *puNumElements = m_uColumns;
    }
    else if (m_uColumns > *puNumElements)
    {
        throw CAppException("Buffer size too small.");
    }
    else
    {
        unsigned int uStartIndex = uRowIndex * m_uColumns;
        rsize_t Size = m_uColumns * sizeof(T);

        memcpy_s(pBuffer, Size, &m_pMatrix[uStartIndex], Size);
        *puNumElements = m_uColumns;
    }
}

// ---------------------------------------------------------------------------
// To "transpose" a matrix, swap the rows and columns.

template <class T>
CMatrix<T> CMatrix<T>::Transpose()
{
    CMatrix<T> result(m_uColumns, m_uRows);

    for (unsigned int uRowIndex = 0; uRowIndex < m_uRows; uRowIndex++)
    {
        for (unsigned int uColIndex = 0; uColIndex < m_uColumns; uColIndex++)
        {
            T val = GetAt(uRowIndex, uColIndex);
            result.SetAt(uColIndex, uRowIndex, val);
        }
    }

    return(result);
}

// ---------------------------------------------------------------------------
// Scalar matrix multiplication. This is the simplest where we multiply each
// matrix element with the provided number.
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T> CMatrix<T>::operator*(const int nVal)
{
    T * pMatrix = new T[m_uRows * m_uColumns];
    
    unsigned int uNumElements = m_uRows * m_uColumns;

    for (unsigned int uIdx = 0; uIdx < uNumElements; uIdx++)
    {
        pMatrix[uIdx] = m_pMatrix[uIdx] * nVal;
    }

    return(CMatrix<T>(m_uRows, m_uColumns, pMatrix));
}

// ---------------------------------------------------------------------------
// Multiply one matrix with another matrix is more complex.
// The main condition of matrix multiplication is that the number of columns
// of the 1st matrix must equal to the number of rows of the 2nd one.
// As a result of multiplication you will get a new matrix that has the same
// quantity of rows as the 1st one has and the same quantity of columns as
// the 2nd one.
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T> CMatrix<T>::operator*(const CMatrix<T> & Matrix)
{
    // Check for matrix conditions to be valid

    if (m_uColumns != Matrix.m_uRows)
    {
        throw CAppException("Number of columns of the 1st matrix must equal to the number of rows of the 2nd.");
    }

    T * pMatrix1Data = NULL;
    T * pMatrix2Data = NULL;
    CMatrix<T> Product(m_uRows, Matrix.m_uColumns);

    try
    {
        unsigned int uNumElementsMatrix1 = m_uColumns;
        unsigned int uNumElementsMatrix2 = m_uColumns;

        pMatrix1Data = new T[m_uColumns];
        pMatrix2Data = new T[m_uColumns];

        // ---------------------------------------------------------------
        // And now the matrix multiplication starts.

        for (unsigned int uRowIndex = 0; uRowIndex < m_uRows; uRowIndex++)
        {
            GetRowData(uRowIndex, &uNumElementsMatrix1, pMatrix1Data);
            assert(uNumElementsMatrix1 = m_uColumns);

            for (unsigned int uMatrix2ColIndex = 0; uMatrix2ColIndex < Matrix.m_uColumns; uMatrix2ColIndex++)
            {
                Matrix.GetColumnData(uMatrix2ColIndex, &uNumElementsMatrix2, pMatrix2Data);
                assert(uNumElementsMatrix1 == uNumElementsMatrix2);

                // Now we can start doing the 'Dot Product' with each cell

                T val = Product.GetAt(uRowIndex, uMatrix2ColIndex);

                for (unsigned int uDotIndex = 0; uDotIndex < uNumElementsMatrix1; uDotIndex++)
                {
                    val += (pMatrix1Data[uDotIndex] * pMatrix2Data[uDotIndex]);
                }

                Product.SetAt(uRowIndex, uMatrix2ColIndex, val);
            }
        }
    }
    catch (...)
    {
        delete[] pMatrix1Data;
        delete[] pMatrix2Data;
        throw;
    }

    delete[] pMatrix1Data;
    delete[] pMatrix2Data;

    return(Product);
}

// ---------------------------------------------------------------------------
// Matrix to matrix addition. The two matrices must be the same size, i.e. the
// rows must match in size, and the columns must match in size

template <class T>
CMatrix<T> CMatrix<T>::operator+(const CMatrix<T> & Matrix)
{
    if (m_uRows != Matrix.m_uRows || m_uColumns != Matrix.m_uColumns)
    {
        throw CAppException("Matrixes must be the same size to add them.");
    }

    unsigned int uElementCount = m_uRows * m_uColumns;
    T * pSum = new T[uElementCount];

    for (unsigned int uIndex = 0; uIndex < uElementCount; uIndex++)
    {
        pSum[uIndex] = m_pMatrix[uIndex] + Matrix.m_pMatrix[uIndex];
    }

    CMatrix<T> p(m_uRows, m_uColumns, pSum);
    delete[] pSum;

    return(p);
}

// ---------------------------------------------------------------------------
// Matrix to matrix subtraction. The two matrices must be the same size, i.e. the
// rows must match in size, and the columns must match in size

template <class T>
CMatrix<T> CMatrix<T>::operator-(const CMatrix<T> & Matrix)
{
    if (m_uRows != Matrix.m_uRows || m_uColumns != Matrix.m_uColumns)
    {
        throw CAppException("Matrixes must be the same size to add them.");
    }

    unsigned int uElementCount = m_uRows * m_uColumns;
    T * pSum = new T[uElementCount];

    for (unsigned int uIndex = 0; uIndex < uElementCount; uIndex++)
    {
        pSum[uIndex] = m_pMatrix[uIndex] - Matrix.m_pMatrix[uIndex];
    }

    CMatrix<T> p(m_uRows, m_uColumns, pSum);
    delete[] pSum;

    return(p);
}

// ---------------------------------------------------------------------------

template <class T>
CMatrix<T> & CMatrix<T>::operator=(const CMatrix<T> & Matrix)
{
    m_uRows = Matrix.m_uRows;
    m_uColumns = Matrix.m_uColumns;
    m_pMatrix = new T[m_uRows * m_uColumns];

    rsize_t Size = m_uRows * m_uColumns * sizeof(T);

    if (memcpy_s(m_pMatrix, Size, Matrix.m_pMatrix, Size))
    {
        throw CAppException("Source data is incorrectly sized");
    }

    return (*this);
}

