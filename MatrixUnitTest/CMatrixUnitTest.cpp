#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\MatrixMultiplication\CMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define TEST_MY_TRAIT(traitValue) TEST_METHOD_ATTRIBUTE(L"Integer Array Testing", traitValue)

namespace MatrixUnitTest
{		
	TEST_CLASS(MatrixIntegerTest)
	{
	public:
        BEGIN_TEST_METHOD_ATTRIBUTE(GetCells)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Basic Cell Access")
        END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(GetCells)
		{
            Logger::WriteMessage("Getting Matrix Cells");

            int Data[] = { 1, 2, 3, 4, 5, 6 };

            CMatrix<int> m(2, 3, Data, sizeof(Data) / sizeof(int));
            Assert::AreEqual(1, m.GetAt(0, 0));
            Assert::AreEqual(2, m.GetAt(0, 1));
            Assert::AreEqual(3, m.GetAt(0, 2));
            Assert::AreEqual(4, m.GetAt(1, 0));
            Assert::AreEqual(5, m.GetAt(1, 1));
            Assert::AreEqual(6, m.GetAt(1, 2));
		}

        BEGIN_TEST_METHOD_ATTRIBUTE(SetCells)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Basic Cell Access")
            END_TEST_METHOD_ATTRIBUTE()

            TEST_METHOD(SetCells)
        {
            Logger::WriteMessage("Setting Matrix Cells");

            int Data[] = { 1, 2, 3, 4, 5, 6 };

            CMatrix<int> m(2, 3, Data, sizeof(Data) / sizeof(int));
            m.SetAt(0, 0, 10);
            m.SetAt(0, 1, 11);
            m.SetAt(0, 2, 12);
            m.SetAt(1, 0, 13);
            m.SetAt(1, 1, 14);
            m.SetAt(1, 2, 15);

            Logger::WriteMessage("Getting Matrix Cells after updating them.");

            Assert::AreEqual(10, m.GetAt(0, 0));
            Assert::AreEqual(11, m.GetAt(0, 1));
            Assert::AreEqual(12, m.GetAt(0, 2));
            Assert::AreEqual(13, m.GetAt(1, 0));
            Assert::AreEqual(14, m.GetAt(1, 1));
            Assert::AreEqual(15, m.GetAt(1, 2));
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(ReadMatrixRow)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Read each matrix row")
            END_TEST_METHOD_ATTRIBUTE()

            TEST_METHOD(ReadMatrixRow)
        {
            int Data[] = { 1, 2, 3, 4, 5, 6 };

            CMatrix<int> m(2, 3, Data, sizeof(Data) / sizeof(int));

            unsigned int uNumElements;
            int * pRowData = m.GetRowData(0, &uNumElements);
            Assert::IsNotNull(pRowData);

            Logger::WriteMessage("Expecting 3 elements to be returned");
            Assert::AreEqual((unsigned int)3, uNumElements);

            Assert::AreEqual(1, pRowData[0]);
            Assert::AreEqual(2, pRowData[1]);
            Assert::AreEqual(3, pRowData[2]);

            delete pRowData;

            pRowData = m.GetRowData(1, &uNumElements);
            Assert::IsNotNull(pRowData);

            Logger::WriteMessage("Expecting 3 elements to be returned");
            Assert::AreEqual((unsigned int)3, uNumElements);

            Assert::AreEqual(4, pRowData[0]);
            Assert::AreEqual(5, pRowData[1]);
            Assert::AreEqual(6, pRowData[2]);

            delete pRowData;

            try
            {
                pRowData = m.GetRowData(2, &uNumElements);
                Logger::WriteMessage("An exception was expected to be thrown");
                Assert::IsFalse(true);
            }
            catch (CAppException ex)
            {
                Logger::WriteMessage(ex.what());
                Assert::AreEqual(ex.what(), "Row index out of range");
            }
            catch (...)
            {
                Logger::WriteMessage("This exception was not expected.");
                Assert::IsFalse(true);
            }
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(ReadMatrixCol)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Read each matrix column")
            END_TEST_METHOD_ATTRIBUTE()

            TEST_METHOD(ReadMatrixCol)
        {
            int Data[] = { 1, 2, 3, 4, 5, 6 };

            CMatrix<int> m(2, 3, Data, sizeof(Data) / sizeof(int));

            // --------------------------------------------------------------

            Logger::WriteMessage("Reading column 1 of 3");

            unsigned int uNumElements;
            int * pColData = m.GetColumnData(0, &uNumElements);
            Assert::IsNotNull(pColData);

            Logger::WriteMessage("Expecting 2 elements to be returned");
            Assert::AreEqual((unsigned int)2, uNumElements);

            Assert::AreEqual(1, pColData[0]);
            Assert::AreEqual(4, pColData[1]);

            delete pColData;

            // --------------------------------------------------------------

            Logger::WriteMessage("Reading column 2 of 3");

            pColData = m.GetColumnData(1, &uNumElements);
            Assert::IsNotNull(pColData);

            Logger::WriteMessage("Expecting 2 elements to be returned");
            Assert::AreEqual((unsigned int)2, uNumElements);

            Assert::AreEqual(2, pColData[0]);
            Assert::AreEqual(5, pColData[1]);

            delete pColData;

            // --------------------------------------------------------------

            Logger::WriteMessage("Reading column 3 of 3");

            pColData = m.GetColumnData(2, &uNumElements);
            Assert::IsNotNull(pColData);

            Logger::WriteMessage("Expecting 2 elements to be returned");
            Assert::AreEqual((unsigned int)2, uNumElements);

            Assert::AreEqual(3, pColData[0]);
            Assert::AreEqual(6, pColData[1]);

            delete pColData;

            // --------------------------------------------------------------

            try
            {
                pColData = m.GetColumnData(3, &uNumElements);
                Logger::WriteMessage("An exception was expected to be thrown");
                Assert::IsFalse(true);
            }
            catch (CAppException ex)
            {
                Logger::WriteMessage(ex.what());
                Assert::AreEqual(ex.what(), "Column index out of range");
            }
            catch (...)
            {
                Logger::WriteMessage("This exception was not expected.");
                Assert::IsFalse(true);
            }
        }

        BEGIN_TEST_METHOD_ATTRIBUTE(ScalarMatrixMultiplication)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Scalar Matrix Multiplication")
            END_TEST_METHOD_ATTRIBUTE()

            TEST_METHOD(ScalarMatrixMultiplication)
        {
            int Data[] = { 1, 2, 3, 4, 5, 6 };

            CMatrix<int> m(2, 3, Data, sizeof(Data) / sizeof(int));

            CMatrix<int> p = m * 2;

            Assert::AreEqual(2, p.GetAt(0, 0));
            Assert::AreEqual(4, p.GetAt(0, 1));
            Assert::AreEqual(6, p.GetAt(0, 2));
            Assert::AreEqual(8, p.GetAt(1, 0));
            Assert::AreEqual(10, p.GetAt(1, 1));
            Assert::AreEqual(12, p.GetAt(1, 2));
        }

        // -------------------------------------------------------------------

        BEGIN_TEST_METHOD_ATTRIBUTE(TwoMatrixMultiplication)
            TEST_OWNER(L"Martin Fallenstedt")
            TEST_PRIORITY(1)
            TEST_MY_TRAIT(L"Two Matrix Multiplication")
            END_TEST_METHOD_ATTRIBUTE()

            TEST_METHOD(TwoMatrixMultiplication)
        {
            int Data1[] = { 1, 2, 3, 4, 5, 6 };
            int Data2[] = { 7, 8, 9, 10, 11, 12 };

            CMatrix<int> m1(2, 3, Data1, sizeof(Data1) / sizeof(int));
            CMatrix<int> m2(3, 2, Data2, sizeof(Data2) / sizeof(int));

            CMatrix<int> p = m1 * m2;

            Assert::AreEqual(58, p.GetAt(0, 0));
            Assert::AreEqual(64, p.GetAt(0, 1));
            Assert::AreEqual(139, p.GetAt(1, 0));
            Assert::AreEqual(154, p.GetAt(1, 1));
        }
	};
}
