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

    CMatrix(unsigned int uRow, unsigned int uCol, T * pData = NULL, unsigned int uNumElements = 0);
    CMatrix(const CMatrix<T> & src);
    ~CMatrix();

    inline unsigned int NumRows() { return(m_uRows); }
    inline unsigned int NumColumns() { return(m_uColumns); }

    void GetRowColFromIndex(unsigned int uElementIndex, unsigned int * puRow, unsigned int * puCol) const;

    T GetAt(unsigned int uRow, unsigned int uCol) const;
    void SetAt(unsigned int uRow, unsigned int uCol, T Element);
    bool GetAllData(unsigned int uNumElements, T * pData) const;
    T * GetRowData(unsigned int uRowIndex, unsigned int * puNumElements) const;
    T * GetColumnData(unsigned int uColumnIndex, unsigned int * puNumElements) const;

    // Scalar multiplier
    CMatrix<T> operator*(const int nVal);

    // Matrix to matrix multiplication
    CMatrix<T> operator*(const CMatrix<T> & Matrix);
    CMatrix<T> & operator=(const CMatrix<T> & Matrix);

private:
    unsigned int m_uRows;
    unsigned int m_uColumns;
    T * m_pMatrix;
};

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
CMatrix<T>::CMatrix(unsigned int uRow, unsigned int uCol, T * pData, unsigned int uNumElements)
{
    m_uRows = uRow;
    m_uColumns = uCol;

    // I treat the 2D matrix as one large block of memory
    m_pMatrix = new T[uRow * uCol];

    if (pData && uNumElements)
    {
        rsize_t Size = uRow * uCol * sizeof(T);

        if (memcpy_s(m_pMatrix, Size, pData, uNumElements * sizeof(T)))
        {
            throw CAppException("Source data is incorrectly sized");
        }
    }
    else
    {
        memset(m_pMatrix, 0, uRow * uCol * sizeof(T));
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
// ---------------------------------------------------------------------------

template <class T>
T CMatrix<T>::GetAt(unsigned int uRow, unsigned int uCol) const
{
    assert(uRow <= m_uRows);
    assert(uCol <= m_uColumns);

    return(m_pMatrix[uRow * m_uColumns + uCol]);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::SetAt(unsigned int uRow, unsigned int uCol, T Element)
{
    assert(uRow <= m_uRows);
    assert(uCol <= m_uColumns);
    m_pMatrix[uRow * m_uColumns + uCol] = Element;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
bool CMatrix<T>::GetAllData(unsigned int uNumElements, T * pData) const
{
    unsigned int uSizeNeeded = m_uRows * m_uColumns * sizeof(T);
    return(memcpy_s(pData, uNumElements * sizeof(T), m_pMatrix, uSizeNeeded) ? false : true);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
T * CMatrix<T>::GetColumnData(unsigned int uColumnIndex, unsigned int * puNumElements) const
{
    if (uColumnIndex >= m_uColumns)
    {
        throw CAppException("Column index out of range");
    }

    T * pRetData = new T[m_uRows];

    for (unsigned int uRowIndex = 0; uRowIndex < m_uRows; uRowIndex++)
    {
        T val = GetAt(uRowIndex, uColumnIndex);
        pRetData[uRowIndex] = val;
    }

    *puNumElements = m_uRows;

    return(pRetData);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

template <class T>
T * CMatrix<T>::GetRowData(unsigned int uRowIndex, unsigned int * puNumElements) const
{
    if (uRowIndex >= m_uRows)
    {
        throw CAppException("Row index out of range");
    }

    T * pRetData = new T[m_uColumns];
    unsigned int uStartIndex = uRowIndex * m_uColumns;
    rsize_t Size = m_uColumns * sizeof(T);

    memcpy_s(pRetData, Size, &m_pMatrix[uStartIndex], Size);

    *puNumElements = m_uColumns;

    return(pRetData);
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

    return(CMatrix<T>(m_uRows, m_uColumns, pMatrix, m_uRows * m_uColumns));
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

    CMatrix<T> Product(m_uRows, Matrix.m_uColumns);

    for (unsigned int uRowIndex = 0; uRowIndex < m_uRows; uRowIndex++)
    {
        unsigned int uNumElementsMatrix1;
        T * pMatrix1Data = GetRowData(uRowIndex, &uNumElementsMatrix1);
        assert(uNumElementsMatrix1 = m_uColumns);

        for (unsigned int uMatrix2ColIndex = 0; uMatrix2ColIndex < Matrix.m_uColumns; uMatrix2ColIndex++)
        {
            unsigned int uNumElementsMatrix2;
            T * pMatrix2Data = Matrix.GetColumnData(uMatrix2ColIndex, &uNumElementsMatrix2);
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

    return(Product);
}

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
