// MatrixMultiplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CAppException.h"
#include "CMatrix.h"
#include "CStopwatch.h"

void PrintMatrix(CMatrix<int> & m)
{
    printf("Rows: %u, Columns: %u\n\n", m.NumRows(), m.NumColumns());

    for (unsigned int uRow = 0; uRow < m.NumRows(); uRow++)
    {
        for (unsigned int uCol = 0; uCol < m.NumColumns(); uCol++)
        {
            printf("%d", m.GetAt(uRow, uCol));

            if ((uCol + 1) < m.NumColumns())
            {
                putchar(',');
            }
        }
        putchar('\n');
    }

    putchar('\n');
}

int main()
{
    int nRet = -1;
    int Data1[] = { 1, 2, 3, 4, 5, 6 };
    int Data2[] = { 7, 8, 9, 10, 11, 12};

    try
    {
        CMatrix<int> m1(2, 3, Data1);
        CMatrix<int> m2(2, 3, Data2);
        
        puts("Matrix 1");
        PrintMatrix(m1);

        puts("Matrix 2");
        PrintMatrix(m2);

        CStopwatch stopWatch;
        stopWatch.Start();
        CMatrix<int> p = m1 + m2;
        float fElapsedSecs = stopWatch.Stop();

        printf("Completed in %f seconds.", fElapsedSecs);

        puts("Matrix Product");
        PrintMatrix(p);

        nRet = 0;
    }
    catch (CAppException ex)
    {
        printf(ex.what());
    }
    catch (...)
    {
        printf("Unknown exception");
    }

    return(nRet);
}

